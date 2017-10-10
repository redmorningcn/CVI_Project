#include 	<ansi_c.h>
#include 	<userint.h>
#include 	<utility.h>
#include 	<rs232.h>
#include   	"config.h"
#include    "string.h" 
#include    "DealData.h" 
#include    "CrcCheck.h" 


#define         FRAM_HERD0     		0x10         	//报文头
#define         FRAM_HERD1     		0x28       		//报文头				
#define     	HOST_ID      		80				//上位机地址	  		   
#define   		DEVICE_ID0      	15				//本机地址0	
#define   		DEVICE_ID1      	15				//本机地址1	
	   
#define			RS485_CHK_SUM		0x02			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define			RS485_CHK_CRC		0x01			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define			RS485_CHK_RESUM		0x03			//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 

#define         FRAM_END0     		0x10         	//报文尾
#define         FRAM_END1     		0x2C       		//报文尾	


uint8	g_MastSlaveFlg;

extern uint8	GetFramRecEndFlg(void);

//清接收完成标志  南瑞
extern void	ClearFramRecEndFlg(void);

extern uint16	ReadRs485Data(uint8 *Buf); 
  			
extern	unsigned char DeviceID;
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
			//PrintfCOM0(("\r\n DataLen %d,%x,%x",DataLen,RecBuf[i+10+DataLen],RecBuf[i+11+DataLen]);
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
					
			//PrintfCOM0(("\r\n 号 %x",RecBuf[i+4]);				
			DataLen = RecBuf[i+6];
			
			SumCheck8 = GetCheckSum((uint8 *)&RecBuf[i+2],5);		   				//索引区校验
						
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

void	HostSend_CSNR(uint8 slaveAddr,uint8	*DataBuf,uint32	DataLen)
{

	uint8	SndBuf[100] = {0};
	uint8	SndBufTmp[100] = {0};
	uint8	SumCheck8;
	uint16	Crc16;
	uint16	AddHeadNum;
	uint16	i,j;
	uint8	Tmp;
	uint8	ByteStation;
	static	uint8 FramNum =0;						//序号
	
	
	ByteStation = 0;
	SndBuf[ByteStation++] = FRAM_HERD0;
	SndBuf[ByteStation++] = FRAM_HERD1;
	SndBuf[ByteStation++] = HOST_ID;	 
	SndBuf[ByteStation++] = slaveAddr;				//地址
	SndBuf[ByteStation++] = FramNum++;	
	SndBuf[ByteStation++] = RS485_CHK_CRC<<4;	
	SndBuf[ByteStation++] = DataLen;
	
	SumCheck8 = GetCheckSum(&SndBuf[2],5);		   	//索引区校验

	SndBuf[ByteStation++] = SumCheck8;

	memcpy(&SndBuf[8],DataBuf,DataLen);

	Crc16 = GetCrc16Check(&SndBuf[2],6+DataLen);

	//memcpy(&SndBuf[2+6+DataLen],(uint8 *)&Crc16,sizeof(Crc16));
	
	SndBuf[2+6+DataLen+1] = Crc16;
	SndBuf[2+6+DataLen]   = Crc16>>8;


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
	
	//地址判断
	if(slaveAddr & 0xA0 == 0xA0)
	{
		SendDataCom(SndBufTmp, 2+6+DataLen + 2 +AddHeadNum +2) ; 
	}
	else if(slaveAddr & 0xC0 == 0xC0)
	{
		SendDataCom(SndBufTmp, 2+6+DataLen + 2 +AddHeadNum +2) ; 		
	}
	//SendCOM1(SndBufTmp,2+6+DataLen + 2 + 2 + AddHeadNum);
	//SendCOM0(SndBufTmp,2+6+DataLen + 2 + 2 + AddHeadNum);
}


#define		BUF_LEN		256

uint8	g_NoRs485ComFlg = 0;
uniRs485DataBuf g_uRs485Data;
extern	void	SetText(void) ;
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
			//printf("\r\n RS485数据接收失败!");
		}
		else
		{
			//printf("\r\n RS485数据接收成功!");
			
			memcpy(g_uRs485Data.Buf,DataBuf,sizeof(g_uRs485Data.Buf));

			//Time = GetSysTime();

			//Sendrs485(DataBuf,RS485_DATA_LEN);				//发送数据			

			SetText();
			g_NoRs485ComFlg = 0;								//置通讯标志	
		}
	}
	
	//if(GetSysTime() - Time > 20)								//200ms无通讯，退出
	{
	//	g_NoRs485ComFlg = 1;
	}
}

