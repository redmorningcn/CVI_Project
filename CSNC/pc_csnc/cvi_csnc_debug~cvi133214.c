/*******************************************************************************
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
#include <formatio.h>
#include "csnc_debug.h"
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "cvi_csnc_debug.h"	

/********************************************************************************************/
/* Constants										    */
/********************************************************************************************/

/********************************************************************************************/
/* Globals										    	*/
/********************************************************************************************/
//面板参数
stcmianpanelpara	smainpanelpara;
int 				gCsncDebugPanelHandle;										//装置主面板

/********************************************************************************************/
//local function
/********************************************************************************************/
int		ReadMainPanelData_File(void);
char	WriteMainPanelData_File(void);

/********************************************************************************************/
//载并显示面板。	redmoringcn 20170927						            
/********************************************************************************************/
void LoadCsncDebugPanel(void)
{
    gCsncDebugPanelHandle = LoadPanel (0, UIR_CSNCDEBUG_NAME, CSNC_DEBUG);
	/* 	Display the panel and run the UI */
	DisplayPanel (gCsncDebugPanelHandle);
	
	ReadMainPanelData_File();
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
			
			WriteMainPanelData_File();													//面板控制字
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
			
			WriteMainPanelData_File();													//面板控制字
			QuitUserInterface(0);
			break;
		}
	return 0;
}


int CVICALLBACK SetSendFlgCallback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	char	buf[512];
	switch (event)
	{
		case EVENT_COMMIT:
			
			WriteMainPanelData_File();													//面板控制字(获得控制区变量)
			
			GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_COM1SENDTEXTBOX,buf); 			//取发送区的内容

			if(smainpanelpara.sendpara.sendstyle == 1) 									//HEX
			{
				sCtrl.PC.ConnCtrl.sendlen = strtohex(buf,sCtrl.PC.wr);					//装换成HEX
			}
			else
			{
				strcpy(sCtrl.PC.wr,buf);												//string格式
				sCtrl.PC.ConnCtrl.sendlen = strlen(buf);
			}
			
			sCtrl.PC.ConnCtrl.DestAddr 		= smainpanelpara.stccsnchead.denst;			//源地址
			sCtrl.PC.ConnCtrl.sourceaddr 	= smainpanelpara.stccsnchead.source;		//目标地址
			sCtrl.PC.ConnCtrl.FrameCode		= smainpanelpara.stccsnchead.code;			
			sCtrl.PC.ConnCtrl.SendFramNum 	= smainpanelpara.stccsnchead.framenum;		
			
			sCtrl.PC.ConnCtrl.SendFlg		= 1;										//发送数据
			
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

			 ResetTextBox (panel, CSNC_DEBUG_INFOTEXTBOX, "");
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
	uint8	buf[512];

	if((times++ %200) == 1)
		WriteMainPanelData_File();				//定期刷新面板状态
	
	if(sCtrl.PC.sCom->sConfig.open == 1)	  	//串口打开
	{				
		SetCtrlAttribute (panel, CSNC_DEBUG_COM1_SEND_DATA, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, CSNC_DEBUG_OPENCOM1, ATTR_LABEL_TEXT, "关闭串口");
	}

	if(sCtrl.PC.sCom->sConfig.open == 0)	  	//串口关闭
	{
		SetCtrlAttribute (panel, CSNC_DEBUG_COM1_SEND_DATA, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, CSNC_DEBUG_OPENCOM1, ATTR_LABEL_TEXT, "打开串口");
	}
	
	if(sCtrl.PC.RxCtrl.recvflg)					//有协议数据接收，打印协议信息
	{
		sprintf(buf,"源地址 ：%02x \r\n目标地址：%02x\r\n帧序号：%02d\r\n帧命令：%02x")
		printftext(panel,CSNC_DEBUG_INFOTEXTBOX,	
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

//////////////////////////////////////////////////////////////////串口面板处理
			CVI_COMPanelHandle(panel);

			break;
	}
	return 0;
}



/********************************************************************************************/
/* 向配置文件写入数据					                							    	*/
/********************************************************************************************/
char	WriteMainPanelData_File(void)
{
	char	dirstring[512];
	char	filestring[512];
	int		filehandle;

	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_COM1SENDTEXTBOX,smainpanelpara.sentbuf);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SOURCEADDR,&smainpanelpara.stccsnchead.source);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_DESTADDR,&smainpanelpara.stccsnchead.denst);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMCODE,&smainpanelpara.stccsnchead.code);
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMNUM,&smainpanelpara.stccsnchead.framenum);

	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SENDFMT,&smainpanelpara.sendpara.sendstyle);  
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_RECVFMT,&smainpanelpara.sendpara.recvstyle);          
	GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_DELAYMS,&smainpanelpara.sendpara.autotime);          
	//GetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SENDFMT,&smainpanelpara.sendpara.sendstyle);        
	if(smainpanelpara.sendpara.autotime)		  				//自动发送标识
		smainpanelpara.sendpara.autosend = 1;
	else
		smainpanelpara.sendpara.autotime = 0;
	
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

	if(datalen)	
	{
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_COM1SENDTEXTBOX,smainpanelpara.sentbuf);

		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_SOURCEADDR,smainpanelpara.stccsnchead.source);
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_DESTADDR,smainpanelpara.stccsnchead.denst);
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMCODE,smainpanelpara.stccsnchead.code);
		SetCtrlVal(gCsncDebugPanelHandle,CSNC_DEBUG_FRAMNUM,smainpanelpara.stccsnchead.framenum);
	}
	
	return datalen;
}

