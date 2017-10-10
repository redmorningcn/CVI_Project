#ifndef  _CardDataTypeDef_h_
#define  _CardDataTypeDef_h_

typedef struct _stcFixInfo    //设备身份信息 	        //出产时设置 28
{
    unsigned char   LocoTyp;            //机车型号          1   
    unsigned char   E_LocoTyp;          //机车型号扩展字节  1   机车型号扩充字节,定义与LKJ2000格式相同
    unsigned char   LocoNum[2];         //机车号            2
	unsigned char   FixDate[6];         //初装日期			6	"20090303"
	unsigned char   FixUint[4];         //初装单位			4	"GTZJ"	
	unsigned char   ReFixDate[6];       //改装日期			6	"20190303"   
	unsigned char   ReFixUint[4];      	//改装单位			4	"GTGJ"	
    unsigned short 	Reserve0;   		//预留				2
	unsigned short  CrcCheck;           //校验和            2   
}stcFixInfo;

//时间----------------------------------------
typedef		struct	_stcTime_
{
	unsigned char	Sec;
	unsigned char	Min;
	unsigned char	Hour;
	unsigned char	Date;
	unsigned char	Month;
	unsigned char	Year;
	unsigned short	CrcCheck;
}stcTime;

typedef struct _stcCardID_						//IC 卡ID号	  32
{
	unsigned char	ProductNum[12];						//IC卡编号
	unsigned char	DriverNum[4];						//司机代码
	unsigned char	DriverUnit[8];						//司机所在单位编号
	unsigned char	Reserve0[4];						//预留0
	unsigned short	Reserve1;	 						//预留1
	unsigned short	CrcCheck;							//CRC检验
}stcCardID;

typedef struct _stcCardType_					//IC 卡类型 8
{
	unsigned short	CardType[3];						//IC卡类型
	unsigned short	CrcCheck;							//CRC检验
}stcCardType;

typedef struct _stcDensity_						//IC 卡密度 24
{
	unsigned char		LocoType[2];					//机车型号
	unsigned char		LocoNum[2];						//机车编号
	//unsigned char		DriverNum[4];					//司机编号
	unsigned int		DenChageTimes;					//密度变化号
	unsigned int		Density;						//加油密度
	unsigned int		AddOilStationNum;				//加油机编号
	unsigned int		AddOilerNum;					//加油人员编号	
	unsigned short		AddVol;							//加油油量
	unsigned short		CrcCheck;						//CRC检验
}stcDensity;

typedef struct _stcModPot_						//IC 卡cd参数
{
	unsigned short	HigVal;								//压力值
	unsigned short	DipVal;								//油量值
}stcModPot;

typedef struct _stcModel_						//油箱模型复杂数据结构
{
	stcModPot	sModPot[50];					//计算模型
	unsigned short		PotNum;							//计算模型点的个数	
	unsigned short		StartHight;						//起始高度
	unsigned char		ModelNum;						//模型编号
	unsigned char		Reserve0;						//预留    //100919   0x01表示使用，0x00表示未用
	unsigned short		CrcCheck;						//CRC校验
}stcModel;

typedef struct _stcModelSemple_					//油箱模型简化数据结构
{
	stcModPot	sModPot[10];					//计算模型
	unsigned short		PotNum;							//计算模型点的个数	
	unsigned short		StartHight;						//起始高度
	unsigned char		ModelNum;						//模型编号
	unsigned char		Reserve0;						//预留	//100919	0x01表示使用，0x00表示未用	
	unsigned short		CrcCheck;						//CRC校验
}stcModelSemple;

typedef struct _stcModelComplex_				//油箱模型简化数据结构
{
	stcModPot	sModPot[200];					//计算模型
	unsigned short		PotNum;							//计算模型点的个数	
	unsigned short		StartHight;						//起始高度
	unsigned char		ModelNum;						//模型编号
	unsigned char		Reserve0;						//预留
	unsigned short		CrcCheck;						//CRC校验
}stcModelComplex;

typedef struct _stcModelCard_					//油箱模型复杂数据结构
{
	stcModel		sModel[18];					//复杂油箱模型
	stcModelSemple	sModelSemple[46];			//简单油箱模型
	unsigned short			Reserve0;					//预留
	unsigned short			CrcCheck;					//CRC校验	
}stcModelCard;

typedef struct _stcReviseHight_					//IC 修正高度
{
	short		ReviseHight;					//修正高度起始高度
	unsigned short		CrcCheck;						//CRC校验
}stcReviseHight;

typedef struct _stcAddDipVal_					//IC 油量增加
{
	short		AddDipVal;						//油量增加
	unsigned short		CrcCheck;						//CRC校验
}stcAddDipVal;

typedef struct _stcDebugCard_					//
{
	unsigned short		DebugCode;						//调试代码
	unsigned short		BufLen;							//有效数据长度
	unsigned char		Buf[100];						//数据区
	unsigned short		Reserve0;						//预留
	unsigned short		CrcCheck;						//CRC校验
}stcDebugCard;

//计算模型卡参数
typedef struct _stcCardPara_ 					//IC 卡cd参数 352
{
	stcDensity		sDensity;					//密度
	unsigned int			SetVal;						//设置值
	stcTime			sTime;						//时间
 	stcReviseHight	sReviseHight;				//修正起始高度
	stcFixInfo      sFixInfo;              	 	//装车卡
	stcDebugCard	sDebugCard;					//调试卡
	unsigned short			Reserve0;					//预留
	unsigned short			CrcCheck;					//CRC检验
}stcCardPara;

typedef struct _stcCardIndex_					//IC 卡文件索引 36
{
	unsigned char			LocoType[2];				//机车型号
	unsigned char			LocoNum[2];					//机车编号
	unsigned char			ProNum[16];					//产品编号
	unsigned int			StartAddr;					//开始地址
	unsigned int			EndAddr;					//结束地址
	unsigned short			RecLen;						//记录长度
	unsigned short			RecNum;						//记录条数	
}stcCardIndex;

typedef struct _stcCardFlg_						//IC 卡标识
{
	stcCardID		sCardID;					//IC卡 ID结构
	stcCardType		sCardType;					//IC卡 IC卡类型（数据卡，密度卡，）
	stcCardPara		sCardPara;					//IC卡 卡参数
	stcCardIndex	sCardIndex[100];			//IC卡 文件索引
	unsigned short			WriteCardTimes;				//已写卡次数
	unsigned short			CrcCheck;					//CRC检验
}stcCardFlg;


#endif
