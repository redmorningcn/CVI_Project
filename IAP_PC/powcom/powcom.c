#include <ansi_c.h>
#include "includes.h" 

typedef	struct	_stcModBusRead_
{
    unsigned char	DeviceAddr;
    unsigned char	WorkCode;
    unsigned short	RegStart;
    unsigned short	RegLenth;	
    unsigned short	Crc16;   	
}stcModBusRead;

typedef	struct	_stcModBusPower_
{
	unsigned char	Voltage[2];
	unsigned char	Current[2];
	unsigned char	Power[2];
	unsigned char	PosEnergy[4];
	unsigned char	NegEnergy[4];
//	unsigned char	AddNBound[2];
//	unsigned char	ModName[4];
}stcModBusPower;

typedef	struct	_stcPower_
{
	short	Voltage;
	short	Current;
	short	Power;
//	short	AddNBound;	
	int	PosEnergy;
	int	NegEnergy;
//	int	ModName;
}stcPower;

stcPower	sPower;

#define		MODBUS_HEAD		0x01
#define		MODBUS_READ		0x03
#define		MODBUS_SET		0x10
#define		MODBUS_LEN		0x07

//------------------------------------------------------------------------
// 名称：	void	SendReadPowerCode(void)
// 功能：初始化定时器1，定时时间单位为10ms，并使能中断。
// 入口参数：无
// 出口参数：无
//从设备地址	功能码	起始寄存器地址	寄存器个数	CRC-L	CRC-H
//0x01	0x03	0x00	0x10	0x00	0x07	0x05	0xCD
//------------------------------------------------------------------------
void  SendReadPowerCode(void)
{ 
	static	stcModBusRead		sModBusRead;
    unsigned short	Crc16;
    
    sModBusRead.DeviceAddr 		= MODBUS_HEAD;
    sModBusRead.WorkCode		= MODBUS_READ;
   	sModBusRead.RegStart		= 0x1000;
   	sModBusRead.RegLenth		= 0x0700;
   	   	
   	Crc16 = GetModBusCrc16Up((unsigned char *)&sModBusRead,sizeof(sModBusRead) - 2);
   	
   	sModBusRead.Crc16 			= Crc16; 
   	
   	SendCom2((unsigned char *)&sModBusRead,sizeof(sModBusRead));
}

uint8	l_PowerDataConFlg =0;
int	JudgeHostPowerData(unsigned char *Buf,int len)
{
	static	stcModBusRead		sModBusRead;
    unsigned short	Crc16;
	static	int	ErrTimes;
	uint16	tmp16;
	uint32	i;
	
    sModBusRead.DeviceAddr 		= MODBUS_HEAD;
    sModBusRead.WorkCode		= MODBUS_READ;
   	sModBusRead.RegStart		= 0x1000;
   	sModBusRead.RegLenth		= 0x0700;
	
	if(len >= sizeof(sModBusRead))
    {
	    for(i = 0; i <= (len - sizeof(sModBusRead));i++)
	    {
			if(	(Buf[i] == sModBusRead.DeviceAddr )		&&		
				(Buf[i + 1] == sModBusRead.WorkCode) 
			  ) 	
			{	
				memcpy((uint8 *)&tmp16,&Buf[i+2],sizeof(tmp16));
				if(tmp16 == sModBusRead.RegStart)
				{   
					memcpy((uint8 *)&tmp16,&Buf[i+2+sizeof(tmp16)],sizeof(tmp16)); 
					
					if(tmp16 == sModBusRead.RegLenth)
					{
						tmp16 = GetModBusCrc16Up(&Buf[i],sizeof(sModBusRead)-2);
						memcpy((uint8 *)&Crc16,&Buf[i+sizeof(sModBusRead)-2],sizeof(Crc16));
						
						if(tmp16 == Crc16)
						{
							ErrTimes = 0;	
							l_PowerDataConFlg = 1;
							
							return 1;
						}
					}
				}
			}
	    }
    }    
	
	ErrTimes++;
	
	if(ErrTimes > 10)
	{
		l_PowerDataConFlg = 0;
	}
	
    return	0;		
}

uint8	GetPowerDataConFlg(void)
{
	return	l_PowerDataConFlg;
}

//----------------------------------------------------------------------------
// 名    称：   unsigned char  GetDipData(void)
// 功    能：   取油量数据
// 入口参数：   无
// 出口参数：   无
//----------------------------------------------------------------------------
unsigned char  SendPowerValtoHost(void)
{
	unsigned char		buf[128];
	short				i = 0;	
	unsigned short		RecCheck = 0,Crc16;
	unsigned short		ModBusLen;
	stcModBusPower 		sModBusPower;
	unsigned int		tmp;

	ModBusLen = 1 + 1 + 1 + MODBUS_LEN*2 + 2;         		//地址+操作码+数据长度字节+数据+校验

	tmp = 500;
	memcpy(sModBusPower.Voltage,(unsigned char *)&tmp,sizeof(sModBusPower.Voltage));
	tmp = 200;
	memcpy(sModBusPower.Current,(unsigned char *)&tmp,sizeof(sModBusPower.Current));
	tmp = 200*500;
	memcpy(sModBusPower.Power,(unsigned char *)&tmp,sizeof(sModBusPower.Power));
	tmp = 1000000;
	memcpy(sModBusPower.PosEnergy,(unsigned char *)&tmp,sizeof(sModBusPower.PosEnergy));
	tmp = 100;
	memcpy(sModBusPower.NegEnergy,(unsigned char *)&tmp,sizeof(sModBusPower.NegEnergy));	
	
	buf[i++] =  MODBUS_HEAD;
	buf[i++] = 	MODBUS_READ;
	buf[i++] = 	MODBUS_LEN*2;
	
	memcpy(&buf[i],(unsigned char *)&sModBusPower,sizeof(sModBusPower));
	
	Crc16 = GetModBusCrc16Up(buf,i+sizeof(sModBusPower)); 
	
	memcpy(&buf[i+ sizeof(sModBusPower)],(unsigned char *)&Crc16,sizeof(Crc16));
	
	SendCom2(buf,ModBusLen);
	
	return 0;
}


void  SimulatePow(void)
{
	short	prs;
	unsigned char buf[128];
	int		len;
	static	uint32	time; 
	
	if(GetOpenCom2Flg())
	{
		if(GetRecCom2EndFlg())	
		{
			//printf("Receive Com2 Data\r\n");
			
			ClearRecCom2EndFlg();
		
			len = ReadCom2(buf,sizeof(buf));
		
			//printf("len = %d\r\n",len);    

			
			if(JudgeHostPowerData(buf,len))
			{
				SendPowerValtoHost();
			}
			
			time = GetSysTime();
		}
		else
		{
			if(GetSysTime() - time > 100)
			{
				l_PowerDataConFlg = 0; 
			}
		}		
		
	}
}

