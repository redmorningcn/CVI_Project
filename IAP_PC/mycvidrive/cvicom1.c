#include <utility.h>
#include <rs232.h>
#include <userint.h>
#include <ansi_c.h>
#include "includes.h"


int			l_programCom1Num = 0; 
unsigned	int	l_recCom1Time = 0;  
char		l_recCom1EndFlg = 0;
char		l_Com1StartFlg = 0;

stcFIFO	  	sCom1FIFO;
char		Com1Buf[256];

unsigned char GetOpenCom1Flg(void)
{
	return	l_programCom1Num;
}

void	HoldCom1Tast(void)
{
	if((GetSysTime()-l_recCom1Time>2)&& l_Com1StartFlg)
	{
		l_recCom1EndFlg = 1;
		l_Com1StartFlg = 0;
	}
}

unsigned char GetRecCom1EndFlg(void)
{
	HoldCom1Tast();
	return	l_recCom1EndFlg;
}

void	ClearRecCom1EndFlg(void)
{
	l_recCom1EndFlg = 0;
	l_Com1StartFlg = 0;	
}
//中断处理函数
void	RecCom1Callback(int portNo, int eventMask, void *callbackData)
{
	int		strLen;
	char	buf[256];
	
	strLen = GetInQLen(l_programCom1Num);
	strLen = strLen>sizeof(buf)?sizeof(buf):strLen;
	ComRd (l_programCom1Num, buf, strLen);

	if(strLen)
	{
		l_recCom1Time = GetSysTime();
		l_Com1StartFlg = 1;
	}
		
	WriteFIFO((stcFIFO *)&sCom1FIFO,buf,strLen);
}


int CVICALLBACK RefreshRecCom1Data (void *functionData)
{
	 static	int	tmp;
	 //RecCom1Callback(0,0,&tmp);
	 while(1)
	 {
	  	RecCom1Callback(0,0,&tmp);
	 }
	 
	 return 0;
}


int	ReadCom1(unsigned char *Buf,int Len)
{
   	int	NoReadSize;
	int	readLen = 0;
	
	NoReadSize = GetNoReadSizeFIFO((stcFIFO *)&sCom1FIFO);
	
	if(NoReadSize)
	{
		readLen = NoReadSize>Len?Len:NoReadSize;

		ReadFIFO((stcFIFO *)&sCom1FIFO,Buf,readLen);	
	}
	
	return	readLen;
}

void OpenProgramCom1(int	comNum,int boaut)
{
	int	statue;
	int	functionId;
	l_programCom1Num = comNum;
	
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

	InitFIFO((stcFIFO *)&sCom1FIFO,Com1Buf,sizeof(Com1Buf));  
	
//	InstallCom1Callback (l_programCom1Num, LWRS_RXCHAR, 0, 0, RecCom1Callback, 0);
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,RefreshRecCom1Data, NULL, &functionId);    
}

void CloseProgramCom1(void)
{
	if(l_programCom1Num)
	{
		CloseCom (l_programCom1Num) ;
	}
}

int	SendCom1(unsigned char *Buf,int Len)
{
	ComWrt(l_programCom1Num,Buf,Len) ; 
	
	return 1;
}

void printfcom1(const char *format, ...) 
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
   
   SendCom1(a,strlen(a));
}


