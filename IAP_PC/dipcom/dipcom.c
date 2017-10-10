#include <ansi_c.h>
#include "includes.h"

#define		COM_START_CODE		0x55
#define		PC_ADDR				0xaa
#define		HOST_ADDR			0x80
#define		DIP1_ADDR			0xA1
#define		DIP2_ADDR			0xA2
#define		BROAD_ADDR			0xAF
#define		SENSOR1_ADDR		0xB1
#define		SENSOR2_ADDR		0xB2

#define		GET_DIP_PRS			0xc0
#define		SET_DIP_MOD			0xD7
#define		SET_PRS_LIN			0xD6
#define		OUT_SET_PRS			0xD4

#define		DIP_REC_FRAM_LEN		15									//测量装置返回的数据帧长
#define		INF_START_POS			4
				


void SentDip1prsToHost(short int dipPrs)		
{
	unsigned char	SourceAddr,TargetAddr,StartCode,FramLen;
	unsigned char	BufTmp[24];
	unsigned char	i = 0;
	unsigned short int	CrcCheck;
	
	StartCode 	= 	COM_START_CODE;
	TargetAddr  = 	HOST_ADDR;	
	SourceAddr 	=	DIP1_ADDR;
	FramLen		= 	DIP_REC_FRAM_LEN - INF_START_POS - 2;
	
	BufTmp[i++] =	StartCode;
	BufTmp[i++] =	TargetAddr;
	BufTmp[i++] =	SourceAddr;
	BufTmp[i++] =	FramLen;
	
	memcpy(&BufTmp[i],(unsigned char *)&dipPrs,sizeof(dipPrs));
	
	CrcCheck 	=	GetCrc16Check(BufTmp,DIP_REC_FRAM_LEN - 2);				//求校验和
	
	memcpy(&BufTmp[DIP_REC_FRAM_LEN - 2],(unsigned char *)&CrcCheck,sizeof(CrcCheck));	

	SendCom0(BufTmp,DIP_REC_FRAM_LEN);					//发送内容
}

short	GetDip1PrsVal(void)
{
	return	g_dip1Prs;
}

uint8 l_Dip1DataConFlg = 0;
int		JudgeHostToDip1Data(unsigned char *Buf,int len)
{
    unsigned char			BufTmp[24] = {0};
    unsigned short int		CrcCheck;
    unsigned short int		i;
    unsigned char			StartCode,TargetAddr,SourceAddr,DataLen;
    static	unsigned int	ErrTimes = 0;
    static	unsigned int	Times = 0;
	unsigned int			framLen = 12;
	 
	
	StartCode 	= COM_START_CODE;
	TargetAddr 	= DIP1_ADDR;
	SourceAddr 	= HOST_ADDR;
	DataLen		= 5+1;
	
	framLen =  DataLen + 4 + 2;
	
	Times++;
	if(len >= framLen)
    {
	    for(i = 0; i <= (len - framLen);i++)
	    {
			if(	(Buf[i] == StartCode )		&&		
				(Buf[i + 1] == TargetAddr) 	&& 
				(Buf[i + 2] == SourceAddr) 	&& 
				(Buf[i + 3] == DataLen)     &&
				(Buf[i + 4] == GET_DIP_PRS)  ) 	
			{					
				memcpy(BufTmp,&Buf[i],framLen);
				
				memcpy((unsigned char *)&CrcCheck,&BufTmp[framLen - 2],sizeof(CrcCheck));
				
				if(CrcCheck == GetCrc16Check(BufTmp,framLen - 2))
				{
					memcpy((unsigned char *)&g_dipval,&BufTmp[INF_START_POS+2],sizeof(g_dipval));
					
					l_Dip1DataConFlg = 1;
					
					Times = 0;
					ErrTimes = 0;						
					return 1;
				}
			}
	    }
	    
	    if(Times > 5)
	    {
	    	//l_Dip1Prs = 0;
	    }
    }    
	
	ErrTimes++;
	
	if(ErrTimes > 10)
	{
		l_Dip1DataConFlg = 0;
	}
	
    return	0;		
}

uint8	GetDip1DataConFlg()
{
	return	l_Dip1DataConFlg;	
}


void	Dip1model(void)
{
	short	prs;
	unsigned char buf[128];
	int		len;
	static	uint32	time;
	
	
	if(GetOpenCom0Flg())
	{
		if(GetRecCom0EndFlg())	
		{
			ClearRecCom0EndFlg();
		
			len = ReadCom0(buf,sizeof(buf));
		
			if(JudgeHostToDip1Data(buf,len))
			{
				prs = GetDip1PrsVal();	  
		
				SentDip1prsToHost(prs);
			}
			else
			{
			}
			
			time = GetSysTime();
		}
		else
		{
			if(GetSysTime() -time>100)
			{
				l_Dip1DataConFlg = 0;  
			}
		}
	}
}


uint8 l_Dip2DataConFlg = 0;
int		JudgeHostToDip2Data(unsigned char *Buf,int len)
{
    unsigned char			BufTmp[24] = {0};
    unsigned short int		CrcCheck;
    unsigned short int		i;
    unsigned char			StartCode,TargetAddr,SourceAddr,DataLen;
    static	unsigned int	ErrTimes = 0;
    static	unsigned int	Times = 0;
	unsigned int			framLen = 12;
	 
	
	StartCode 	= COM_START_CODE;
	TargetAddr 	= DIP2_ADDR;
	SourceAddr 	= HOST_ADDR;
	DataLen		= 5+1;
	
	framLen =  DataLen + 4 + 2;
	
	Times++;
	if(len >= framLen)
    {
	    for(i = 0; i <= (len - framLen);i++)
	    {
			if(	(Buf[i] == StartCode )		&&		
				(Buf[i + 1] == TargetAddr) 	&& 
				(Buf[i + 2] == SourceAddr) 	&& 
				(Buf[i + 3] == DataLen)     &&
				(Buf[i + 4] == GET_DIP_PRS)  ) 	
			{					
				memcpy(BufTmp,&Buf[i],framLen);
				
				memcpy((unsigned char *)&CrcCheck,&BufTmp[framLen - 2],sizeof(CrcCheck));
				
				if(CrcCheck == GetCrc16Check(BufTmp,framLen - 2))
				{
					memcpy((unsigned char *)&g_dipval,&BufTmp[INF_START_POS+2],sizeof(g_dipval));
					
					l_Dip2DataConFlg = 1;
					
					Times = 0;
					ErrTimes = 0;						
					return 1;
				}
			}
	    }
	    
	    if(Times > 5)
	    {
	    	//l_Dip2Prs = 0;
	    }
    }    
	
	ErrTimes++;
	
	if(ErrTimes > 10)
	{
		l_Dip2DataConFlg = 0;
	}
	
    return	0;		
}

uint8	GetDip2DataConFlg()
{
	return	l_Dip2DataConFlg;	
}

void SentDip2prsToHost(short int dipPrs)		
{
	unsigned char	SourceAddr,TargetAddr,StartCode,FramLen;
	unsigned char	BufTmp[24];
	unsigned char	i = 0;
	unsigned short int	CrcCheck;
	
	StartCode 	= 	COM_START_CODE;
	TargetAddr  = 	HOST_ADDR;	
	SourceAddr 	=	DIP2_ADDR;
	FramLen		= 	DIP_REC_FRAM_LEN - INF_START_POS - 2;
	
	BufTmp[i++] =	StartCode;
	BufTmp[i++] =	TargetAddr;
	BufTmp[i++] =	SourceAddr;
	BufTmp[i++] =	FramLen;
	
	memcpy(&BufTmp[i],(unsigned char *)&dipPrs,sizeof(dipPrs));
	
	CrcCheck 	=	GetCrc16Check(BufTmp,DIP_REC_FRAM_LEN - 2);				//求校验和
	
	memcpy(&BufTmp[DIP_REC_FRAM_LEN - 2],(unsigned char *)&CrcCheck,sizeof(CrcCheck));	

	SendCom1(BufTmp,DIP_REC_FRAM_LEN);					//发送内容
}

short	GetDip2PrsVal(void)
{
	return	g_dip2Prs;
}

void	Dip2model(void)
{
	short	prs;
	unsigned char buf[128];
	int		len;
	static	uint32	time;
	
	if(GetOpenCom1Flg())
	{
		if(GetRecCom1EndFlg())	
		{
			ClearRecCom1EndFlg();
		
			len = ReadCom1(buf,sizeof(buf));
		
			if(JudgeHostToDip2Data(buf,len))
			{
				prs = GetDip2PrsVal();	  
	
				SentDip2prsToHost(prs);
			}
			else
			{
			}
			
			time = GetSysTime();
		}
		else
		{
			if(GetSysTime() - time > 100)
			{
				l_Dip2DataConFlg = 0; 
			}
		}
	}
}

void	SensorSimulator(void)
{

		Dip1model();
		Dip2model();    	
		DelayX10ms(1);

	//printf("\r\n time = %d,%d,%d",GetSysTime(),g_dip1Prs,g_dip2Prs);
}


