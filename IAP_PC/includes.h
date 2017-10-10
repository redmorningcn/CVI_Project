#ifndef	INCLUDES_H_
#define	INCLUDES_H_   0

#include "ndpcom.h"   

#include	"FIFO.h"
#include	"CrcCheck.h" 


typedef	unsigned char  uint8;					/* 无符号8位整型变量						*/
typedef	    	 char  int8;					/* 有符号8位整型变量						*/
//typedef	unsigned char  uint8;				/* 无符号8位整型变量						*/
//typedef	signed	 char  int8;				/* 有符号8位整型变量						*/
typedef	unsigned short uint16;					/* 无符号16位整型变量						*/
typedef	signed	 short int16;					/* 有符号16位整型变量						*/
typedef	unsigned int   uint32;					/* 无符号32位整型变量						*/
typedef	signed	 int   int32;					/* 有符号32位整型变量						*/
typedef	float		   fp32;					/* 单精度浮点数（32位长度）					*/
typedef	double		   fp64;					/* 双精度浮点数（64位长度）					*/

typedef	unsigned char  UINT8;					/* 无符号8位整型变量						*/
typedef	signed	 char  INT8;					/* 有符号8位整型变量						*/
typedef	unsigned short UINT16;					/* 无符号16位整型变量						*/
typedef	signed	 short INT16;					/* 有符号16位整型变量						*/
typedef	unsigned int   UINT32;					/* 无符号32位整型变量						*/
typedef	signed	 int   INT32;					/* 有符号32位整型变量						*/
typedef	float		   FP32;					/* 单精度浮点数（32位长度）					*/
typedef	double		   FP64;					/* 双精度浮点数（64位长度）					*/

#include	"DataTypeDef.h"

int	g_dip1Prs,g_dip2Prs,g_dipval;
int	g_cardsta;

extern	int panelHandle;  

//cvicom0.c
extern	void 	OpenProgramCom0(int	comNum,int boaut);
extern	void 	CloseProgramCom0(void) ;
extern	int		SendCom0(unsigned char *Buf,int Len);
extern	void 	printfcom0(const char *format, ...);
extern	int		ReadCom0(unsigned char *Buf,int Len);
extern	unsigned char GetOpenCom0Flg(void);
extern	unsigned char GetRecCom0EndFlg(void);
extern	void	ClearRecCom0EndFlg(void);
extern	void 	printftextbox(const char *format, ...);   
//extern	int CVICALLBACK RefreshRecCom0Data (void *functionData);

//cvicom1.c
extern	void 	OpenProgramCom1(int	comNum,int boaut) ;
extern	void 	CloseProgramCom1(void) ;
extern	int		SendCom1(unsigned char *Buf,int Len);
extern	void 	printfcom1(const char *format, ...);
extern	int		ReadCom1(unsigned char *Buf,int Len);
extern	unsigned char GetOpenCom1Flg(void);
extern	unsigned char GetRecCom1EndFlg(void);
extern	void	ClearRecCom1EndFlg(void);
//extern	int CVICALLBACK RefreshRecCom1Data (void *functionData);

//cvicom2.c
extern	void 	OpenProgramCom2(int	comNum,int boaut) ;
extern	void 	CloseProgramCom2(void) ;
extern	int		SendCom2(unsigned char *Buf,int Len);
extern	void 	printfcom2(const char *format, ...);
extern	int		ReadCom2(unsigned char *Buf,int Len);
extern	unsigned char GetOpenCom2Flg(void);
extern	unsigned char GetRecCom2EndFlg(void);
extern	void	ClearRecCom2EndFlg(void);
//extern	int CVICALLBACK RefreshRecCom2Data (void *functionData);

//cvicom3.c
extern	void 	OpenProgramCom3(int	comNum,int boaut) ;
extern	void 	CloseProgramCom3(void) ;
extern	int		SendCom3(unsigned char *Buf,int Len);
extern	void 	printfcom3(const char *format, ...);
extern	int		ReadCom3(unsigned char *Buf,int Len);
extern	unsigned char GetOpenCom3Flg(void);
extern	unsigned char GetRecCom3EndFlg(void);
extern	void	ClearRecCom3EndFlg(void);
//extern	int CVICALLBACK RefreshRecCom3Data (void *functionData);

//cvicom4.c
extern	void 	OpenProgramCom4(int	comNum,int boaut) ;
extern	void 	CloseProgramCom4(void) ;
extern	int		SendCom4(unsigned char *Buf,int Len);
extern	void 	printfcom4(const char *format, ...);
extern	int		ReadCom4(unsigned char *Buf,int Len);
extern	unsigned char GetOpenCom4Flg(void);
extern	unsigned char GetRecCom4EndFlg(void);
extern	void	ClearRecCom4EndFlg(void);
//extern	int CVICALLBACK RefreshRecCom4Data (void *functionData);


//cvitime.c
extern  void	SysTickTask(void);
void	AsyncTimerStart(void);
extern	unsigned int GetSysTime(void);
extern	void	DelayX10ms(unsigned int Time); 

//cvifile.c
extern	void	GetHexFileDir(char * HexFileDir);
extern	unsigned char ReadFileLine(char *fileLineBuf); 
extern	unsigned char filetesttast(void);
extern	char  GetHexFileOpenFlg(void);



//cvitmpcomtext.c
extern	void  refreshComtext(int panel,int id);


//tax2simulation.c


extern	void  SimulateTax(void);
extern	void  ReadTime(stcTime * sTime)  ;
extern	void  Sendrs485(uint8	*DataBuf,uint32	DataLen)  ;


//dipcom.c
extern	void	SensorSimulator(void) ;
extern	void 	TestHostSent(void);
extern	uint8	GetDip1DataConFlg(void);     
extern	uint8	GetDip2DataConFlg(void); 


//computertime.c
extern	void	timetest(void );

//powcom.c
extern	void  	SimulatePow(void);
extern	uint8	GetPowerDataConFlg(void);

//cviled.c
extern  void	HoldLed(void);
extern  void 	OnLed(uint8 num,uint32 times,uint16	LightTime,uint16 unLightTime)  ;
extern	void 	OffLed(uint8 num);
#define		Led0Dip1	0
#define		Led1Dip2	1
#define		Led2Tax2	2
#define		Led3Exp1	3
#define		Led4Exp2	4
#define		Led5Run		5
#define		Led6Num		6
#define		Led7Num		7


//cviLedCtrl.c
extern	void LedTast(void);

//speed.c
extern	void	BroadsDist(void);
extern	uint8	GetSpeedDataConFlg(void);

//csncascom.c
extern	void ascomtest(void);

//settime.c
extern	void	SetRTCTime(void);

//csnr
extern	void  CSNR_Test(void); 
extern	void	DataSend_CSNR(unsigned char SourceAddr,unsigned char DistAddr,unsigned char	*DataBuf,unsigned int	DataLen);

//salveeqiupment.c
#define		DATA_CARD			0x0001			//数据卡
#define		DENSITY_CARD		0x0020			//密度卡
#define		MODEL_CARD			0x0040			//参数模型卡
#define		TIME_CARD			0x0100			//时间卡
#define		HIGHT_CARD			0x0200			//高度调整卡
#define		COPY_CARD			0x0400			//FLASH复制卡
#define		MODEL_SELECT_CARD	0x1000			//模型选择卡
#define		FIX_CARD			0x2000			//装车卡
#define		COPY_FRAM_CARD		0x4000			//铁电拷贝卡

#define		SET_DATA_CARD		0x0080			//置数卡
#define		DEBUG_CARD			0x0800			//调试复制卡

#define		EXPAND_CARD			0x8000			//扩展卡

extern	void 			EqiupmentOperate(void);
extern	unsigned int  	l_eqiupmentcode;
extern	short 			g_density;
extern	short 			g_hig; 
extern	char			g_modelnum;
extern	unsigned short 	g_loconum;
extern	unsigned short 	g_locotype;


extern	void PCOperate(unsigned char *DataBuf,unsigned int DataLen);		

extern	unsigned char GetRecaddr(void);
extern	void DipOperate(unsigned char *buf,unsigned int Len);

extern	void			DataSend_CSNR(unsigned char SourceAddr,unsigned char DistAddr,unsigned char	*DataBuf,unsigned int	DataLen);
extern	unsigned char  	CSNR_GetData(unsigned char	*RecBuf,unsigned char RecLen,unsigned char	*DataBuf,unsigned char	*InfoLen);


extern	uint8	GetFramRecEndFlg(void);
//清接收完成标志  南瑞
extern	void	ClearFramRecEndFlg(void);
extern	uint16	ReadRs485Data(uint8 *Buf);
extern	unsigned char GetRecaddr();
extern	void CSNR_Testdip(void);
#endif

