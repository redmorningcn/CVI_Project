#include <formatio.h>
#include <ansi_c.h>
#include "ndpcom.h"
#include <utility.h>
#include <cvirte.h>		
#include <userint.h>


#include "includes.h" 

int panelHandle;

uint8	l_programflg = 0;

uint8	GetProgramFlg(void)
{
	return l_programflg;
}

void	ClearProgramFlg(void)
{
	l_programflg = 0;
}



int main (int argc, char *argv[])
{
	int	functionId;
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ndpcom.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	
	AsyncTimerStart();

	RunUserInterface ();
	DiscardPanel (panelHandle);
	
	return 0;
}

void printftexbox(uint8 *g_textboxbuf)
{
	SetCtrlVal(panelHandle,PANEL_INFOTEXTBOX,g_textboxbuf);
}

extern uint8	SetFileDealPer(void);
	
uint8	l_destaddr;
int CVICALLBACK SysTickCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	uint8	per;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			SysTickTask();

			if(GetProgramFlg())
			{
				SetCtrlAttribute (panel, PANEL_DOWNLOAD, ATTR_LABEL_TEXT, "取消下载");  
			}else
			{
				SetCtrlAttribute (panel, PANEL_DOWNLOAD, ATTR_LABEL_TEXT, "开始下载");    
			}
			
			per =	SetFileDealPer(); 	
			SetCtrlVal (panel, PANEL_PRE, per);
			
			GetCtrlVal(panel,PANEL_DESTADDR,&l_destaddr);
			break;
	}  
	
	return 0; 
}

uint8	GetDestAddr(void)
{
	return	l_destaddr;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int	comNum;
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			
			CloseProgramCom0();
			break;
	}
	return 0;
}


int CVICALLBACK OpenComCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int		comNum,boaut;
	static	char  flg = 0;
	switch (event)
	{
		case EVENT_COMMIT:
			if(flg == 0)
			{
				GetCtrlVal (PANEL, PANEL_SELECTDIP1COM, &comNum); 
				GetCtrlVal (PANEL, PANEL_DIP1COMBOUT, &boaut);   
			
				OpenProgramCom0(comNum,boaut);
			
				SetCtrlAttribute (PANEL, PANEL_SELECTDIP1COM, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_DIP1COMBOUT, ATTR_DIMMED, 1);
				SetCtrlAttribute (PANEL, PANEL_OPENDIP1COM, ATTR_LABEL_TEXT, "关闭串口");
				flg = 1;
				break;
			}
			
			if(flg == 1)
			{
				CloseProgramCom0();
				
				SetCtrlAttribute (PANEL, PANEL_SELECTDIP1COM, ATTR_DIMMED, 0);
				SetCtrlAttribute (PANEL, PANEL_DIP1COMBOUT, ATTR_DIMMED, 0);
				
				SetCtrlAttribute (PANEL, PANEL_OPENDIP1COM, ATTR_LABEL_TEXT, "打开串口"); 
				flg = 0;
				break;
			}
			
			
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


extern	void	GetHexFileDir(char * HexFileDir);
uint8		binpathstring[512];

int CVICALLBACK OpenBinFileCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
																		
		case EVENT_COMMIT:
			GetHexFileDir(binpathstring);
			SetCtrlVal(panel,PANEL_BINPAHT,binpathstring);
			break;
	}
	return 0;
}

extern	void	StartFile(void);

int CVICALLBACK SetProgramFlgCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			if(l_programflg)
			{
				l_programflg = 0;
			}
			else
			{
				l_programflg = 1;
			}
			
			StartFile();
			break;
	}
	return 0;
}

void printftext(const char *format, ...) 
{ 
   va_list  argptr;
   unsigned int   cnt;
   char     a[255];
   
   strlen(format);
   if(strlen(format) > (sizeof(a) - 55))
   {
		return	;
   }
   va_start(argptr, format);
   
   cnt = vsprintf(a, format, argptr);
   
   va_end(argptr);
   
   SetCtrlVal(PANEL,PANEL_INFOTEXTBOX,a);
}
