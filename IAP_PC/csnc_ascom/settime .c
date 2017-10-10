#include <ansi_c.h>
#include "includes.h"  


void SetRTCTime(void)
{
	stcTime		sTime;   
	
	 ReadTime((stcTime *)&sTime);											// Ê±¼ä   
	 
//	 Sendrs485((uint8 *)&sTime,sizeof(sTime));
}
