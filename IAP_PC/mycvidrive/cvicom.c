#include <utility.h>
#include <rs232.h>
#include <userint.h>
#include <ansi_c.h>
#include "includes.h"


int			l_programCom0Num = 0; 
unsigned	int	l_recCom0Time = 0;  
char		l_recCom0EndFlg = 0;
char		l_Com0StartFlg = 0;

stcFIFO	  	sCom0FIFO;
char		Com0Buf[256];

unsigned char GetOpenCom0Flg(void)
{
	return	l_programCom0Num;
}

void	HoldCom0Tast(void)
{
	if((GetSysTime()-l_recCom0Time>2)&& l_Com0StartFlg)
	{
		l_recCom0EndFlg = 1;
		l_Com0StartFlg = 0;
	}
}

unsigned char GetRecCom0EndFlg(void)
{
	HoldCom0Tast();
	return	l_recCom0EndFlg;
}

void	ClearRecCom0EndFlg(void)
{
	l_recCom0EndFlg = 0;
	l_Com0StartFlg = 0;	
}

extern	uint8	GetCom0OpenFlg(void);
//中断处理函数
void	RecCom0Callback(int portNo, int eventMask, void *callbackData)
{
	int		strLen;
	char	buf[256];
	
	if(GetCom0OpenFlg())
	{
		strLen = GetInQLen(l_programCom0Num);
		strLen = strLen>sizeof(buf)?sizeof(buf):strLen;
		ComRd (l_programCom0Num, buf, strLen);

		if(strLen)
		{
			l_recCom0Time = GetSysTime();
			l_Com0StartFlg = 1;
		}
		
		WriteFIFO((stcFIFO *)&sCom0FIFO,buf,strLen);
	}
}


int CVICALLBACK RefreshRecCom0Data (void *functionData)
{
	 static	int	tmp;
	 //RecCom0Callback(0,0,&tmp);
	 while(1)
	 {
	  	RecCom0Callback(0,0,&tmp);
	 }
	 
	 return 0;
}


int	ReadCom0(unsigned char *Buf,int Len)
{
   	int	NoReadSize;
	int	readLen = 0;
	
	NoReadSize = GetNoReadSizeFIFO((stcFIFO *)&sCom0FIFO);
	
	if(NoReadSize)
	{
		readLen = NoReadSize>Len?Len:NoReadSize;

		ReadFIFO((stcFIFO *)&sCom0FIFO,Buf,readLen);	
	}
	
	return	readLen;
}

int	com0_functionId; 
uint8	l_com0openflg = 0;
void OpenProgramCom0(int	comNum,int boaut)
{
	int	statue;
	
	l_programCom0Num = comNum;
	
	statue = OpenComConfig (comNum, "", boaut, 0, 8, 1, 512, 512); 
	
	if(statue > 0 ||statue == 0)
	{
		SetCTSMode (comNum, LWRS_HWHANDSHAKE_OFF);
		FlushInQ (comNum) ;
		FlushOutQ (comNum) ;
	      
		
		InitFIFO((stcFIFO *)&sCom0FIFO,Com0Buf,sizeof(Com0Buf));  
	
	//	InstallCom0Callback (l_programCom0Num, LWRS_RXCHAR, 0, 0, RecCom0Callback, 0);
		CmtScheduleThreadPoolFunction (DEFAULT_THREAD_POOL_HANDLE,RefreshRecCom0Data, NULL, &com0_functionId);  
		
		l_com0openflg = 1; 
	}
	else
	{
		MessagePopup("消息提示","串口不存在或被占用！"); 
	}

	
}

uint8	GetCom0OpenFlg(void)
{
	return	l_com0openflg;
}


void CloseProgramCom0(void)
{   
	int	tmp;
	if(l_programCom0Num)
	{
		l_com0openflg = 0;   
		DelayX10ms(5);
		CloseCom (l_programCom0Num) ;
		CmtReleaseThreadPoolFunctionID(DEFAULT_THREAD_POOL_HANDLE,com0_functionId);
		//CmtGetThreadPoolFunctionAttribute (, , ATTR_TP_FUNCTION_RETURN_VALUE, ); 
		//CmtExitThreadPoolThread (0);
		
	}
}

int	SendCom0(unsigned char *Buf,int Len)
{
//	int		i;
//	printf("\r\n send %d\r\n",Len);
//	for(i = 0;i<Len;i++)
//		printf("%02x",Buf[i]);
//	
//	printf("\r\n");
	
	ComWrt(l_programCom0Num,Buf,Len) ; 
	
	return 1;
}

void printfcom(const char *format, ...) 
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
   
   SendCom0(a,strlen(a));
}

//extern	void printftexbox(uint8 *g_textboxbuf)   ;
//void printftextbox(const char *format, ...) 
//{ 
//   va_list  argptr;
//   unsigned int   cnt;
//   char     a[255];
//   
//   strlen(format);
//   if(strlen(format) > (sizeof(a) - 55))
//   {
//		return	;
//   }
//   va_start(argptr, format);
//   
//   cnt = vsprintf(a, format, argptr);
//   
//   va_end(argptr);
//   
//   printftexbox(a);
//}






