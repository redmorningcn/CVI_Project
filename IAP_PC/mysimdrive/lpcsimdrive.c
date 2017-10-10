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


//从“”中选择出字符串
char* GetStringFrom2YH(char *srcString,char *denString)
{
	char	*p,*p1;
	
	if(strlen(srcString) == 0)
		return 0;
	
	p =  srcString;  
	p1 = strstr(p,"\"");
	p  = strstr(p1+1,"\"");
	*(p++) = 0;
	strcpy(denString,p1+1);
	
//	if(strlen(denString)==0)
//		return 0;
	
	return p;
}


//+CMGR: "REC READ","106581541801","","14/11/12,17:09:43+32" $application/vnd.wap.mms-message
void	Getmessage(stcMessageInfo *sMessageInfo,int messageNum)
{
	char	*p,*p1;
	char	buftmp[32];
	
	if(ReadMessage((char *)sMessageInfo->info,messageNum))	
	{
		p = sMessageInfo->info;
		printf(p);
		p1 = strstr(p,"+CMGR:");
		p = p1+strlen("+CMGR:");
		
	 			 //READ
		p1 = GetStringFrom2YH(p,buftmp);
		
		p  = p1+1;		  
		p1 = GetStringFrom2YH(p,sMessageInfo->phoneNum);  
	  
		p  = p1+1;//""
		p1 =GetStringFrom2YH(p,buftmp);  
		printf(buftmp); 
		
		p  = p1+1;
		p1 =GetStringFrom2YH(p,sMessageInfo->date);  
		printf(buftmp); 
		
		p = p1+1;
		strcpy(sMessageInfo->info,p);
	}
	
	printf("\r\n %s \r\n %s\r\n %s",sMessageInfo->phoneNum,sMessageInfo->date,sMessageInfo->info);
}


//
int	CreatNetConnect(char *ip)
{
	char	string[128];
	char	*p;
	int		strLen;
	
	DelayX10ms(50);
	printfcom("AT+CIPCLOSE\r\n");
	DelayX10ms(50); 
	printfcom("AT+CIPSHUT\r\n");	
	DelayX10ms(50); 
	printfcom("AT+CLPORT=\"TCP\",\"8000\"\r\n");
	DelayX10ms(50);
	strLen = ReadCom3(string,sizeof(string)); 
	
	sprintf(string,"AT+CIPSTART=\"TCP\",\"%s\",\"9000\"\r\n",ip);
	printfcom(string);
	DelayX10ms(50);   
	DelayX10ms(50); 
	strLen = ReadCom3(string,sizeof(string));      
	
	p = strstr(string,"CONNECT OK");
	if(p)
		return 1;
	else
		return 0;
}

void	SendNetInfo(char *string)
{
	char	messageEnd = 0x1a;
	DelayX10ms(50);
	printfcom("AT+CIPSEND\r\n");
	DelayX10ms(50);
	printfcom(string);
	DelayX10ms(50);
	SendCom3(&messageEnd,sizeof(messageEnd));
	DelayX10ms(50);   			
}

void NetConnect(void)
{
	//char ip[] = "175.8.119.42";
	char ip[] = "www.nanche.com";   
	
	if(CreatNetConnect(ip))
	{
		SendNetInfo("net test!!");	
	}
}


void SimTast(void)
{
	char	phoneNum[16];   
	char	messageBuf[256];   	
	stcMessageInfo	sMessageInfo;
	
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
		case REC_MESSAGE:	Getmessage((stcMessageInfo *)&sMessageInfo,1); 
							break;
							
		case NET_CONNECT:	NetConnect();
							break;
							
		default:	;
	}
	
	gSimTastCode = 0;
	
}

