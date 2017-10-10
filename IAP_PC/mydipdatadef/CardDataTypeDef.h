#ifndef  _CardDataTypeDef_h_
#define  _CardDataTypeDef_h_

//时间----------------------------------------
typedef		struct	_stcTime_
{
	uint8	Sec;
	uint8	Min;
	uint8	Hour;
	uint8	Date;
	uint8	Month;
	uint8	Year;
	uint16	CrcCheck;
}stcTime;

typedef struct _stcCardID_						//IC 卡ID号	  32
{
	uint8	ProductNum[12];						//IC卡编号
	uint8	DriverNum[4];						//司机代码
	uint8	DriverUnit[8];						//司机所在单位编号
	uint8	Reserve0[4];						//预留0
	uint16	Reserve1;	 						//预留1
	uint16	CrcCheck;							//CRC检验
}stcCardID;

typedef struct _stcCardType_					//IC 卡类型 8
{
	uint16	CardType[3];						//IC卡类型
	uint16	CrcCheck;							//CRC检验
}stcCardType;

typedef struct _stcDensity_						//IC 卡密度 24
{
	uint8		LocoType[2];					//机车型号
	uint8		LocoNum[2];						//机车编号
	//uint8		DriverNum[4];					//司机编号
	uint32		DenChageTimes;					//密度变化号
	uint32		Density;						//加油密度
	uint32		AddOilStationNum;				//加油机编号
	uint32		AddOilerNum;					//加油人员编号	
	uint16		AddVol;							//加油油量
	uint16		CrcCheck;						//CRC检验
}stcDensity;

typedef struct	 _stcModPot_						//IC 卡cd参数
{
	uint16	HigVal;								//压力值
	uint16	DipVal;								//油量值
}stcModPot;

typedef struct _stcModel_						//油箱模型复杂数据结构
{
	stcModPot	sModPot[50];					//计算模型
	uint16		PotNum;							//计算模型点的个数	
	uint16		StartHight;						//起始高度
	uint8		ModelNum;						//模型编号
	uint8		Reserve0;						//预留    //100919   0x01表示使用，0x00表示未用
	uint16		CrcCheck;						//CRC校验
}stcModel;

typedef struct _stcModelSemple_					//油箱模型简化数据结构
{
	stcModPot	sModPot[10];					//计算模型
	uint16		PotNum;							//计算模型点的个数	
	uint16		StartHight;						//起始高度
	uint8		ModelNum;						//模型编号
	uint8		Reserve0;						//预留	//100919	0x01表示使用，0x00表示未用	
	uint16		CrcCheck;						//CRC校验
}stcModelSemple;

typedef struct _stcModelComplex_				//油箱模型简化数据结构
{
	stcModPot	sModPot[200];					//计算模型
	uint16		PotNum;							//计算模型点的个数	
	uint16		StartHight;						//起始高度
	uint8		ModelNum;						//模型编号
	uint8		Reserve0;						//预留
	uint16		CrcCheck;						//CRC校验
}stcModelComplex;

typedef struct _stcModelCard_					//油箱模型复杂数据结构
{
	stcModel		sModel[18];					//复杂油箱模型
	stcModelSemple	sModelSemple[46];			//简单油箱模型
	uint16			Reserve0;					//预留
	uint16			CrcCheck;					//CRC校验	
}stcModelCard;

typedef struct _stcReviseHight_					//IC 修正高度
{
	int16		ReviseHight;					//修正高度起始高度
	uint16		CrcCheck;						//CRC校验
}stcReviseHight;

typedef struct _stcAddDipVal_					//IC 油量增加
{
	int16		AddDipVal;						//油量增加
	uint16		CrcCheck;						//CRC校验
}stcAddDipVal;

typedef struct _stcDebugCard_					//
{
	uint16		DebugCode;						//调试代码
	uint16		BufLen;							//有效数据长度
	uint8		Buf[100];						//数据区
	uint16		Reserve0;						//预留
	uint16		CrcCheck;						//CRC校验
}stcDebugCard;

//计算模型卡参数
typedef struct _stcCardPara_ 					//IC 卡cd参数 352
{
	stcDensity		sDensity;					//密度
	uint32			SetVal;						//设置值
	stcTime			sTime;						//时间
 	stcReviseHight	sReviseHight;				//修正起始高度
	stcFixInfo      sFixInfo;              	 	//装车卡
	stcDebugCard	sDebugCard;					//调试卡
	uint16			Reserve0;					//预留
	uint16			CrcCheck;					//CRC检验
}stcCardPara;

typedef struct _stcCardIndex_					//IC 卡文件索引 36
{
	uint8			LocoType[2];				//机车型号
	uint8			LocoNum[2];					//机车编号
	uint8			ProNum[16];					//产品编号
	uint32			StartAddr;					//开始地址
	uint32			EndAddr;					//结束地址
	uint16			RecLen;						//记录长度
	uint16			RecNum;						//记录条数	
}stcCardIndex;

typedef struct _stcCardFlg_						//IC 卡标识
{
	stcCardID		sCardID;					//IC卡 ID结构
	stcCardType		sCardType;					//IC卡 IC卡类型（数据卡，密度卡，）
	stcCardPara		sCardPara;					//IC卡 卡参数
	stcCardIndex	sCardIndex[100];			//IC卡 文件索引
	uint16			WriteCardTimes;				//已写卡次数
	uint16			CrcCheck;					//CRC检验
}stcCardFlg;


#endif
