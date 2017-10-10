/*******************************************************************************
 *   Filename:       cvi_bspComm.h
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/
#ifndef	CVI_BSPCOMM_H
#define	CVI_BSPCOMM_H

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <formatio.h>

#include "csnr_package_deal.h" 
#include "cvi_com1.h"   


/********************************************************************************************/
// Constants
/********************************************************************************************/
#define		COMM_DATA_BUF_SZIE		256
#define		MASTE_ADDR_HOST			0xCA

/********************************************************************************************/
// struct
/********************************************************************************************/
//接收控制结构体
struct stcRecvCtrl
{
	unsigned char		DestAddr;			//接收地址		slave  =0xA1\A2
	unsigned char		SourceAddr; 		//源地址		master	 =0x80
	unsigned char		FramNum;			//帧序号
	unsigned char		Len;				//接收有效数据长度
	unsigned char		FrameCode;			//帧控制字
	unsigned char		recvflg;			//接收到有效数据
	unsigned char 		printflg;			//数据打印
	unsigned char		Tmp[1];
	unsigned int		DataCode;			//接收控制字
} ;

//连接控制结构体
struct stcConnCtrl
{
	unsigned char		ConnFlg;		//连接控制, 1，允许连接，0，不允许连接
	unsigned char		RecvEndFlg; 	//接收标示，1，数据接收完成，0，无数据接收。
	unsigned char		TimeOut;		//超时时间，单位1s
	unsigned char		ErrFlg; 		//错误标示，连接正常，0；连接错误，1
	unsigned char		DestAddr;		//接收地址	slave  = 0xCA
	unsigned char		sourceaddr; 	//源地址	master = 0x80
	unsigned char		SendFramNum;	//帧序号
	unsigned char		SendFlg;		//发送标示， 有数据发送，1；无数据发送，0
	unsigned char		FrameCode;		//帧控制字
	int					sendlen;		//发送数据长度
} ;

//pc串口控制
typedef struct _stcThreadCommCtrl_
{
	unsigned char		wr[COMM_DATA_BUF_SZIE];			//发送区
	unsigned char		rd[COMM_DATA_BUF_SZIE];			//接收区
	struct 	stcRecvCtrl	RxCtrl;							//接收控制
	struct 	stcConnCtrl	ConnCtrl;						//发送控制
	stcUartContrl		*sCom;							//该线程对应的串口
	int					ThreadID;						//该线ID
	int					ThreadExit;						//该线释放标识

} stcThreadCommCtrl;


/********************************************************************************************/
/* Globals																					*/
/********************************************************************************************/
//----------------------------------------------------------------------------
// 功    能： 初始化收发线程。为实现com1的通讯，必须开启该函数，启动处理线程
// 参    数：
// 作    者： redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_InitBspComm(stcThreadCommCtrl * T_Ctrl);

//----------------------------------------------------------------------------
// 功    能： 关闭收发线程。退出文件处理时，需退出线程，释放资源。
// 参    数：
// 作    者： redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_CloseBspComm(stcThreadCommCtrl * T_Ctrl);


/********************************************************************************************/
/* Prototypes																				*/
/********************************************************************************************/

#endif
