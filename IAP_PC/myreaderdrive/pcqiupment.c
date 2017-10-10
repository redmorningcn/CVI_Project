#include <ansi_c.h>
#include "includes.h"

///////////////////////////////////////////////////////////

short 			g_density;
short 			g_hig; 
char			g_modelnum;
unsigned short 	g_loconum;
unsigned short 	g_locotype;


short  GetDensity(void)
{
	return  g_density;
}

short  GetHig(void)
{
	return  g_hig;
}

short  Getmodelnum(void)
{
	return  g_modelnum;
}

unsigned short  GetLocoType(void)
{
	return  g_locotype;
}

unsigned short  GetLocoNum(void)
{
	return  g_loconum;
}

stcModelCard	m_sModelCard;

static	uint32	l_StoreCnt  = 0;

void  GetRecData(stcFlshRec *msFlshRec)
{	
	uint32			FlshAddr;										//Flash存储地址
	stcFlshRec		sFlshRec={0};									//Flsh记录
	stcTAX2Full		sTAXTmp;										//TAXII箱信息缓存
	stcTime			sTime;											//系统时间
	stcFixInfo 		sFixInfo;			
	static	uint32	l_StoreCnt  = 0;			//流水号				
	uint8			l_RecTypCod = 0;       		//记录类型          	1   数据类型（开机、上班卡、下班卡、数据记录）
	uint8			l_DriverUnitNum[2] ={0,0};	//司机单位编号      	2   //用来唯一确定司机
	int32			l_MstLocoPower = 0;        	//主机车功率          	2   外接设备测量
	int32			l_SlvLocoPower = 0;        	//从机车功率          	2   外接设备测量
	uint16 			l_MstDip = 0;           	//主机油量          	2   主油尺油量(主从机级联时为主机油量,否则为本机车油量)
	uint16 			l_SlvDip = 0;           	//从机油量          	2   主油尺油量(主从机级联时为从机油量,否则此油量值为空)
	uint16			l_MstDip1Prs = 0;        	//压强            		2   主机油尺1压强值
	uint16			l_MstDip2Prs = 0;        	//压强            		2   主机油尺2压强值
	uint16			l_SlvDip1Prs = 0;       	//压强            		2   从机油尺1压强值
	uint16 			l_SlvDip2Prs = 0;       	//压强            		2   从机油尺2压强值
	uint16			l_MstDensity = 0;			//密度					2	主机燃油密度
	uint16			l_SlvDensity = 0;			//密度					2	从机燃油密度
	int32			l_GpsLongitude = 0;			//经度					4	地理经度
	int32			l_GpsLatitude  = 0;			//纬度					4	地理纬度
	int16			l_GpsHeight    = 0;			//高度					4	地理高度
	int16			l_MstEngRotSpd = 0;			//主机柴油机转速		2	主机柴油机转速
	int16			l_SlvEngRotSpd = 0;			//从机柴油机转速		2	从机柴油机转速
	uint16 			l_JcRealType = 0;			//机车型号				2   外部设置的机车型号
	uint16 			l_JcRealNo   = 0;			//机车编号				2   外部设置的机车编号
	uint8			l_ErrorCode  = 0;			//故障代码				1   0~255
	uint8 			l_DipValChgState = 0;  		//油量变化状态   		1   油量变化情况分：两油尺增加/减小、变化平稳、轻微波动、剧烈波动
	uint8			l_MstDipTemp = 0;			//温度					1	主机温度
	uint8			l_OilBoxModel= 0;			//油箱模型代码			1	从机温度				
	uint8			l_SoftVes	 = 0;			//软件版本				1					
	uint8			l_Reserve1	 = 0;			//其他信息				1					
	
	
	//l_SoftVes 			= 0x01;				//程序版本
//	l_SoftVes 			= 0x05;					//程序版本
//	l_SoftVes 			= 0x06;					//程序版本
	l_SoftVes 			= 0x07;					//程序版本 20150302  中铁
	

	l_StoreCnt++;
	l_MstDip1Prs		= 0x1111;
	l_MstDip2Prs		= 0x2222;	
//	l_MstDipTemp		= GetDipTmp();
	l_MstDipTemp 		= 1200;	
	l_DipValChgState	= 1300;
	l_MstDip			= 1500;
	
	l_GpsHeight			= 1600;
	
	l_MstDensity		= 1700;
	l_SlvDensity		= 1800;
//			
	l_ErrorCode			= 1;
	l_RecTypCod			= 2;
////	l_MstEngRotSpd		= GetDieselSpeed();	
	l_OilBoxModel		= 3;			
	
	sFlshRec.SoftVes		= l_SoftVes;						 
	sFlshRec.StoreCnt		= l_StoreCnt;						 
	sFlshRec.RecTypCod		= l_RecTypCod;						
   	sFlshRec.MstLocoPower	= l_MstLocoPower;						      
   	sFlshRec.SlvLocoPower	= l_SlvLocoPower;						      
   	sFlshRec.MstDip			= l_MstDip;							
	sFlshRec.SlvDip			= l_SlvDip;							
	sFlshRec.MstDip1Prs		= l_MstDip1Prs;						
	sFlshRec.MstDip2Prs		= l_MstDip2Prs;						
	sFlshRec.SlvDip1Prs		= l_SlvDip1Prs;						
	sFlshRec.SlvDip2Prs		= l_SlvDip2Prs;						
	sFlshRec.MstDensity		= l_MstDensity;						
	sFlshRec.SlvDensity		= l_SlvDensity;						
	sFlshRec.GpsLongitude	= l_GpsLongitude;					
	sFlshRec.GpsLatitude	= l_GpsLatitude;					
	sFlshRec.GpsHeight		= l_GpsHeight;						
	sFlshRec.MstEngRotSpd	= l_MstEngRotSpd;					
	sFlshRec.SlvEngRotSpd	= l_SlvEngRotSpd;					
	sFlshRec.JcRealType		= l_JcRealType;						
	sFlshRec.JcRealNo		= l_JcRealNo;						
	sFlshRec.ErrorCode		= l_ErrorCode;						
   	sFlshRec.MstDipTemp		= l_MstDipTemp;						
   	sFlshRec.OilBoxModel	= l_OilBoxModel;						
	sFlshRec.DipValChgState	= l_DipValChgState;					
	
	ReadTime((stcTime *)&sTime);											// 时间   
	
	sFlshRec.MyYear			= sTime.Year;			
	sFlshRec.MyMonth		= sTime.Month;			
	sFlshRec.MyData			= sTime.Date;			
	sFlshRec.MyHour			= sTime.Hour;			
	sFlshRec.MyMinute		= sTime.Min;			
	sFlshRec.MySecond		= sTime.Sec;			
	
	memcpy((unsigned char *)msFlshRec,(unsigned char *)&sFlshRec,sizeof(sFlshRec));
	
	sFlshRec.CrcCheck = GetCrc16Check((uint8 *)&sFlshRec,sizeof(sFlshRec) - 2);
}


void	 EquipmentReadData(unsigned int startnum,unsigned int endnum)
{
	unsigned int  recnum;
	stcFlshRec		sFlshRec={0};								//Flsh记录   
	
	recnum = endnum - startnum;
	
	if(recnum >1000)
	{
		recnum = 100;
	}
	while(recnum--)
	{
		GetRecData((stcFlshRec *)&sFlshRec);

		DataSend_CSNR(0x80,0xca,(unsigned char *)&sFlshRec,sizeof(sFlshRec));
		printfcom1("\r\n 发送数据 %d,%d",recnum,sizeof(sFlshRec));
	}
}

extern	uint8	l_befaddr;


unsigned char l_recFlg = 0;
unsigned char GetRecFlg(void)
{
	return l_recFlg;
}



//////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


uint8	GetFramRecEndFlg(void)
{
	return GetRecCom0EndFlg();  
}

//清接收完成标志  南瑞
void	ClearFramRecEndFlg(void)
{
	ClearRecCom0EndFlg();   
}

uint16	ReadRs485Data(uint8 *Buf)
{
		return	ReadCom0(Buf,256);  
}


unsigned int  l_eqiupmentcode = 0;

unsigned char *pmodel;
static stcModelCard  m_sModelCard;

unsigned char l_modelcardflg = 0;

unsigned char GetModelCardFlg(void)
{
	return	l_modelcardflg;
}

uint8	sentendflg = 0;
uint8	l_befaddr = 0;

void CarddataOperate(unsigned char *buf,unsigned int Len)		
{
	unsigned int   datalen = 0;
	unsigned int   modeldatanum = 0;
	
	if(Len < 4)
	{
		printfcom1("\r\n 数据长度小于4，错误");
		return	;
	}
	
	datalen = 0;
	memcpy((unsigned char *)&l_eqiupmentcode,&buf[datalen],sizeof(l_eqiupmentcode));
	datalen += sizeof(l_eqiupmentcode);
	
	printfcom1("接收的数据类型 %x",l_eqiupmentcode);
	
	if(l_eqiupmentcode != 0)
	{
		switch (l_eqiupmentcode)
		{
			unsigned int startnum=0,endnum=0xffffffff;
			short  density; 
			stcTime		sTime;   
			short   hig; 
			char	modelnum;
			unsigned short locotype,loconum; 
			unsigned int  i;
			

			case DATA_CARD:  
							  printfcom1("\r\n DATA_CARD 准备发送数据 ");
							  memcpy((unsigned char *)&startnum,&buf[datalen],sizeof(startnum));
							  datalen += sizeof(startnum);
							  memcpy((unsigned char *)&endnum,&buf[datalen],sizeof(endnum));  
							  
							  EquipmentReadData(startnum,endnum);								//
								
							  break;    
			case DENSITY_CARD: 
				              printfcom1("\r\n DENSITY_CARD ");  
							  memcpy((unsigned char *)&density,&buf[datalen],sizeof(density));
							  
							  printfcom1("\r\n density %d  数据长度 %d datalen %d  buf %x buf++ %x",density,sizeof(density),datalen,buf[datalen],buf[datalen+1]);  
							  break;    
			case MODEL_CARD:  
							  //printfcom1("\r\n MODEL_CARD "); 
							  
	
							  memcpy((unsigned char *)&modeldatanum,&buf[datalen],sizeof(modeldatanum));
							  datalen += sizeof(modeldatanum);
							 
							  printfcom1("\r\n modeldatanum %d",modeldatanum);
							  
							  if(modeldatanum == 0)
							  {
								   pmodel = (unsigned char *)&m_sModelCard;
								   printfcom1("\r\n 指针设值 pmodel %x",pmodel);
								   l_modelcardflg = 1;
							  }
							  
							  
							  memcpy(pmodel,&buf[datalen],128<(Len-datalen)?128:(Len-datalen));
							  pmodel += 128<(Len-datalen)?128:(Len-datalen);

							  if(modeldatanum == 0xffffffff)
							  {
								l_modelcardflg = 0;
								
								printfcom1("\r\n 模型结束 sModel 点数，编号 %d,%d",m_sModelCard.sModelSemple[1].PotNum,m_sModelCard.sModelSemple[1].ModelNum );
								for(i = 0; i< m_sModelCard.sModelSemple[1].PotNum;i++)
								{
									printfcom1("\r\n 高度，油量 %d,%d",m_sModelCard.sModelSemple[1].sModPot[i].HigVal,m_sModelCard.sModelSemple[1].sModPot[i].DipVal ); 
								}

							  }
	
							  
							  break;   
			case TIME_CARD:   
							  printfcom1("\r\n TIME_CARD ");
							  
							  memcpy((unsigned char *)&sTime,&buf[datalen],sizeof(sTime));
							  
							  printfcom1("\r\n 时间：%d-%d-%d %d-%d-%d",sTime.Year,sTime.Month,sTime.Date,sTime.Hour,sTime.Min,sTime.Sec);
							  
							  break; 
			case HIGHT_CARD:  
							  printfcom1("\r\n HIGHT_CARD ");
							  
							  memcpy((unsigned char *)&hig,&buf[datalen],sizeof(hig));

							  printfcom1("\r\n 高度：%d",hig);
							  
							  break; 
			case COPY_CARD:  	
							  printfcom1("\r\n COPY_CARD ");
							  
							  
						      break; 
			case MODEL_SELECT_CARD:
							  printfcom1("\r\n MODEL_SELECT_CARD "); 
							  
							  memcpy((unsigned char *)&modelnum,&buf[datalen],sizeof(modelnum));

							  printfcom1("\r\n 选模：%d",modelnum);

							  break; 
			case FIX_CARD:  
							  printfcom1("\r\n FIX_CARD ");

							  memcpy((unsigned char *)&locotype,&buf[datalen],sizeof(locotype));
							  datalen += sizeof(locotype);
							 
							  memcpy((unsigned char *)&loconum,&buf[datalen],sizeof(loconum));
							  
							  printfcom1("\r\n 机车类型：%d，机车编号：%d",locotype,loconum);
							  
				     		  break; 
			case COPY_FRAM_CARD:  
							  printfcom1("\r\n COPY_FRAM_CARD "); 
							  break; 
			case EXPAND_CARD:  	
							  printfcom1("\r\n EXPAND_CARD "); 
							  break; 
			default:
				   printfcom1("\r\n default "); 
		}
		
		l_eqiupmentcode = 0;
	}
}

unsigned char GetRecaddr()
{
	return	l_befaddr;
}

void  CSNR_RecDeal(void)     									//主机
{
	unsigned char		RecBuf[200] = {0};						//接收缓冲区
	unsigned char		DataBuf[200] = {0};						//
	static	unsigned int		Time = 0;
	unsigned int	i;

	unsigned int		RecLen;		
	unsigned int		DataLen = 16;		
			
			
	if( GetFramRecEndFlg() == 1 && sentendflg == 1)				//有新的帧尾，进行数据处理
	{
		sentendflg = 0;   										//发送指令后，等待接收
		RecLen 	= ReadRs485Data(RecBuf);
		
		ClearFramRecEndFlg();									//清标志，可重新取数
		
		l_recFlg = 1;

		l_befaddr = CSNR_GetData(RecBuf,RecLen,DataBuf,(unsigned char*)&DataLen); 
		
		printfcom1("\r\n reclen! %d l_befaddr %x",RecLen,l_befaddr);

		switch(l_befaddr)
		{
			case 0xCA: 
					   printfcom1("\r\n pc端处理！");				//PC端处理

					   CarddataOperate(DataBuf,DataLen); 			//读卡器对接收的信息进行处理 
					   
				break;
				
			case 0xC1: printfcom1("\r\n 无线处理！");				//无线模块
			
				break;
				
			case 0xC2: printfcom1("\r\n 数据读取装置处理！");		//数据读取装置
				
				break;
				
			case 0xC3: printfcom1("\r\n 智能电量处理！");			//智能电量
				
				break;	
				
			case 0x80: printfcom1("\r\n 主机信息！%d",DataLen);	//智能电量
			
				break;	
				
			default: ;
																	//错误信息
		}
	}
}


unsigned int GetHostBuf(unsigned char *Buf)
{
	memcpy(Buf,(unsigned char *)&g_cardsta,sizeof(g_cardsta));
	printfcom1("len %d,g_cardsta %d",sizeof(g_cardsta),g_cardsta); 
	return 	sizeof(g_cardsta);
}

void	HostStationSend_CSNR(void)
{
	unsigned char HostBuf[64];
	unsigned int  Len;
	unsigned char addr;
	
	Len = GetHostBuf(HostBuf);
	
	addr = 0xCF;
	
	DataSend_CSNR(0x80,addr,HostBuf,Len);	
}

void HostScan(void)
{
	static	unsigned char 	tmp = 0;
	static	unsigned int 	time = 0;
	//unsigned char 		 	ScanAddrList[9]={0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xCA};
	unsigned char 		 	ScanAddrList[9]={0xCA,0xCA,0xCA,0xCA,0xCA,0xCA,0xCA,0xCA,0xCA};
	unsigned char           addr;
	
	if(GetSysTime() - time > 100 || GetRecFlg())
	{
		if((tmp %10) == 9) 
		{
			HostStationSend_CSNR();
		}
		else
		{
			DataSend_CSNR(0x80,ScanAddrList[tmp%10],(unsigned char *)&l_befaddr,sizeof(l_befaddr));
		}

		tmp++;
		sentendflg = 1;
		l_recFlg = 0; 
		time 	 = GetSysTime(); 
	}
}




void  CSNR_Test(void)   
{
	if(GetOpenCom0Flg())
	 {
		 if(GetModelCardFlg() == 0) 
		 {
			 HostScan();   
		 }
		 else
		 {
			 sentendflg = 1;
		 }
		 
		 CSNR_RecDeal();
		 DelayX10ms(1);
	 }
}




