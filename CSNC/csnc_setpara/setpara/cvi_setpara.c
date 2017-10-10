/*******************************************************************************
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Description:    
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
#include <ansi_c.h>
#include <cvi_setpara.h>

#include "setpara.h"
#include "cvi_config.h"

/********************************************************************************************/
/* Constants																				*/
/********************************************************************************************/

/********************************************************************************************/
//  Globals																					
/********************************************************************************************/
//面板变量
int 			gsetpara_panelHandle;

/********************************************************************************************/
//local																					
/********************************************************************************************/
extern	void	WriteSetParaToFile(void);
extern	int		ReadSetPararomFile(void);


/********************************************************************************************/
//载并显示面板。	redmoringcn 20170930						            
/********************************************************************************************/
void LoadCsncSetparaPanel(void)
{
    gsetpara_panelHandle = LoadPanel (0, UIR_SETPARA_NAME, SETP_PANEL);
	/* 	Display the panel and run the UI */
	DisplayPanel (gsetpara_panelHandle);
	
	ReadSetPararomFile();
}

/********************************************************************************************/
/*打开参数设置面板																	        */
/********************************************************************************************/
int CVICALLBACK OpenSetParaPanelCallback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			LoadCsncSetparaPanel();
			
			/* Schedule two thread functions */				//多线程，创建新线程函数
			cvi_InitBspComm(&sCtrl.PC);						//串口
			
			ReadSetPararomFile();													//调出以前存储的参数
			
			//HidePanel(gPanelHandle);												//隐藏主面板
			break;
	}
	return 0;
}

/********************************************************************************************/
/*在面板上显示时间																	        */
/********************************************************************************************/
void	DisplayTimeOnSetPanel(void)
{
	struct tm *ptr;
    time_t lt; 
	char  databuf[64];  
	
    lt=time(NULL);
    ptr=localtime((time_t*)&lt);
	
	sprintf(databuf,"%02d-%02d-%02d %02d:%02d:%02d",ptr->tm_year-100,
		ptr->tm_mon+1,ptr->tm_mday,ptr->tm_hour,ptr->tm_min,ptr->tm_sec);
		
	if(gsetpara_panelHandle)
		SetCtrlVal (gsetpara_panelHandle, SETP_PANEL_DATEANDTIME, databuf);  
}

/********************************************************************************************/
/*退出面板      																	        */
/********************************************************************************************/
void	QuitSetParaPanel(void)
{
	WriteSetParaToFile();			//退出，保存参数
	
	HidePanel (gsetpara_panelHandle);
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int	comNum;
	switch (event)
	{
		case EVENT_COMMIT:
			QuitSetParaPanel();			//退出面板操作
			break;
	}
	return 0;
}

int CVICALLBACK SetLocoCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			l_eqiupmentcode = FIX_CARD;
			
			GetCtrlVal(panel,SETP_PANEL_LOCOTYPE,&gstrProductInfo.sLocoId.Type);
			GetCtrlVal(panel,SETP_PANEL_LOCONUM,&gstrProductInfo.sLocoId.Num);
			
			if(gstrProductInfo.sLocoId.Num == 0|| gstrProductInfo.sLocoId.Type==0)
			{
				MessagePopup ("ErrMessage", "信息内容错误，请重新输入");
			}
			break;
	}
	return 0;
}

int CVICALLBACK SetTimeCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		   	l_eqiupmentcode = TIME_CARD; 
			break;
	}
	return 0;
}



int CVICALLBACK SetHigCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		   	l_eqiupmentcode = HIGHT_CARD; 
			GetCtrlVal(panel,SETP_PANEL_HIG,&gsOilPara.Hig);
			break;
	}
	return 0;
}

int CVICALLBACK SetSelCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		   	l_eqiupmentcode = MODEL_SELECT_CARD; 
			GetCtrlVal(panel,SETP_PANEL_SEL,&gsOilPara.ModelNum);
			break;
	}
	return 0;
}

int CVICALLBACK SetReadDataCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			l_eqiupmentcode = DATA_CARD_DIS; 
			break;
	}
	return 0;
}

int CVICALLBACK SetReadDataFinalCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			l_eqiupmentcode = DATA_CARD_FIN; 
			break;
	}
	return 0;
}

int CVICALLBACK ClearAllCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			l_eqiupmentcode = CLEAR_ALL;  
			break;
	}
	return 0;
}


int CVICALLBACK RstSysCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			l_eqiupmentcode = RST_SYS;  
			break;
	}
	return 0;
}


int CVICALLBACK SetCalcParaCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			l_eqiupmentcode = CALC_PARA; 
			
			GetCtrlVal(panel,SETP_PANEL_CALCPARA,(uint32 *)&gsOilPara.Slope);
			GetCtrlVal(panel,SETP_PANEL_HIG,&gsOilPara.Hig);
			break;
	}
	return 0;
}

int CVICALLBACK CardStaticIndicateCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	return 0;
}

/********************************************************************************************/
/* 给控制面板赋值
/********************************************************************************************/
void	SetSetParaPanelVal(void)
{
	SetCtrlVal(gsetpara_panelHandle,SETP_PANEL_LOCOTYPE,gstrProductInfo.sLocoId.Type);
	SetCtrlVal(gsetpara_panelHandle,SETP_PANEL_LOCONUM,gstrProductInfo.sLocoId.Num);	
	
//	SetCtrlVal(gsetpara_panelHandle,SETP_PANEL_DEN,gsOilPara.Density);
	SetCtrlVal(gsetpara_panelHandle,SETP_PANEL_HIG,gsOilPara.Hig);
	SetCtrlVal(gsetpara_panelHandle,SETP_PANEL_SEL,gsOilPara.ModelNum);
	SetCtrlVal(gsetpara_panelHandle,SETP_PANEL_CALCPARA,gsOilPara.Slope);	
}

/********************************************************************************************/
/* 取控制面板赋值
/********************************************************************************************/
void	GetSetParaPanelVal(void)
{
	GetCtrlVal(gsetpara_panelHandle,SETP_PANEL_LOCOTYPE,&gstrProductInfo.sLocoId.Type);
	GetCtrlVal(gsetpara_panelHandle,SETP_PANEL_LOCONUM,&gstrProductInfo.sLocoId.Num);	
	
//	GetCtrlVal(gsetpara_panelHandle,SETP_PANEL_DEN,&gsOilPara.Density);
	GetCtrlVal(gsetpara_panelHandle,SETP_PANEL_HIG,&gsOilPara.Hig);
	GetCtrlVal(gsetpara_panelHandle,SETP_PANEL_SEL,&gsOilPara.ModelNum);
	GetCtrlVal(gsetpara_panelHandle,SETP_PANEL_CALCPARA,&gsOilPara.Slope);	
}


/********************************************************************************************/
/* ReadSetParaConfigFromFile  		从配置文件中读出串口的配置文件，如果有则按配置文件配置；
否则，显示默认值	*/
/********************************************************************************************/
int	ReadSetPararomFile(void)
{
	char 	*fileName;									//文件指针
	char 	projectDir[MAX_PATHNAME_LEN];				//工程路径
	char 	fullPath[MAX_PATHNAME_LEN];					//文件全路径
	ssize_t size;										//文件大小
	int		filecom1hand;								//文件句柄
	int		readsize;
	int		flg = 0;
	
	fileName = SETPRAR_FILE_NAME;
	
	GetProjectDir (projectDir) ;					//取项目路径
//
	MakePathname (projectDir, fileName, fullPath);	//文件路劲 where fullPath has the following format: c:\myproject\myfile.dat
	
	if(GetFileInfo (fullPath, &size) == 0)			//文件不存在
	{
		return 0;
	}else{
		filecom1hand = OpenFile (fullPath, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_BINARY);	//打开文件
		SetFilePtr (filecom1hand, 0, 0);												//文件从头开始
		
		ReadFile (filecom1hand, (char *)&gsOilPara, sizeof(gsOilPara));					//配置文件读取
		ReadFile (filecom1hand, (char *)&gstrProductInfo, sizeof(gstrProductInfo));		//配置文件读取 
		
		SetSetParaPanelVal();						//设置参数面板值
	}
	
	CloseFile(filecom1hand);						//关闭文件
	
	return	flg;
}


/********************************************************************************************/
/* WriteCom1ConfigToFile  		写配置数据到文件
/********************************************************************************************/
void	WriteSetParaToFile(void)
{
	char 	*fileName;									//文件指针
	char 	projectDir[MAX_PATHNAME_LEN];				//工程路径
	char 	fullPath[MAX_PATHNAME_LEN];					//文件全路径
	int		filecom1hand;								//文件句柄
	
	fileName = SETPRAR_FILE_NAME;
	
	GetProjectDir (projectDir) ;					//取项目路径

	MakePathname (projectDir, fileName, fullPath);	//文件路劲 where fullPath has the following format: c:\myproject\myfile.dat
	
	filecom1hand = OpenFile (fullPath, VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_BINARY);		//打开文件
		
	SetFilePtr (filecom1hand, 0, 0);													//文件从头开始

	GetSetParaPanelVal();																//取面板值
	
	WriteFile (filecom1hand, (char *)&gsOilPara, sizeof(gsOilPara));					//存运算参数
	WriteFile (filecom1hand, (char *)&gstrProductInfo, sizeof(gstrProductInfo));		//存产品信息			
	
	CloseFile(filecom1hand);															//关闭文件     				
}

int CVICALLBACK SetParaPanelCB (int panel, int event, void *callbackData,
								int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
				QuitSetParaPanel();				//退出面板操作
			break;
	}
	return 0;
}


int CVICALLBACK SetStoreRecordRecCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int	tmp;
	switch (event)
	{
		case EVENT_COMMIT:
			l_eqiupmentcode = SYS_RUN_PARA;
			
			GetCtrlVal(gsetpara_panelHandle,SETP_PANEL_STORE_STYPE,&tmp);
			
			gsRunPara.StoreType 	= 0x00;
			if(tmp == 0)
				gsRunPara.StoreType 	= 0x02;
			
			gsRunPara.StoreTypeBak	= gsRunPara.StoreType;
			gsRunPara.Setbitflg		= 0x01 << ((char *)&gsRunPara.StoreType - (char *)&gsRunPara);		//设置有效位				  
			break;
	}
	return 0;
}

int CVICALLBACK SetParaTimerCallback (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			DisplayTimeOnSetPanel();	  	//面板显示
			
			Com_SetParaTask();				//串口参数设置
			
			break;
	}
	return 0;
}
