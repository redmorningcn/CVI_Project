#include <ansi_c.h>
#include <includes.h>


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

////////////////////////////////////mdk 0

 

////////////////////////////////////////////

unsigned char   l_recslaveaddr = 0;
unsigned char GetRecSlaveAddr(void)
{
	return	l_recslaveaddr;
}

uint8	l_recframnum = 0;
uint8	GetRecFramNum(void)
{
	return 	l_recframnum;
}

//----------------------------------------------------------------------------
// 名    称：   
// 功    能：   接
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
unsigned char  CSNR_GetData(unsigned char	*RecBuf,unsigned char RecLen,unsigned char	*DataBuf,unsigned char	*InfoLen)
{
    unsigned short		i,j,k;
    unsigned char		SumCheck8;
    unsigned char		Tmp;
    unsigned short		RecCRC16,Check16;
	
	unsigned char 		SourceAddr;	
	unsigned char		DataLen;

  
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
			 	//i = i + 10 + DataLen;											//地址不对或帧尾错误，跳过此帧
			 	continue;
			 }
			

			SourceAddr = RecBuf[i+2];	
			l_recslaveaddr = RecBuf[i+3];	
			l_recframnum   = RecBuf[i+4];
			//PrintfCOM0(("\r\n 号 %x",RecBuf[i+4]);				
			DataLen = RecBuf[i+6];
			*InfoLen = DataLen; 
			
			SumCheck8 = GetCheckSumNR((unsigned char *)&RecBuf[i+2],5);		   				//索引区校验
						
			RecCRC16 = ((unsigned short)RecBuf[i+8+DataLen]) *256 + RecBuf[i+9+DataLen];	//校验	

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
				 
				 //return	1;				//SourceAddr
				 return		SourceAddr;
			}
		}
	}
  
    return	0;
}

uint8	l_FramNum = 0;
uint8	GetFramNum(void)
{
	return l_FramNum;		
}

void	AddFramNum(void)
{
	l_FramNum++;		
}

void	ClearFramNum(void)
{
	l_FramNum = 0;		
}

void	DataSend_CSNR(unsigned char SourceAddr,unsigned char DistAddr,unsigned char	*DataBuf,unsigned int	DataLen)
{
	unsigned char	SndBuf[250] = {0};
	unsigned char	SndBufTmp[250] = {0};
	unsigned char	SumCheck8;
	unsigned short	Crc16;
	unsigned short	AddHeadNum;
	unsigned short	i;
	unsigned char	ByteStation;
	static	unsigned char FramNum =0;						//序号
	
	ByteStation = 0;
	SndBuf[ByteStation++] = FRAM_HERD0;
	SndBuf[ByteStation++] = FRAM_HERD1;
	SndBuf[ByteStation++] = SourceAddr;	 
	SndBuf[ByteStation++] = DistAddr;				//地址
//	SndBuf[ByteStation++] = FramNum++;	
	SndBuf[ByteStation++] = GetFramNum();
	SndBuf[ByteStation++] = RS485_CHK_CRC<<4;	
	SndBuf[ByteStation++] = DataLen;
	
	SumCheck8 = GetCheckSumNR(&SndBuf[2],5);		   	//索引区校验

	SndBuf[ByteStation++] = SumCheck8;

	memcpy(&SndBuf[8],DataBuf,DataLen);

	Crc16 = GetCrc16Check(&SndBuf[2],6+DataLen);

	//memcpy(&SndBuf[2+6+DataLen],(unsigned char *)&Crc16,sizeof(Crc16));
	
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

	if((DistAddr & 0xA0) == 0xA0)
	{
		SendCom0(SndBufTmp, 2+6+DataLen + 2 +AddHeadNum +2) ;
	}
	else if((DistAddr & 0xC0) == 0xC0)
	{
		SendCom0(SndBufTmp, 2+6+DataLen + 2 +AddHeadNum +2) ; 	
	}
	else
	{
		SendCom0(SndBufTmp, 2+6+DataLen + 2 +AddHeadNum +2) ; 	
	}
	
	
	//SendCOM1(SndBufTmp,2+6+DataLen + 2 + 2 + AddHeadNum);
	//SendCOM0(SndBufTmp,2+6+DataLen + 2 + 2 + AddHeadNum);
}
