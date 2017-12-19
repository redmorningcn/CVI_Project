#include <ansi_c.h>
#include "includes.h" 

#define		BROAD_ADDR			0xAF

#define		GET_DIP_PRS			0xc0
#define		SET_DIP_MOD			0xD7
#define		SET_PRS_LIN			0xD6

#define		TEST_COM			0xD4
#define		BROAD_INFO			0x66

#define		INF_HEAD_LEN			5
#define		INF_START_POS			4


typedef	struct	_stcCalDist_
{
	uint32	ModDPR;					//计算余量
	uint32	Dist;					//里程
}stcCalDist;

typedef struct _stcDist_						//油尺
{
	stcCalDist	A_sDist;						//总里程
	stcCalDist	C_sDist;						//分段里程
	stcCalDist	E_sDist;						//自定义里程
	uint32		Rec32;							//预留4字节
	uint16		Speed;							//速度
	uint16		Crc16;							//CRC检验
}stcDist;

stcDist	 	g_sDist;		

//------------------------------------------------------------------------
//  功  能 ：存油箱模型
// 入口参数：无
// 出口参数：当前流水号
//------------------------------------------------------------------------
uint8	InitDistData(void)
{
	uint16	CrcCheck;
	uint8	Flg;

	g_sDist.A_sDist.Dist	= 12340;
	g_sDist.Speed 			= 68;   	
	
	CrcCheck = GetCrc16Check((uint8 *)&g_sDist,sizeof(stcDist) - 2);
	
	g_sDist.Crc16 = CrcCheck;

	return	1;
}


//------------------------------------------------------------------------
// 功    能：   向测量装置广播信息
// 入口参数：	Buf:数据缓冲区 BufLen:数据缓冲区数据长度 ；Typ:数据类型
// 出口参数：	无
//------------------------------------------------------------------------
void SendBroadInfo(uint8 *Buf,uint32	BufLen,uint8 Typ)		
{
	uint8	SourceAddr,TargetAddr,DatTyp,StartCode,DataLen;
	uint8	BufTmp[50];
	uint8	i = 0;
	uint16	CrcCheck;
	
	DatTyp		=	Typ;
	StartCode 	= 	COM_START_CODE;
	TargetAddr  = 	BROAD_ADDR;	
	SourceAddr 	=	HOST_ADDR;
	DataLen		= 	BufLen + sizeof(DatTyp);
	
	BufTmp[i++] =	StartCode;
	BufTmp[i++] =	TargetAddr;
	BufTmp[i++] =	SourceAddr;
	BufTmp[i++] =	DataLen;
	BufTmp[i++] =	DatTyp;
	
	memcpy(&BufTmp[i],Buf,BufLen);
	
	CrcCheck 	=	GetCrc16Check(BufTmp,BufLen + INF_HEAD_LEN);				
	
	memcpy(&BufTmp[BufLen + i],(uint8 *)&CrcCheck,sizeof(CrcCheck));	

	SendCom4(BufTmp,i + BufLen + sizeof(CrcCheck));					
}


uint8	l_SpeedDataConFlg =0;    


extern int			l_programCom4Num ; 
extern  unsigned	int	l_recCom4Time ;  
extern  char		l_recCom4EndFlg ;
extern  char		l_Com4StartFlg ;
//------------------------------------------------------------------------
// 名    称：   void    BroadsDist(void)	
// 功    能：   广播里程信息
// 入口参数：	无
// 出口参数：	无
//------------------------------------------------------------------------
void    BroadsDist(void)
{
	static	uint32	Time  = 0;
	static	uint32	Times = 0;
	uint32	len;
	uint8	Buf[64];
	
	if(GetOpenCom4Flg())
	{
		if( GetSysTime() - Time > 100)		//0.25s广播一次
		{
			InitDistData();
	
			SendBroadInfo((uint8 *)&g_sDist,sizeof(stcDist),BROAD_INFO);	
	
			Time = GetSysTime();											//读当前值
			
			Times++;
			// 
		}
		
		len = ReadCom4(Buf,sizeof(Buf)); 
		if(len)	
		{
			l_SpeedDataConFlg = 1;
			ClearRecCom4EndFlg();    
			Times = 0;
		}
		else
		{
			if(Times > 3)
			{
				 l_SpeedDataConFlg = 0;
			}
		}
	}
}

uint8	GetSpeedDataConFlg(void)
{
	return	  l_SpeedDataConFlg;
}
