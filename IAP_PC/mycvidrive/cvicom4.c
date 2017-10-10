#include <utility.h>
#include <rs232.h>
#include <userint.h>
#include <ansi_c.h>
#include "includes.h"


int			l_programCom4Num = 0; 
unsigned	int	l_recCom4Time = 0;  
char		l_recCom4EndFlg = 0;
char		l_Com4StartFlg = 0;
char		l_Com4BufFullFlg = 0;

stcFIFO	  	sCom4FIFO;
char		Com4Buf[256];

unsigned char GetOpenCom4Flg(void)
{
	return	l_programCom4Num;
}

void	HoldCom4Tast(void)
{
	if(((GetSysTime()-l_recCom4Time>2)&& l_Com4StartFlg)  || l_Com4BufFullFlg == 1)
	{
		l_recCom4EndFlg = 1;
		l_Com4StartFlg = 0;
		l_Com4BufFullFlg = 0;
	}

}

unsigned char GetRecCom4EndFlg(void)
{
	HoldCom4Tast();
//	printf("\r\n test %d,%d,%d",l_recCom4EndFlg,l_Com4StartFlg,l_programCom4Num); 
	return	l_recCom4EndFlg;
}

void	ClearRecCom4EndFlg(void)
{
	l_recCom4EndFlg = 0;
	l_Com4StartFlg = 0;	
}
//中断处理函数
void	RecCom4Callback(int portNo, int eventMask, void *callbackData)
{
	int		strLen;
	char	buf[256];
	
	strLen = GetInQLen(l_programCom4Num);
	strLen = strLen>sizeof(buf)?sizeof(buf):strLen;
	ComRd (l_programCom4Num, buf, strLen);

	
	if(strLen)
	{
		l_recCom4Time = GetSysTime();
		l_Com4StartFlg = 1;
	}
		
	if(WriteFIFO((stcFIFO *)&sCom4FIFO,buf,strLen)==0)
	{
		l_Com4BufFullFlg = 1;	
	}
}


int CVICALLBACK RefreshRecCom4Data (void *functionData)
{
	 static	int	tmp;
	 //RecCom4Callback(0,0,&tmp);
	 while(1)
	 {
	  	RecCom4Callback(0,0,&tmp);
	 }
	 
	 return 0;
}


int	ReadCom4(unsigned char *Buf,int Len)
{
   	int	NoReadSize;
	int	readLen = 0;
	
	NoReadSize = GetNoReadSizeFIFO((stcFIFO *)&sCom4FIFO);
	
	if(NoReadSize)
	{
		readLen = NoReadSize>Len?Len:NoReadSize;

		ReadFIFO((stcFIFO *)&sCom4FIFO,Buf,readLen);	
	}
	
	return	readLen;
}

void OpenProgramCom4(int	comNum,int boaut)
{
	int	statue;
	int	functionId;
	l_programCom4Num = comNum;
	
	statue = OpenComConfig (comNum, "", boaut, 0, 8, 1, 512, 512); 
	
	if(statue > 0 ||statue == 0)
	{
		SetCTSMode (comNum, LWRS_HWHANDSHAKE_OFF);
		FlushInQ (comNum) ;
		FlushOutQ (comNum) ;
	}
	else
	{
		MessagePopup("消息提示","串口不存在或被占用！"); 
	}

	InitFIFO((stcFIFO *)&sCom4FIFO,Com4Buf,sizeof(Com4Buf));  
	
//	InstallCom4Callback (l_programCom4Num, LWRS_RXCHAR, 0, 0, RecCom4Callback, 0);
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,RefreshRecCom4Data, NULL, &functionId);    
}

void CloseProgramCom4(void)
{
	if(l_programCom4Num)
	{
		CloseCom (l_programCom4Num) ;
	}
}

int	SendCom4(unsigned char *Buf,int Len)
{
	ComWrt(l_programCom4Num,Buf,Len) ; 
	
	return 1;
}

void printfcom4(const char *format, ...) 
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
   
   SendCom4(a,strlen(a));
}


