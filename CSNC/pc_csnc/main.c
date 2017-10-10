#include "mian.h"

#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include "csnc_debug.h"   

#include "cvi_com1.h"  
#include "cvi_csnc_debug.h"  
#include "cvi_config.h"   

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

	DisplayPanel (gmainPanel);

	RunUserInterface ();
				
	/*	Discard the loaded panels from memory */
	DiscardPanel (gmainPanel);

	return 0;
}


int CVICALLBACK OpenDebugPanelCallback (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			LoadCsncDebugPanel();
			
			/* Schedule two thread functions */				//多线程，创建新线程函数
			cvi_InitBspComm(&sCtrl.PC);
			
			break;
	}
	return 0;
}


int CVICALLBACK mainPanelCB (int panel, int event, void *callbackData,
							int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			cvi_CloseBspComm(&sCtrl.PC);					//线程退出 			
			break;
	}
	return 0;
}
