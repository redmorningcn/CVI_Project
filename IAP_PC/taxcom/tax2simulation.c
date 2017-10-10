#include <ansi_c.h>
#include "includes.h"  

/*
//TAXII前-------------------------------------------------------------------------------------
typedef struct _stcTAX2Bef_     //定义新TAX2箱后部分的数据结构 (32)
{
    unsigned char   TAX2BefAdrCod;      //本板地址(38H)     1   指通讯记录单元的地址，定为38H
    unsigned char   CharacterCod;       //特征码            1   无,暂填0
    unsigned char  	NewFlg;      		//标志     			1   新协议:67H
    unsigned char   SoftVerNum;      	//软件版本号      	1   
 	
	unsigned char   Reserve0;      		//预留0       			1   
    unsigned char   E_StationNum;       //车站号扩展字节    1   车站号扩展字节
    unsigned char   TrainTyp[4];        //车次种类          4   车次种类,车次编码新方案(铁运[2005]72号文件)
    														//						的车次种类标识符,定义与LKJ2000格式相同 
    unsigned char   E_MstDriverNum;     //司机号扩展字节    1     
    unsigned char   E_SlvDriverNum;     //副司机号扩展字节  1     
    
    unsigned char   Reserve1[2];      	//预留1 			2
    unsigned char   E_LocoTyp;          //机车型号扩展字节  1   机车型号扩充字节,定义与LKJ2000格式相同
    unsigned char   RelRoadNum;         //实际交路号        1   定义格式与LKJ2000相同   
    
    unsigned char   Reserve2[11];      	//预留2 			11
    unsigned char   Car_Truck;          //本/补、客/货      1   D0＝0/1 → 货/客   D1＝0/1 → 本/补
    
    unsigned char   TrainNum[3];        //车次数字部分      3   车次编码新方案(铁运[2005]72号文)的车次数据部分
    unsigned char   CheckSum;           //检查和            1   为前面所有字节累加和的二进制补码
}stcTAX2Bef;

//TAXII后----------------------------------------------------------------------------------
typedef struct _stcTAX2Bak_     //定义新TAX2箱后部分的数据结构
{
    unsigned char   TAX2BakAdrCod;      //本板地址(39H)     1   指通讯记录单元的地址，定为39H
    unsigned char   CharacterCod;       //特征码            1   高4位为3表上次接收成功，为C表
                                //                      上次接收失败，其它说明本串数据
                                //                      通讯过程中受干扰，应作无效数据处理。
                                //                      底四位暂为0
    unsigned char   DetectCellCod;      //检测单元代号      1   指将与通讯记录单元通讯的检测单元代号。
                                //                      定为01-轨道检测，
                                //                          02-弓网检测，03-TMIS，04-DMIS，
                                //                          06-语音录音，05、07-09预留给备用单元。
    unsigned char   Time[4];            //月/日/时/分/秒    4   D5~D0：秒，D11~D6：分，D16~D12：
                                //                          时D21~D17：日，D25~D22：月
    unsigned char   RelSpeed[3];        //实速              3   D9~D0：实速，D19~D10：预留
    unsigned char   LocoSign;           //机车信号          1   B4 = 0/1  单灯/多灯;   B3~B0 0-无灯,1-绿,2-黄, 
    unsigned char   LocoWorkState;      //机车工况          1   b0-零位,B1-向后,B2-向前,B3-制动,B4-牵引
    unsigned char   SignalNum[2];       //信号机编号        2   
    unsigned char   SignalTyp;          //信号机类型        1   B2-B0 :2-出站,3-进站,4-通过,5-预告,6-容许
                                //                  3   -双黄,4-红黄,5-红,6-白,7-绿黄,8-黄
    unsigned char   KmMark[3];   				//公里标    3   单位：米
    unsigned char   Weight[2];          //总重              2
    unsigned char   PlanLen[2];         //计长              2   单位：0.1
    unsigned char   VoitureCnt;         //辆数              1
    unsigned char   Car_Truck;          //本/补、客/货      1   D0＝0/1 → 货/客   D1＝0/1 → 本/补
    unsigned char   TrainNum[2];       	//车次              2
    unsigned char   RoadNum;            //区段号(交路号)    1
    unsigned char   StationNum;         //车站号            1
    unsigned char   MstDriverNum[2];    //司机号            2
    unsigned char   SlvDriverNum[2];    //副司机号          2
    unsigned char   LocoNum[2];         //机车号            2
    unsigned char   LocoTyp;            //机车型号          1   暂未定义
    unsigned char   LocoPipePress[2];   //列车管压          2   B9-B0:管压(单位:1kPa)    
    unsigned char   LocoState;          //装置状态          1   B0 = 1/0 - 降级/监控; B2 = 1/0 - 调车/非调车
    unsigned char   Reserve1;           //预留0 			1
    unsigned char   CheckSum;           //检查和            1   为前面所有字节累加和的二进制补码
}stcTAX2Bak;


typedef struct _stcTAX2Full_  	//定义新TAX2数据结构
{
	stcTAX2Bef	sTAX2Bef;		//TAX2前部分			
	stcTAX2Bak	sTAX2Bak;		//TAX2后部分
}stcTAX2Full;

*/
void     ReadTime(stcTime * sTime)
{
	struct tm *ptr;
    time_t lt; 
	
    lt=time(NULL);
    ptr=localtime((time_t*)&lt);
	
	
	sTime->Sec = ptr->tm_sec;
	sTime->Min = ptr->tm_min;
	sTime->Hour = ptr->tm_hour;	
	sTime->Date = ptr->tm_mday;	  	
	sTime->Month = ptr->tm_mon+1;
	sTime->Year = ptr->tm_year-100;	
//	printf("%d-%d-%d %d:%d:%d\r\n",sTime->Year ,sTime->Month,sTime->Date,sTime->Hour,sTime->Min,sTime->Sec);
}

void  ReadSimsTaxData(stcTAX2Full *sTax)
{
    unsigned char   	CheckSum;        		//检查和            1   为前面所有字节累加和的二进制补码  
	stcTime		sTime;
	unsigned int		RealSpeed;
	unsigned int		Time;
	unsigned int		KmMark;
	unsigned short		LocoNum;
	unsigned short		MstDriverNum;
	
	sTax->sTAX2Bef.TAX2BefAdrCod 	= 0x38;
	sTax->sTAX2Bef.NewFlg			= 0x67;
	sTax->sTAX2Bef.CheckSum = GetCheckSum((unsigned char *)&sTax->sTAX2Bef,sizeof(sTax->sTAX2Bef) -1);

    sTax->sTAX2Bak.TAX2BakAdrCod = 0x39;

    ReadTime((stcTime *)&sTime);											// 时间 
	
	Time	=  sTime.Sec + sTime.Min * (1 << 6) + sTime.Hour * (1 << 12) 
				+ sTime.Date * (1 << 17) + sTime.Month * (1 << 22) + sTime.Year * (1 << 26);
	memcpy(sTax->sTAX2Bak.Time,(unsigned char *)&Time,sizeof(Time));
	
//	printf("sTime  %d  %d\r\n",sTax->sTAX2Bak.Time,Time);  
	
	RealSpeed = 45;
//    RealSpeed	= GetSetTaxSpeed();		// 取TAX箱信息]
	
	memcpy((unsigned char *)&sTax->sTAX2Bak.RelSpeed,(unsigned char *)&RealSpeed,sizeof(sTax->sTAX2Bak.RelSpeed));

	//KmMark	= RealSpeed * ((GetSysTime()/10)/36);  	//((GetSysTime()/100)/3600) 单位：m
	KmMark	= 0xffffffff;  	//((GetSysTime()/100)/3600) 单位：m  
	memcpy((unsigned char *)&sTax->sTAX2Bak.KmMark,(unsigned char *)&KmMark,sizeof(sTax->sTAX2Bak.KmMark));

    //sTax->sTAX2Bak.LocoTyp						=  GetSetTaxLocoTyp();		// 取TAX箱机车型号    
    sTax->sTAX2Bak.LocoTyp						=  141;		// 取TAX箱机车型号  
      
	//LocoNum	=  GetSetTaxLocoNum();					// 取TAX箱机车编号
	LocoNum	=  314;					// 取TAX箱机车编号
	memcpy((unsigned char *)&sTax->sTAX2Bak.LocoNum,(unsigned char *)&LocoNum,sizeof(sTax->sTAX2Bak.LocoNum));

//	MstDriverNum	=  GetSetTaxDriverNum();		// 取TAX箱司机号
	MstDriverNum = 251;
	memcpy((unsigned char *)&sTax->sTAX2Bak.MstDriverNum,(unsigned char *)&MstDriverNum,sizeof(sTax->sTAX2Bak.MstDriverNum));
													// 公里标

	sTax->sTAX2Bak.CheckSum = GetCheckSum((unsigned char *)&sTax->sTAX2Bak,sizeof(sTax->sTAX2Bak) -1);
}


void	SendSimsTaxData(stcTAX2Full * sTax)			//发送TAX数据
{
	SendCom3((uint8 *)sTax,sizeof(stcTAX2Full));	//发送TAX信息
}

//--------------------------------------------------------------------------------------------------
void  SimulateTax(void)
{
	stcTAX2Full		sTax;
	static	uint32	Time;
	time_t t;  
	
	if(GetOpenCom3Flg())	
	{  
		ClearRecCom3EndFlg(); 
		
		if(GetSysTime() - Time > 10)				//20ms
		{
			ReadSimsTaxData((stcTAX2Full *)&sTax);		//读TAX箱模拟器数据

			SendSimsTaxData((stcTAX2Full *)&sTax);		//发送TAX数据
		
			Time = GetSysTime();
		}
	}
}
