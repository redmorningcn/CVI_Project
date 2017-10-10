#include <ansi_c.h>
#include "includes.h"  


typedef struct	_stcRs485DataDefine
{
	uint8	LifeInfo;
	uint8	Rec1;
	uint8	Rec2;
	uint8	NeutralDistH;
	uint8	NeutralDistL;
	uint8	NeutralCod;
	uint8	Rec6;
	uint8	Rec7;
	uint8	LocoAxle;
	uint8	OtherAxle;
	uint8	AxleCod;
	uint8	Rec11;
	uint8	UnionControl;
	uint8	Rec13;
	uint8	Rec14;
	uint8	Rec15;
}stcRs485DataDefine;


#define		RS485_DATA_LEN	16  
typedef	union	_uniRs485DataBuf
{
	//
	uint8					Buf[RS485_DATA_LEN];
	stcRs485DataDefine		sRs485Data;
}uniRs485DataBuf;


#define         FRAM_HERD0     		0x10         	//报文头
#define         FRAM_HERD1     		0x28       		//报文头				
#define     	HOST_ID      		10				//上位机地址	  		   
#define   		DEVICE_ID0      	15				//本机地址0	
#define   		DEVICE_ID1      	15				//本机地址1	
	   
#define			RS485_CHK_SUM		0x02			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define			RS485_CHK_CRC		0x01			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define			RS485_CHK_RESUM		0x03			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 

#define         FRAM_END0     		0x10         	//报文尾
#define         FRAM_END1     		0x2C       		//报文尾	

//////////////////////////////////// cvi 0

uint8	g_MastSlaveFlg;

uint8	GetFramRecEndFlg(void)
{
	return GetRecCom4EndFlg();
}

//清接收完成标志  南瑞
void	ClearFramRecEndFlg(void)
{
	ClearRecCom4EndFlg();
}

uint16	ReadRs485Data(uint8 *Buf)
{
		return	ReadCom4(Buf,256);
}

  			
//extern	unsigned char DeviceID;
//////////////////////////////////// cvi 1
//----------------------------------------------------------------------------
// 名    称：   
// 功    能：   接
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
uint8  RecData(uint8	*RecBuf,uint8 RecLen,uint8	*DataBuf,uint8	DataLen)
{
    uint16		i,j,k;
    uint8		SumCheck8;
    uint8		Tmp;
    uint16		RecCRC16,Check16;
	
	uint8	DeviceID;
	
//	if(g_MastSlaveFlg == 0)
//	{
//		DeviceID = DEVICE_ID0;
//	}
//	else
//	{
//		DeviceID = DEVICE_ID1;
//	}    
  
  
	for(j = 0;j < RecLen ;j++ )														//去重复通讯首字节
	{
		if(	 	RecBuf[j] 	== FRAM_HERD0
			&& 	RecBuf[j+1] == FRAM_HERD0
		   )
		{
			for(k = j;k < RecLen; k++)
			{
				Tmp	 = RecBuf[k+1];
				
				RecBuf[k] = Tmp;
			}
		}
	}
	
    for(i = 0; i < RecLen - 10;i++)												//循环遍历
    {
		if(		((RecBuf[i+0]) == FRAM_HERD0) 
			&& 	((RecBuf[i+1]) == FRAM_HERD1)		
		  )																		//报头
		{
			DataLen = RecBuf[i+6];
			printf("\r\n DataLen %d,%x,%x",DataLen,RecBuf[i+10+DataLen],RecBuf[i+11+DataLen]);
			if(		
					//	(RecBuf[i+2] != DeviceID) 				||
					  //	(RecBuf[i+3] != HOST_ID) 				||
				 		(RecBuf[i+10+DataLen] 	!= FRAM_END0)	||
				 	 	(RecBuf[i+11+DataLen] 	!= FRAM_END1)
			 )
			 {	
			 	i = i + 10 + DataLen;											//地址不对或帧尾错误，跳过此帧
			 	continue;
			 }
					
			printf("\r\n 号 %x",RecBuf[i+4]);				
			DataLen = RecBuf[i+6];
			
			SumCheck8 = GetCheckSumNR((uint8 *)&RecBuf[i+2],5);		   				//索引区校验
						
			RecCRC16 = ((uint16)RecBuf[i+8+DataLen]) *256 + RecBuf[i+9+DataLen];	//校验	

			switch(RecBuf[i+5]>>4)													//校验和算法判断
			{
					case	RS485_CHK_SUM:
									//Check16 = GetCheckSum16(&RecBuf[i+2],6+DataLen);
									////PrintfCOM0(("\r\n 16位校验和！");
									break;
					case	RS485_CHK_CRC:
									Check16 = GetCrc16Check(&RecBuf[i+2],6+DataLen);
									////PrintfCOM0(("\r\n CRC校验！");
									break;
					case	RS485_CHK_RESUM:
									break;
			}
			
			
			printf("\r\n SumCheck8 %x,%x",SumCheck8,RecBuf[i+7]);  
			printf("\r\n RecCRC16  %x,%x",RecCRC16,Check16);   
			
			if(			
						SumCheck8 == 	RecBuf[i+7] 
					&& 	RecCRC16  ==	Check16 				
			  )
			{
				 memcpy(DataBuf,&RecBuf[i+8],DataLen);				//数据拷贝
				 
				 return	1;
			}
		}
	}
  
    return	0;
}
/* */

extern	int		ComSelect;
extern		void	SendDataCom(unsigned char *Buf,int  Buflen);
//extern	char 	DeviceID;

void	Sendrs485(uint8	*DataBuf,uint32	DataLen)
{

	uint8	SndBuf[100] = {0};
	uint8	SndBufTmp[100] = {0};
	uint8	SumCheck8;
	uint16	Crc16;
	uint16	AddHeadNum;
	uint16	i,j;
	uint8	Tmp;
	uint8	ByteStation;
	static	uint8 FramNum =0;			//序号
	uint8	DeviceID;
	
//	if(g_MastSlaveFlg == 0)
	{
//		DeviceID = DEVICE_ID0;
	}
//	else
//	{
	DeviceID = DEVICE_ID1;
//	}
	
	
	ByteStation = 0;
	SndBuf[ByteStation++] = FRAM_HERD0;
	SndBuf[ByteStation++] = FRAM_HERD1;
	SndBuf[ByteStation++] = HOST_ID;	 
	SndBuf[ByteStation++] = DeviceID;	
	SndBuf[ByteStation++] = FramNum++;	
	SndBuf[ByteStation++] = (RS485_CHK_CRC<<4) + 1;	
	SndBuf[ByteStation++] = DataLen;
	
	SumCheck8 = GetCheckSumNR(&SndBuf[2],5);		   	//索引区校验
	SndBuf[ByteStation++] = SumCheck8;
	
	memcpy(&SndBuf[8],DataBuf,DataLen);

	Crc16 = GetCrc16Check(&SndBuf[2],6+DataLen);

	//memcpy(&SndBuf[2+6+DataLen],(uint8 *)&Crc16,sizeof(Crc16));
	
	SndBuf[2+6+DataLen+1] = Crc16;
	SndBuf[2+6+DataLen] = Crc16>>8;


	SndBufTmp[0] = SndBuf[0];						//准备数据发送
	SndBufTmp[1] = SndBuf[1];
	
	AddHeadNum = 0;
	for(i = 2; i< 2+6+DataLen + 2;i++ )				//数据，补移位 FRAM_HERD0
	{
		SndBufTmp[i+AddHeadNum] = SndBuf[i];
		
		if(SndBuf[i] == FRAM_HERD0)
		{
			SndBufTmp[i+AddHeadNum+1] = FRAM_HERD0;
			AddHeadNum++;
		}
	}

	SndBufTmp[2+6+DataLen + 2 +AddHeadNum] = FRAM_END0;
	SndBufTmp[2+6+DataLen + 2 +AddHeadNum+1] = FRAM_END1;      
	
	SendCom4(SndBufTmp, 2+6+DataLen + 2 +AddHeadNum +2) ; 
	
	//SendCOM1(SndBufTmp,2+6+DataLen + 2 + 2 + AddHeadNum);
	//SendCOM0(SndBufTmp,2+6+DataLen + 2 + 2 + AddHeadNum);
}



#define		BUF_LEN		256

uint8	g_NoRs485ComFlg = 0;
uniRs485DataBuf g_uRs485Data;
extern	void	SetText(void)       ;
//----------------------------------------------------------------------------
// 名    称：   void  RecTAX2Info(void)
// 功    能：   接收TAX2信息。
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
void  RecRs485(void)
{
	static	uint8		RecBuf[BUF_LEN] = {0};								//接收缓冲区
	static	uint8		DataBuf[BUF_LEN/2] = {0};								//
	static	uint32		Time = 0;
	uint32	i,j;

	static	 uint8	BufTestTmp[25] = {0};
	static	 uint32	TestTmp = 0;

	
	uint32		RecLen;		
	uint32		DataLen = 16;		
			
			
	if( GetFramRecEndFlg() == 1)								//有新的帧尾，进行数据处理
	{
		RecLen 	= ReadRs485Data(RecBuf);
		
		ClearFramRecEndFlg();									//清标志，可重新取数
		
		if(!RecData(RecBuf,RecLen,DataBuf,DataLen))				//接受数据
		{
			printf("\r\n RS485数据接收失败!");
		}
		else
		{
			printf("\r\n RS485数据接收成功!");
			
			
			g_NoRs485ComFlg = 0;								//置通讯标志	
		}
	}
	
	//if(GetSysTime() - Time > 20)								//200ms无通讯，退出
	{
	//	g_NoRs485ComFlg = 1;
	}
}




void ascomtest(void)
{
	if(GetOpenCom4Flg())
	{
		
		RecRs485();	
	}
}
