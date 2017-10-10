#include "ndpcom.h"
#include <cvirte.h>		
#include <userint.h>
#include "lpcisp.h"
#include "lpcisp.h" 
#include "includes.h" 

static int panelHandle;


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "lpcisp.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK OpenComCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int	comNum;
	switch (event)
	{
		case EVENT_COMMIT:

			GetCtrlVal (PANEL, PANEL_SELECTCOM, &comNum); 
			OpenProgramCom(comNum);
			
			break;
	}
	return 0;
}


int CVICALLBACK SysTickCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			SysTickTask();
			
			break;
	}
	return 0;
}

extern	void	CloseBinFile(void);

int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int	comNum;
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			
			CloseProgramCom();
			CloseBinFile();

			break;
	}
	return 0;
}
