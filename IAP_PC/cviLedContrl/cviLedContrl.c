#include <ansi_c.h>
#include "includes.h"

void LedTast(void)
{
	uint8	led1flg,led2flg,led3flg,led4flg;
	
	if(GetSysTime() % 1 ==0)
	{
		led1flg = GetDip1DataConFlg();
		if(led1flg)
		{
			OnLed(Led0Dip1,1000,20,10); 
		}
		else
		{
			OffLed(Led0Dip1);
		}
	
		led2flg = GetDip2DataConFlg();
		if(led2flg)
		{
			OnLed(Led1Dip2,1000,20,10); 
		}
		else
		{
			OffLed(Led1Dip2);
		}
		
		led3flg = GetPowerDataConFlg();
		if(led3flg)
		{
			OnLed(Led3Exp1,1000,20,10); 
		}
		else
		{
			OffLed(Led3Exp1);
		}
	
		led4flg = GetSpeedDataConFlg();
	
		if(led4flg)
		{	 
			OnLed(Led4Exp2,1000,20,10); 
		}
		else
		{
			OffLed(Led4Exp2);
		}
	
	}
}
