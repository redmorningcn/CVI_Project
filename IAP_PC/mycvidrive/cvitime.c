#include <userint.h>
#include <asynctmr.h>
#include <ansi_c.h>
#include "includes.h"


unsigned int	l_systime = 0;



int CVICALLBACK AsyncTimerCallback(int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{
	l_systime++;
	
	HoldLed(); 
	return 1;
}


unsigned int GetSysTime(void)
{
	return	l_systime;
}

void	DelayX10ms(unsigned int Time)
{
	unsigned int tmp;
	
	tmp	= GetSysTime();
	
	while(GetSysTime() - tmp < Time)
	{
	//	;
	}

}

void AsyncTimerStart(void)
{
	NewAsyncTimer (0.01, -1, 1, AsyncTimerCallback, 0);
}

#ifdef	CVI_TCP_SERVER
void	SysRunLed(void)
{
	OnLed(Led5Run,1000,25,12);
}
#endif




extern	void	IAP_ProgramTask(void);    

void	SysTickTask(void)
{
	IAP_ProgramTask();	
	
	//DelayX10ms(10);
}
