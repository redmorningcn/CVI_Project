/*******************************************************************************
 *   Filename:       ansytime.c
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *   Date:			 20170927
 *   Description:
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include "asynctmr.h"
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "cvi_bspComm.h"		 				//串口通讯
#include "cvi_config.h"		 					//串口通讯   

/********************************************************************************************/
/* Constants																				*/
/********************************************************************************************/
//uir文件名

/********************************************************************************************/
/* Globals																					*/
/********************************************************************************************/


/********************************************************************************************/
/* local Prototypes																			*/
/********************************************************************************************/


//----------------------------------------------------------------------------
// 功    能： 串口的收、发控制.通过标识位，线程，控制程序收发协调
// 参    数：
// 作    者： redmornigcn 20170927
//----------------------------------------------------------------------------
static int CVICALLBACK Thread_CommRecvAndSendCtrl (void *functionData)
{
	char recbuf[COMM_DATA_BUF_SZIE];
	char sendbuf[COMM_DATA_BUF_SZIE];
	int	 reclen;
	stcCsnrProtocolPara sCsncPara;		//csns結構

	while (!sCtrl.PC.ThreadExit)								//线程不退出
	{
//////////////////////////////////////////////////////////////////接收
		if(cvi_GetComRecEndFlg(sCtrl.PC.sCom))					//接收数据，并显示
		{
			cvi_ClearComRecEndFlg(sCtrl.PC.sCom);				//清空接收标识
			reclen = cvi_ReadCom(sCtrl.PC.sCom,recbuf,sizeof(recbuf));	
																//取串口数据

			sCsncPara.rxtxbuf 	= recbuf;						//原始数据
			sCsncPara.rxtxlen 	= reclen;						//原始数据长度
			sCsncPara.databuf	= sCtrl.PC.rd;					//接收数据区

			char	flg = 0;
			flg = DataUnpack_CSNC(&sCsncPara);					//解包处理

		    if ( MASTE_ADDR_HOST != sCsncPara.destaddr || flg == 0 ) //解析错误、地址错误，退出。
		    {
				sCtrl.PC.RxCtrl.recvflg = 0;					//接收失败
	        }
	        else
	        {
				sCtrl.PC.RxCtrl.SourceAddr 	= sCsncPara.sourceaddr; 			//源地址
				sCtrl.PC.RxCtrl.DestAddr	= sCsncPara.destaddr;				//目标地址
				sCtrl.PC.RxCtrl.FramNum		= sCsncPara.framnum;				//接收到的帧号
				sCtrl.PC.RxCtrl.FrameCode	= (char)sCsncPara.framcode & (0x0f);//指令码 ，测量装置未定义
				sCtrl.PC.RxCtrl.Len			= sCsncPara.datalen;				//数据区长度

				sCtrl.PC.RxCtrl.recvflg 	= 1;								//接收成功( 与应用层接口 )
				sCtrl.PC.RxCtrl.printflg 	= 1;								//数据打印（CVI）
	        }
		}

////////////////////////////////////////////////////////////////数据发送
		if(
					sCtrl.PC.sCom->sConfig.open == 1			//串口已打开
				&&  sCtrl.PC.ConnCtrl.SendFlg 	!= 0			//数据发送有数据发送
		  )
		{
			sCsncPara.databuf	= sCtrl.PC.wr;					//接收数据区
			sCsncPara.datalen	= (unsigned char)sCtrl.PC.ConnCtrl.sendlen;	//发送数据区长度
			sCsncPara.rxtxbuf 	= (uint8 *)sendbuf;				//

			sCsncPara.sourceaddr= sCtrl.PC.ConnCtrl.sourceaddr;	//地址
			sCsncPara.destaddr  = sCtrl.PC.ConnCtrl.DestAddr;
			sCsncPara.framnum   = sCtrl.PC.ConnCtrl.SendFramNum;
			sCsncPara.framcode	= sCtrl.PC.ConnCtrl.FrameCode;

			DataPackage_CSNC(&sCsncPara);						//按csnc协议打包

			cvi_SendCom(sCtrl.PC.sCom, sCsncPara.rxtxbuf, sCsncPara.rxtxlen);//通过串口发送数据

			sCtrl.PC.ConnCtrl.SendFlg 	= 0;					//置发送位无效.本次发送结束()
		}
	}

	return 0;
}

//----------------------------------------------------------------------------
// 功    能： 初始化收发线程。为实现com1的通讯，必须开启该函数，启动处理线程
// 参    数：
// 作    者： redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_InitBspComm(stcThreadCommCtrl * T_Ctrl)
{
	if(T_Ctrl->ThreadID == 0)				//初始化收发控制线程
	{
		T_Ctrl->sCom = &gsCom1Contrl;		//串口1建立关联
		T_Ctrl->ThreadExit = 0;				//线程不释放

		cvi_OpenCom(T_Ctrl->sCom);			//打开该该线程对应的串口
		
		CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,
							   Thread_CommRecvAndSendCtrl,		//该线程处理函数
							   NULL,
							   &T_Ctrl->ThreadID);
	}
	
	return	T_Ctrl->ThreadID;				//返回线程ID
}

//----------------------------------------------------------------------------
// 功    能： 关闭收发线程
// 参    数：
// 作    者： redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_CloseBspComm(stcThreadCommCtrl * T_Ctrl)
{
	if( T_Ctrl->ThreadID )					//关闭并释放线程
	{
		T_Ctrl->ThreadExit = 1;				//线程退出，置线程释放标识
		/* Wait for the thread functions to finish executing */
		CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,
												T_Ctrl->ThreadID,
												OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
		/* Release thread functions */
		CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, T_Ctrl->ThreadID);
		
		cvi_CloseCom(T_Ctrl->sCom);			//打开该该线程对应的串口
	}

	return	T_Ctrl->ThreadID;
}
