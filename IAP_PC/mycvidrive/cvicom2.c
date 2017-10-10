#include <utility.h>
#include <rs232.h>
#include <userint.h>
#include <ansi_c.h>
#include "includes.h"


int			l_programCom2Num = 0; 
unsigned	int	l_recCom2Time = 0;  
char		l_recCom2EndFlg = 0;
char		l_Com2StartFlg = 0;

stcFIFO	  	sCom2FIFO;
char		Com2Buf[256];

unsigned char GetOpenCom2Flg(void)
{
	return	l_programCom2Num;
}

void	HoldCom2Tast(void)
{
	if((GetSysTime()-l_recCom2Time>2)&& l_Com2StartFlg)
	{
		l_recCom2EndFlg = 1;
		l_Com2StartFlg = 0;
	}
}

unsigned char GetRecCom2EndFlg(void)
{
	HoldCom2Tast();
	return	l_recCom2EndFlg;
}

void	ClearRecCom2EndFlg(void)
{
	l_recCom2EndFlg = 0;
	l_Com2StartFlg = 0;	
}
//中断处理函数
void	RecCom2Callback(int portNo, int eventMask, void *callbackData)
{
	int		strLen;
	char	buf[256];
	
	strLen = GetInQLen(l_programCom2Num);
	strLen = strLen>sizeof(buf)?sizeof(buf):strLen;
	ComRd (l_programCom2Num, buf, strLen);

	if(strLen)
	{
		l_recCom2Time = GetSysTime();
		l_Com2StartFlg = 1;
	}
		
	WriteFIFO((stcFIFO *)&sCom2FIFO,buf,strLen);
}


int CVICALLBACK RefreshRecCom2Data (void *functionData)
{
	 static	int	tmp;
	 //RecCom2Callback(0,0,&tmp);
	 while(1)
	 {
	  	RecCom2Callback(0,0,&tmp);
	 }
	 
	 return 0;
}


int	ReadCom2(unsigned char *Buf,int Len)
{
   	int	NoReadSize;
	int	readLen = 0;
	
	NoReadSize = GetNoReadSizeFIFO((stcFIFO *)&sCom2FIFO);
	
	if(NoReadSize)
	{
		readLen = NoReadSize>Len?Len:NoReadSize;

		ReadFIFO((stcFIFO *)&sCom2FIFO,Buf,readLen);	
	}
	
	return	readLen;
}

void OpenProgramCom2(int	comNum,int boaut)
{
	int	statue;
	int	functionId;
	l_programCom2Num = comNum;
	
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

	InitFIFO((stcFIFO *)&sCom2FIFO,Com2Buf,sizeof(Com2Buf));  
	
//	InstallCom2Callback (l_programCom2Num, LWRS_RXCHAR, 0, 0, RecCom2Callback, 0);
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,RefreshRecCom2Data, NULL, &functionId);    
}

void CloseProgramCom2(void)
{
	if(l_programCom2Num)
	{
		CloseCom (l_programCom2Num) ;
	}
}

int	SendCom2(unsigned char *Buf,int Len)
{
	ComWrt(l_programCom2Num,Buf,Len) ; 
	
	return 1;
}

void printfcom2(const char *format, ...) 
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
   
   SendCom2(a,strlen(a));
}


