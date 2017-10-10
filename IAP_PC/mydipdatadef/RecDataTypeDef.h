#ifndef  _RecDataTypeDef_h_
#define  _RecDataTypeDef_h_

//TAXII前-------------------------------------------------------------------------------------
typedef struct _stcTAX2Bef_     //定义新TAX2箱后部分的数据结构 (32)
{
    uint8   TAX2BefAdrCod;      //本板地址(38H)     1   指通讯记录单元的地址，定为38H
    uint8   CharacterCod;       //特征码            1   无,暂填0
    uint8  	NewFlg;      		//标志     			1   新协议:67H
    uint8   SoftVerNum;      	//软件版本号      	1   
 	
	uint8   Reserve0;      		//预留0       			1   
    uint8   E_StationNum;       //车站号扩展字节    1   车站号扩展字节
    uint8   TrainTyp[4];        //车次种类          4   车次种类,车次编码新方案(铁运[2005]72号文件)
    														//						的车次种类标识符,定义与LKJ2000格式相同 
    uint8   E_MstDriverNum;     //司机号扩展字节    1     
    uint8   E_SlvDriverNum;     //副司机号扩展字节  1     
    
    uint8   Reserve1[2];      	//预留1 			2
    uint8   E_LocoTyp;          //机车型号扩展字节  1   机车型号扩充字节,定义与LKJ2000格式相同
    uint8   RelRoadNum;         //实际交路号        1   定义格式与LKJ2000相同   
    
    uint8   Reserve2[11];      	//预留2 			11
    uint8   Car_Truck;          //本/补、客/货      1   D0＝0/1 → 货/客   D1＝0/1 → 本/补
    
    uint8   TrainNum[3];        //车次数字部分      3   车次编码新方案(铁运[2005]72号文)的车次数据部分
    uint8   CheckSum;           //检查和            1   为前面所有字节累加和的二进制补码
}stcTAX2Bef;

//TAXII后----------------------------------------------------------------------------------
typedef struct _stcTAX2Bak_     //定义新TAX2箱后部分的数据结构
{
    uint8   TAX2BakAdrCod;      //本板地址(39H)     1   指通讯记录单元的地址，定为39H
    uint8   CharacterCod;       //特征码            1   高4位为3表上次接收成功，为C表
                                //                      上次接收失败，其它说明本串数据
                                //                      通讯过程中受干扰，应作无效数据处理。
                                //                      底四位暂为0
    uint8   DetectCellCod;      //检测单元代号      1   指将与通讯记录单元通讯的检测单元代号。
                                //                      定为01-轨道检测，
                                //                          02-弓网检测，03-TMIS，04-DMIS，
                                //                          06-语音录音，05、07-09预留给备用单元。
    uint8   Time[4];            //月/日/时/分/秒    4   D5~D0：秒，D11~D6：分，D16~D12：
                                //                          时D21~D17：日，D25~D22：月
    uint8   RelSpeed[3];        //实速              3   D9~D0：实速，D19~D10：预留
    uint8   LocoSign;           //机车信号          1   B4 = 0/1  单灯/多灯;   B3~B0 0-无灯,1-绿,2-黄, 
    uint8   LocoWorkState;      //机车工况          1   b0-零位,B1-向后,B2-向前,B3-制动,B4-牵引
    uint8   SignalNum[2];       //信号机编号        2   
    uint8   SignalTyp;          //信号机类型        1   B2-B0 :2-出站,3-进站,4-通过,5-预告,6-容许
                                //                  3   -双黄,4-红黄,5-红,6-白,7-绿黄,8-黄
    uint8   KmMark[3];   				//公里标    3   单位：米
    uint8   Weight[2];          //总重              2
    uint8   PlanLen[2];         //计长              2   单位：0.1
    uint8   VoitureCnt;         //辆数              1
    uint8   Car_Truck;          //本/补、客/货      1   D0＝0/1 → 货/客   D1＝0/1 → 本/补
    uint8   TrainNum[2];       	//车次              2
    uint8   RoadNum;            //区段号(交路号)    1
    uint8   StationNum;         //车站号            1
    uint8   MstDriverNum[2];    //司机号            2
    uint8   SlvDriverNum[2];    //副司机号          2
    uint8   LocoNum[2];         //机车号            2
    uint8   LocoTyp;            //机车型号          1   暂未定义
    uint8   LocoPipePress[2];   //列车管压          2   B9-B0:管压(单位:1kPa)    
    uint8   LocoState;          //装置状态          1   B0 = 1/0 - 降级/监控; B2 = 1/0 - 调车/非调车
    uint8   Reserve1;           //预留0 						1
    uint8   CheckSum;           //检查和            1   为前面所有字节累加和的二进制补码
}stcTAX2Bak;

typedef struct _stcTAX2Full_  	//定义新TAX2数据结构
{
	stcTAX2Bef	sTAX2Bef;		//TAX2前部分			
	stcTAX2Bak	sTAX2Bak;		//TAX2后部分
}stcTAX2Full;

typedef struct _stcFlshRec   	//定义油尺记录数据结构
{
	uint32	StoreCnt;         	//存储流水号    		4   追踪记录用

	uint8	RecTypCod;       	//记录类型          	1   数据类型（开机、上班卡、下班卡、数据记录）
	uint8	RelSpeed[3];      	//实速              	3   D9~D0：实速，D19~D10：预留

	uint8	Time[4];          	//月/日/时/分/秒      	4   D5~D0:秒,D11~D6:分,D16~D12:时D21~D17：日，
	                               	//                      	D25~D22：月 ;D26~D31:年
	uint8	LocoTyp;          	//机车型号          	1   
	uint8	E_LocoTyp;        	//机车型号扩展字节  	1   机车型号扩充字节,定义与LKJ2000格式相同
	uint8 	LocoNum[2];       	//机车号            	2	

	uint8 	TrainTyp[4];     	//车次种类          	4   车次种类,车次编码新方案(铁运[2005]72号文件)
	                             	//                      	的车次种类标识符,定义与LKJ2000格式相同
	                             	
	uint8	TrainNum[3];     	//车次数字部分      	3   车次编码新方案(铁运[2005]72号文)的车次数据部分
	uint8 	Car_Truck;       	//本/补、客/货       	1   D0＝0/1 → 货/客   D1＝0/1 → 本/补   
	 
	uint8 	VoitureCnt;       	//辆数              	1	
	uint8 	KmMark[3];   		//公里标            	3   单位：米

	uint8 	Weight[2];       	//总重              	2
	uint8 	PlanLen[2];       	//计长              	2   单位：0.1   

	uint8 	MstDriverNum[2];  	//司机号            	2	
	uint8 	E_MstDriverNum; 	//司机号扩展字节    	1   
	uint8 	E_SlvDriverNum;   	//副司机号扩展字节  	1   

	uint8 	SlvDriverNum[2];  	//副司机号          	2   
	uint8	DriverUnitNum[2];  	//司机单位编号      	2   //用来唯一确定司机

	uint8 	RoadNum;          	//区段号(交路号)     	1	
	uint8  	RelRoadNum;     	//实际交路号         	1   定义格式与LKJ2000相同   
	uint8  	StationNum;     	//车站号            	1
	uint8  	E_StationNum;   	//车站号扩展字节    	1   车站号扩展字节

	uint8 	SignalTyp;        	//信号机类型        	1   B2-B0 :2-出站,3-进站,4-通过,5-预告,6-容许
	uint8 	LocoSign;        	//机车信号          	1   B4 = 0/1  单灯/多灯;   B3~B0 0-无灯,1-绿,2-黄, 
	                            	//                      	3-双黄,4-红黄,5-红,6-白,7-绿黄,8-黄
	uint8 	LocoWorkState;    	//机车工况          	1   b0-零位,B1-向后,B2-向前,B3-制动,B4-牵引
	uint8 	LocoState;      	//装置状态          	1   B0 = 1/0 - 降级/监控; B2 = 1/0 - 调车/非调车

	uint8 	SignalNum[2];      	//信号机编号        	2   
	uint8 	LocoPipePress[2]; 	//列车管压          	2   B9-B0:管压(单位:1kPa)
	  
	uint32 	MstDip;           	//主机油量          	4   主油尺油量(主从机级联时为主机油量,否则为本机车油量)
	uint32 	SlvDip;           	//从机油量          	4   主油尺油量(主从机级联时为从机油量,否则此油量值为空)
	int32 	MstLocoPower;       //主机机车功率       	4   主机外接设备测量
	int32 	SlvLocoPower;     	//从机机车功率       	4   从机外接设备测量	
	//090901
 	uint16	MstDip1Prs;        	//压强            		2   主机油尺1压强值
	uint16	MstDip2Prs;        	//压强            		2   主机油尺2压强值
	
	uint16	SlvDip1Prs;       	//压强            		2   从机油尺1压强值
	uint16 	SlvDip2Prs;       	//压强            		2   从机油尺2压强值
	
	uint8  	MyYear;           	//年      				1   装置时间的年
	uint8 	MyMonth;         	//月      				1   装置时间的月
	uint8 	MyData;          	//日      				1   装置时间的日
	uint8 	MyHour;           	//时      				1   装置时间的时
	
	uint8 	MyMinute;     		//分      				1   装置时间的分
	uint8 	MySecond;       	//秒       				1   装置时间的秒
	uint16	MstDensity;			//密度					2	主机燃油密度
	
	uint16	SlvDensity;			//密度					2	从机燃油密度
	uint16	GpsHeight;			//高度					2	地理高度
	
	int32	GpsLongitude;		//经度					4	地理经度
	int32	GpsLatitude;		//纬度					4	地理纬度
		
	int16	MstEngRotSpd;		//主机柴油机转速		2	主机柴油机转速
	int16	SlvEngRotSpd;		//从机柴油机转速		2	从机柴油机转速
	//补充内容 
	//与自加时钟芯片配套的数据有
	uint16 	JcRealType;			//机车型号				2   外部设置的机车型号
	uint16 	JcRealNo;			//机车编号				2   外部设置的机车编号
	
	uint16 	MstPotPress;		//主机缸压				2	预留接监控	
	uint16 	SlvPotPress;		//从机缸压				2	预留接监控	
	
	uint16 	MstPot1;			//主机均缸1				2	预留接监控	
	uint16 	MstPot2;			//主机均缸2				2	预留接监控
	
	uint16 	SlvPot1;			//从机均缸1				2	预留接监控		
	uint16 	SlvPot2;			//从机均缸2				2	预留接监控
	
	uint16 	MaxSpeed;			//限速					2	预留接监控	
	uint16 	MstOcurrent;		//主机原边电流   		2	预留接监控	
	
	uint8	ErrorCode;			//故障代码				1   0~255
	uint8 	DipValChgState;  	//油量变化状态   		1   油量变化情况分：两油尺增加/减小、变化平稳、轻微波动、剧烈波动
	//090901
	uint8	MstDipTemp;			//温度					1	主机温度
	uint8	OilBoxModel;		//油箱模型				1	机车油箱代码
	
	uint8	SoftVes;        	//软件版本          	1   
	uint8	Reserve1;        	//预留          		1  	
//	uint16	Reserve0;        	//预留          		2   
	uint16 	CrcCheck;         	//存储的校验和    		2   Flash记录存储CRC校验
}stcFlshRec;


typedef struct _stcLog_							//Fram日志索引   12
{
	uint16			DriveNum;					//设备编号	   2 
	uint8			LocoTyp[2];					//机车型号     2
	uint8			LocoNum[2];					//机车编号     2
	uint8			ErrorCode;					//故障代码     1
	uint8			RecTypCod;					//记录类型	   1
	uint32			StoreCnt;					//流水号       4
}stcLog;

typedef struct _stcLogZone_			//Fram日志区间
{
	stcLog			sLog[100];
	uint16			Times;						//预留
	uint16			CrcCheck;					//CRC检验
}stcLogZone;



#endif
