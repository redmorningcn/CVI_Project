#include <utility.h>
#include <rs232.h>
#include <userint.h>
#include <ansi_c.h>
#include "includes.h"


int			l_programCom3Num = 0; 
unsigned	int	l_recCom3Time = 0;  
char		l_recCom3EndFlg = 0;
char		l_Com3StartFlg = 0;

stcFIFO	  	sCom3FIFO;
char		Com3Buf[256];

unsigned char GetOpenCom3Flg(void)
{
	return	l_programCom3Num;
}

void	HoldCom3Tast(void)
{
	if((GetSysTime()-l_recCom3Time>2)&& l_Com3StartFlg)
	{
		l_recCom3EndFlg = 1;
		l_Com3StartFlg = 0;
	}
}

unsigned char GetRecCom3EndFlg(void)
{
	HoldCom3Tast();
	return	l_recCom3EndFlg;
}

void	ClearRecCom3EndFlg(void)
{
	l_recCom3EndFlg = 0;
	l_Com3StartFlg = 0;	
}
//中断处理函数
void	RecCom3Callback(int portNo, int eventMask, void *callbackData)
{
	int		strLen;
	char	buf[256];
	
	strLen = GetInQLen(l_programCom3Num);
	strLen = strLen>sizeof(buf)?sizeof(buf):strLen;
	ComRd (l_programCom3Num, buf, strLen);

	if(strLen)
	{
		l_recCom3Time = GetSysTime();
		l_Com3StartFlg = 1;
	}
		
	WriteFIFO((stcFIFO *)&sCom3FIFO,buf,strLen);
}


int CVICALLBACK RefreshRecCom3Data (void *functionData)
{
	 static	int	tmp;
	 //RecCom3Callback(0,0,&tmp);
	 while(1)
	 {
	  	RecCom3Callback(0,0,&tmp);
	 }
	 
	 return 0;
}


int	ReadCom3(unsigned char *Buf,int Len)
{
   	int	NoReadSize;
	int	readLen = 0;
	
	NoReadSize = GetNoReadSizeFIFO((stcFIFO *)&sCom3FIFO);
	
	if(NoReadSize)
	{
		readLen = NoReadSize>Len?Len:NoReadSize;

		ReadFIFO((stcFIFO *)&sCom3FIFO,Buf,readLen);	
	}
	
	return	readLen;
}

void OpenProgramCom3(int	comNum,int boaut)
{
	int	statue;
	int	functionId;
	l_programCom3Num = comNum;
	
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

	InitFIFO((stcFIFO *)&sCom3FIFO,Com3Buf,sizeof(Com3Buf));  
	
//	InstallCom3Callback (l_programCom3Num, LWRS_RXCHAR, 0, 0, RecCom3Callback, 0);
	CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,RefreshRecCom3Data, NULL, &functionId);    
}

void CloseProgramCom3(void)
{
	if(l_programCom3Num)
	{
		CloseCom (l_programCom3Num) ;
	}
}

int	SendCom3(unsigned char *Buf,int Len)
{
	ComWrt(l_programCom3Num,Buf,Len) ; 
	
	return 1;
}

void printfcom3(const char *format, ...) 
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
   
   SendCom3(a,strlen(a));
}


