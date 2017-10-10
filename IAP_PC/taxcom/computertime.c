#include <ansi_c.h>
#include "includes.h"  

void timetest(void )
{
    static	unsigned int	systime = 0;
	struct tm *ptr;

 
   time_t lt,t; 
	
	
	if(GetSysTime() - systime > 200 )
	{
	    lt=time(NULL);
	    ptr=localtime((time_t*)&lt);
	    //printf("current time is:%s ",asctime(ptr));
		//printf("Today's date and time: %s", ctime(&lt));  
		
	
	    //ptr = gmtime(&lt);
	    printf("The current time is: %2d:%02d:%02d.%02d \r\n",ptr->tm_hour, ptr->tm_min, ptr->tm_sec, ptr->tm_year);
		
		
		systime = GetSysTime();
	}
}
