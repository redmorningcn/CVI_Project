#include "f:\cvi\project\CSNC\csnc_setpara\mian.h"

#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "cvi_com1.h"  
#include "cvi_config.h"   
#include "cvi_setpara.h"   
#include "setpara.h"   

/********************************************************************************************/
/* Constants										    */
/********************************************************************************************/
//uir文件名
#define		UIR_MAIN_NAME			"csnc_debug.uir"

/********************************************************************************************/
/* Globals										    */
/********************************************************************************************/
//面板变量
stcSysCtrl	sCtrl;		
int			gmainPanel  = 0;						//全局面板变量

/********************************************************************************************/
/* Application entry point.																	*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)				/* Initialize CVI libraries */
		return -1;	/* out of memory */
	
	gmainPanel = LoadPanel (0, "mian.uir", MAINPANEL); //装载主面板

	cvi_InitBspComm(&sCtrl.PC);

	DisplayPanel (gmainPanel);

	RunUserInterface ();
				
	/*	Discard the loaded panels from memory */
	DiscardPanel (gmainPanel);

	return 0;
}


int CVICALLBACK mainPanelCB (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			
			cvi_CloseBspComm(&sCtrl.PC);					//线程退出 
			QuitUserInterface(0);
			break;
	}
	return 0;
}


/********************************************************************************************/
/*面板及周期性事件							            */
/********************************************************************************************/
void CVI_MainPanelHandle(int panel)
{
	//static	int	times = 0;

/////////////////////////////////////////////////刷新配置信息
	if(sCtrl.PC.sCom != NULL && sCtrl.PC.sCom->sConfig.open == 1)	  	//串口打开
	{				
		SetCtrlAttribute (panel, MAINPANEL_OPEN_SETPARA, ATTR_DIMMED, 0);
		SetCtrlAttribute (panel, MAINPANEL_OPEN_COM, ATTR_LABEL_TEXT, "关闭串口");
	}

	if(sCtrl.PC.sCom != NULL && sCtrl.PC.sCom->sConfig.open == 0)	  	//串口关闭
	{
		SetCtrlAttribute (panel, MAINPANEL_OPEN_SETPARA, ATTR_DIMMED, 1);
		SetCtrlAttribute (panel, MAINPANEL_OPEN_COM, ATTR_LABEL_TEXT, "打开串口");
	}
}

int CVICALLBACK MainTimerCallback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:

			CVI_MainPanelHandle(panel);
			break;
	}
	return 0;
}
