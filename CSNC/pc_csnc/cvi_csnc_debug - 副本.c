#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include "pc_csnr.h"
#include "cvi_com_operate.h"
#include "csnr_package_deal.h"
#include "cvi_file.h"
#include "senddata.h"
#include "setpara.h"
#include "oilrecord.h"
#include "databasesample.h"
#include "csnr_package_deal.h"


/********************************************************************************************/
/* Constants										    */
/********************************************************************************************/
//uir文件名
#define		UIR_MAIN_NAME			"csnc_debug.uir"

//打印接收信息
//#define		RECV_PRINTF	    1


/********************************************************************************************/
/* Globals										    */
/********************************************************************************************/
//面板变量
int 		gPanelHandle;										//装置主面板

static int		gthreadFunctionId1;									//线程编号
static int 		gExiting;											//线程退出

int		gSendDataFlg = 0;

//串口结构体及通讯
extern	stcUartConfig   	gsCom1Config;								//串口1配置
extern	stcUartContrl		gsCom1Contrl;								//串口1通讯
extern	stcCsnrProtocolPara	gsCsnrProtocolPara;							//协议端口

typedef 	struct	_stcmainpara_
{
	int		sendfmt;
	int		recvfmt;
}stcmainpara;

stcmainpara		gsmainpara;

/********************************************************************************************/
/* local Prototypes																				*/
/********************************************************************************************/
int		GetMainPanelHandle(void);										//主面板
void 	printftext(int	panel,int id,const char *format, ...);
void 	CloseCom1(void);
char	WriteMainPanelData_File(void) ;
int		ReadMainPanelData_File(void) ;
static int 	CVICALLBACK ThreadFunction1 (void *functionData);
extern	void	SendTextBox(void);
/********************************************************************************************/
/* Application entry point.																	*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)					/* Initialize CVI libraries */
		return -1;	/* out of memory */

    gPanelHandle 		= LoadPanel (0, UIR_MAIN_NAME, PANEL);

	gExiting = 0;									//加入线程
	/* Schedule two thread functions */				//多线程，创建新线程函数
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,
								   ThreadFunction1, NULL,
								   &gthreadFunctionId1);
	/* 	Display the panel and run the UI */
	DisplayPanel (gPanelHandle);

	RunUserInterface ();

	CloseCom1();
	WriteMainPanelData_File();

	gExiting = 1;									//线程退出
	/* Wait for the thread functions to finish executing */
	CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,
											gthreadFunctionId1,
											OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	/* Release thread functions */
	CmtReleaseThreadPoolFunctionID (DEFAULT_THREAD_POOL_HANDLE, gthreadFunctionId1);

	/*	Discard the loaded panels from memory */
	DiscardPanel (gPanelHandle);

	return 0;
}





#define		COMM_DATA_BUF_SZIE	512
#define		MASTE_ADDR_HOST		0xCA


struct stcRecvCtrl
{
	unsigned char		DestAddr;			//接收地址		slave  =0xA1\A2
	unsigned char		SourceAddr; 		//源地址		master	 =0x80
	unsigned char		FramNum;			//帧序号
	unsigned char		Len;				//接收有效数据长度
	unsigned char		FrameCode;			//帧控制字
	unsigned char		recvflg;
	unsigned char		Tmp[2];
	unsigned int		DataCode;			//接收控制字
} ;

struct stcConnCtrl
{
	unsigned char		ConnFlg;		//连接控制, 1，允许连接，0，不允许连接
	unsigned char		RecvEndFlg; 	//接收标示，1，数据接收完成，0，无数据接收。
	unsigned char		TimeOut;		//超时时间，单位1s
	unsigned char		ErrFlg; 		//错误标示，连接正常，0；连接错误，1
	unsigned char		DestAddr;		//接收地址		   slave  =0xCA
	unsigned char		sourceaddr; 	//源地址		   master	=0x80
	unsigned char		SendFramNum;	//帧序号
	unsigned char		SendFlg;		//发送标示， 有数据发送，1；无数据发送，0
	unsigned char		FrameCode;			//帧控制字
	int					sendlen;		//发送数据长度
} ;

typedef struct _stcThreadCommCtrl_
{
	char		wr[COMM_DATA_BUF_SZIE];			//发送区
	char		rd[COMM_DATA_BUF_SZIE];			//接收区

	stcRecvCtrl	RxCtrl;							//接收控制
	stcConnCtrl	ConnCtrl;						//发送控制
} stcThreadCommCtrl;


//----------------------------------------------------------------------------
// 功    能： 初始化收发线程
// 参    数：
// 作    者： redmornigcn 20170927
//----------------------------------------------------------------------------
int	cvi_InitBspComm(void)
{
	if(gthreadFunctionId1 == 0)					//初始化收发控制线程
	{
		CmtWaitForThreadPoolFunctionCompletion (DEFAULT_THREAD_POOL_HANDLE,
												gthreadFunctionId1,
												OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
	}
	return	gthreadFunctionId1;
}


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

	while (!gComThreadExiting)									//线程退出
	{
////////////////////////////////////////////////////////////////接收
		if(cvi_GetComRecEndFlg(&sCtrl.sCom1))					//接收数据，并显示
		{
			cvi_ClearComRecEndFlg(&sCtrl.sCom1);				//清空接收标识
			reclen = cvi_ReadCom(&sCtrl,recbuf,sizeof(recbuf));	//取串口数据

			sCsncPara.rxtxbuf 	= (uint8 *)recbuf;				//原始数据
			sCsncPara.rxtxlen 	= reclen;						//原始数据长度
			sCsncPara.databuf	= sCtrl.sThreadCom1.rd;			//接收数据区

			char	flg = DataUnpack_CSNC(&sCsncPara);			//解包处理

		    if ( MASTE_ADDR_HOST != sCsncPara.destaddr || flg == 0 ) //解析错误、地址错误，退出。
		    {
				sCtrl.PC.RxCtrl.recvflg = 0;					//接收失败
	        	return 0;
	        }
	        else
	        {
				sCtrl.PC.RxCtrl.SourceAddr 	= sCsncPara.sourceaddr; 			//源地址
				sCtrl.PC.RxCtrl.DestAddr	= sCsncPara.destaddr;				//目标地址
				sCtrl.PC.RxCtrl.FramNum		= sCsncPara.framnum;				//接收到的帧号
				sCtrl.PC.RxCtrl.FrameCode	= (char)sCsncPara.framcode & (0x0f);//指令码 ，测量装置未定义
				sCtrl.PC.RxCtrl.Len			= sCsncPara.datalen;				//数据区长度

				sCtrl.PC.RxCtrl.recvflg 	= 1;								//接收成功( 与应用层接口 )
	        }
		}

////////////////////////////////////////////////////////////////数据发送
		if(
					sCtrl.sCom1.sConfig.open 	== 1			//串口已打开
				&&  sCtrl.PC.ConnCtrl.SendFlg 	!= 0			//数据发送有数据发送
		  )
		{
			sCsncPara.databuf	= sCtrl.PC.wr;					//接收数据区
			sCsncPara.datalen	= sCtrl.PC.ConnCtrl.sendlen;	//发送数据区长度
			sCsncPara.rxtxbuf 	= (uint8 *)sendbuf;				//

			sCsncPara.sourceaddr= sCtrl.PC.ConnCtrl.sourceaddr;	//地址
			sCsncPara.destaddr  = sCtrl.PC.ConnCtrl.DestAddr;
			sCsncPara.framnum   = sCtrl.PC.ConnCtrl.SendFramNum;
			sCsncPara.framcode	= sCtrl.PC.ConnCtrl.FrameCode;

			DataPackage_CSNC(&sCsncPara);						//按csnc协议打包

			cvi_SendCom(&sCtrl.sCom, sCsncPara.rxtxbuf, sCsncPara.rxtxlen)//通过串口发送数据

			sCtrl.PC.ConnCtrl.SendFlg 	= 0;					//置发送位无效.本次发送结束()
		}
	}

	return 0;
}


/********************************************************************************************/
/* PanelCB ():  Quits this application.														*/
/********************************************************************************************/
int CVICALLBACK PanelCB (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
		}
	return 0;
}

/********************************************************************************************/
/* PanelCB ():  Quits this application.														*/
/********************************************************************************************/
int CVICALLBACK Quit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
		{
		case EVENT_COMMIT:
			QuitUserInterface(0);
			break;
		}
	return 0;
}


int CVICALLBACK SetSendFlgCallback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			gSendDataFlg = 1;
			break;
	}
	return 0;
}


int CVICALLBACK ClearTextBoxCallback (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			 ResetTextBox (panel, PANEL_INFOTEXTBOX, "");
			break;
	}
	return 0;
}


int   g_systime = 0;


/********************************************************************************************/
/*CVI_COM_Tick_Handl串口处理句柄,和com面板及周期性事件							            */
/********************************************************************************************/
void CVI_COMPanelHandle(int panel)
{
	static	int	times = 0;

	if(times == 0)
	{
		times++;
		if(ReadCom1ConfigFromFile(&gsCom1Config))	//如串口配置文件有效，直接打开
		{
			Com1Config();							//配置串口
		}
		ReadMainPanelData_File();
	}

	if(gsCom1Config.open == 1)	  //串口打开
	{
		SetCtrlAttribute (panel, PANEL_OPEN_BIN_FILE, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, PANEL_COM1_SEND_DATA, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, OPEN_COM1_PANEL, ATTR_LABEL_TEXT, "关闭串口");
	}

	if(gsCom1Config.open == 0)	  //串口关闭
	{
		SetCtrlAttribute (panel, PANEL_OPEN_BIN_FILE, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, PANEL_COM1_SEND_DATA, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, OPEN_COM1_PANEL, ATTR_LABEL_TEXT, "打开串口");
	}
}

/********************************************************************************************/
/*CVI_Set_Data_Handl串口处理句柄,串口数据相关句柄								            */
/********************************************************************************************/
void CVI_SentDataHandle(int panel)
{
	static	int	autotime = 0;
	double	autoflg = 0;
	int		i= 10;


	GetCtrlVal (panel, PANEL_TIMER, &autoflg);	//取自动发送标示

	GetCtrlAttribute (panel, PANEL_TIMER, ATTR_INTERVAL,&autoflg );

	if( autoflg < 0.01)				//自动发送
	{
		if(autotime < 10 )		//10ms
		{
			GetCtrlVal (panel, PANEL_DELAYMS, &autotime);
			gSendDataFlg = 1;	//设置标示置1
		}
		else
		{
			i = 10;
			while(i--)
				autotime--;
		}
	}else
	{
		 autotime = 0;
	}

////////////////////////////////////////////////////////////取协议内的值
	GetCtrlVal(panel,PANEL_SOURCEADDR,&gsCsnrProtocolPara.sourceaddr);
	GetCtrlVal(panel,PANEL_DESTADDR,&gsCsnrProtocolPara.destaddr);
	GetCtrlVal(panel,PANEL_FRAMNUM,&gsCsnrProtocolPara.framnum);
	GetCtrlVal(panel,PANEL_FRAMCODE,&gsCsnrProtocolPara.framcode);

////////////////////////////////////////////////////////////面板参数
	GetCtrlVal(panel,PANEL_SENDFMT,&gsmainpara.sendfmt);
	GetCtrlVal(panel,PANEL_RECVFMT,&gsmainpara.recvfmt);
}

void	DisplayTimeOnSetPanel(void);
/********************************************************************************************/
/*CVI_SetPara串口参数设置相关句柄												            */
/********************************************************************************************/
void CVI_SetParaHandle(int panel)
{
	DisplayTimeOnSetPanel();
	Com_SetParaTask();									//串口发送参数设置指令
}

int  InsertoilRecToDBTable (void);
/********************************************************************************************/
/*CVI_SetPara串口参数设置相关句柄												            */
/********************************************************************************************/
void CVI_DataBaseHandle(int panel)
{
	if(gsRecvOilRecordCtrl.storeflg == 1 && db_panelHandle)
	{
		gsRecvOilRecordCtrl.storeflg = 0;

		InsertoilRecToDBTable();
	}

	if(db_panelHandle && gsRecvOilRecordCtrl.ICflg == 1)
	{
		SetCtrlVal(db_panelHandle,SETM_PANEL_CUR_NUM,gsRecvOilRecordCtrl.currecnum);
		SetCtrlVal(db_panelHandle,SETM_PANEL_ICREAD_NUM,gsRecvOilRecordCtrl.ICreadnum);
	}
}

/********************************************************************************************/
/*面板定时器程序																            */
/********************************************************************************************/
int CVICALLBACK SysTickCallback (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_TIMER_TICK:

			g_systime++;									   	//系统记时

///////////////////////////////////////////////////////////////////串口面板处理
			CVI_COMPanelHandle(panel);

//////////////////////////////////////////////////////////////////串口数据发送面板相关处理
			CVI_SentDataHandle(panel);

//////////////////////////////////////////////////////////////////串口数据发送面板相关处理
			CVI_SetParaHandle(panel);

//////////////////////////////////////////////////////////////////串口数据发送面板相关处理
			CVI_DataBaseHandle(panel);

			break;
	}
	return 0;
}


//保存面板属性
#define	MNAI_PANEL_FILE	"cvi_main_myinfo.ini"
typedef	struct	_stcmianpanelpara{
	struct	_csnchead_
	{
		char	source;
		char	denst;
		char 	code;
		char	framenum;
	}stccsnchead ;

	char	sentbuf[256];
}stcmianpanelpara;

//面板参数
stcmianpanelpara	smainpanelpara;

/********************************************************************************************/
/* 向配置文件写入数据					                							    	*/
/********************************************************************************************/
char	WriteMainPanelData_File(void)
{
	char	dirstring[512];
	char	filestring[512];
	int		filehandle;

	GetCtrlVal(gPanelHandle,PANEL_COM1SENDTEXTBOX,smainpanelpara.sentbuf);
	GetCtrlVal(gPanelHandle,PANEL_SOURCEADDR,&smainpanelpara.stccsnchead.source);
	GetCtrlVal(gPanelHandle,PANEL_DESTADDR,&smainpanelpara.stccsnchead.denst);
	GetCtrlVal(gPanelHandle,PANEL_FRAMCODE,&smainpanelpara.stccsnchead.code);
	GetCtrlVal(gPanelHandle,PANEL_FRAMNUM,&smainpanelpara.stccsnchead.framenum);

	GetProjectDir (dirstring);
	MakePathname (dirstring, MNAI_PANEL_FILE, filestring);

	filehandle = OpenFile (filestring, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_BINARY);
	SetFilePtr (filehandle, 0, 0);

	WriteFile(filehandle,(char *)&smainpanelpara,sizeof(smainpanelpara));

	CloseFile(filehandle);

	return 1;
}

/********************************************************************************************/
/* 从配置文件中读取数据					                								*/
/********************************************************************************************/
int	ReadMainPanelData_File(void)
{
	char	dirstring[512];
	char	filestring[512];
	int		filehandle;
	int		datalen = 0;

	GetProjectDir (dirstring);
	MakePathname (dirstring, MNAI_PANEL_FILE, filestring);

	filehandle = OpenFile (filestring, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_BINARY);
	SetFilePtr (filehandle, 0, 0);
	datalen = ReadFile(filehandle,(char *)&smainpanelpara,sizeof(smainpanelpara));

	CloseFile(filehandle);

	SetCtrlVal(gPanelHandle,PANEL_COM1SENDTEXTBOX,smainpanelpara.sentbuf);

	SetCtrlVal(gPanelHandle,PANEL_SOURCEADDR,smainpanelpara.stccsnchead.source);
	SetCtrlVal(gPanelHandle,PANEL_DESTADDR,smainpanelpara.stccsnchead.denst);
	SetCtrlVal(gPanelHandle,PANEL_FRAMCODE,smainpanelpara.stccsnchead.code);
	SetCtrlVal(gPanelHandle,PANEL_FRAMNUM,smainpanelpara.stccsnchead.framenum);

	return datalen;
}

