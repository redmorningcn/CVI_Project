#include <ansi_c.h>
#include <userint.h>
#include	"includes.h"


int	gSimTastCode = 0;      
void	GetPhoneNum(char *phoneNum)
{
	memcpy(phoneNum,gphoneNum,strlen(gphoneNum));
}

void	GetMessage(char *messageBuf)
{
	memcpy(messageBuf,gMessageBuf,strlen(gMessageBuf));
	messageBuf[strlen(gMessageBuf)] = 0;
}



void	CallTelephone(char *phoneNum)
{
	char	string[24];
	
	DelayX10ms(50);   
	printfcom("ATE1\r\n");
	DelayX10ms(50);
	printfcom("AT+COLP=1\r\n");
	DelayX10ms(50);     
	sprintf(string,"ATD%s;\r\n",phoneNum);
	printfcom(string);
	DelayX10ms(50);
	
}

void	SendMessage(char *phoneNum,char *MessageBuf)
{
	char	string[24];
	char	messageEnd = 0x1a;
	
	DelayX10ms(50);   
	printfcom("AT+CNMI=2,1\r\n");
	DelayX10ms(50);
	printfcom("AT+CMGF=1\r\n");
	DelayX10ms(50);     
	sprintf(string,"AT+CMGS=\"%s\"\r\n",phoneNum);
	printfcom(string);
	DelayX10ms(50);
	printfcom(MessageBuf);
	DelayX10ms(50);   
	SendCom3(&messageEnd,sizeof(messageEnd));
	DelayX10ms(50);   		
}

int	ReadMessage(char *MessageBuf,int messageNum)
{
	char	string[24];   
	int		strLen;
	char	*p = 0;
	
	DelayX10ms(50);   
	printfcom("AT+CNMI=2,1\r\n");
	DelayX10ms(50);
	printfcom("AT+CMGF=1\r\n");
	DelayX10ms(50);   
	strLen = ReadCom3(MessageBuf,256);  //清接收     	
	sprintf(string,"AT+CMGR=%d\r\n",messageNum);
	printfcom(string);
	DelayX10ms(50);
	
	strLen = ReadCom3(MessageBuf,256);
	
	if(strLen)
	{
		p = strstr(MessageBuf,"+CMGR");
		if(p)
		{
			return 1;
		}
	}
	DelayX10ms(50);   	
	
	return 0;
}

typedef	struct	_stcMessageInfo
{
	char	phoneNum[24];
	char	info[256];
	char	date[24];
}stcMessageInfo;


//从“”中选择出字符串
int	GetStringFrom2YH(char *srcString,char *denString)
{
	char	*p,*p1;
	p = 
	
}

void	Getmessage(stcMessageInfo *sMessageInfo,int messageNum)
{
	char	*p,*p1;
	if(ReadMessage((char *)sMessageInfo,messageNum))	
	{
		p = MessageBuf;
		printf(p);
		p1 = strstr(p,"+CMGR:");
		p = p1;
		
		p1 = strstr(p+1,"\"");
		p = p1;
		p1 = strstr(p+1,"\""); 
		*(p1++) = 0;
		printf(p+1);
		
		p = p1
		p1 = strstr(p+1,"\"");  	
		
		//p1 = strstr
	}
}

void SimTast(void)
{
	char	phoneNum[16];   
	char	messageBuf[256];   	
	
	GetPhoneNum(phoneNum); 
	
	if(GetOpenComFlg() == 0)
	{
		//MessagePopup("错误消息","请打开串口！");
		return;
	}
	
	switch(gSimTastCode)
	{
		case CALL_PHONE:	CallTelephone(phoneNum); 
							break;
		case SEND_MESSAGE:  GetMessage(messageBuf);   
							SendMessage(phoneNum,messageBuf); 
							break;   
		case REC_MESSAGE:	Getmessage(messageBuf,1); 
							break;							
							
		default:	;
	}
	
	gSimTastCode = 0;
	
}

