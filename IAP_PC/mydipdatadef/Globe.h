#ifndef  _Globe_h_
#define  _Globe_h_

//#include "DataTypeDef.h"
//#include "FIFO.h"
//#include "GlobePara.h"
//#include "CrcCheck.h"
//#include "BackUp.h"

#ifdef		HARD_VER_090214

#define		A1_LED			7
#define		A2_LED			6
#define		A3_LED			5
#define		A4_LED			4
#define		B1_LED			3
#define		B2_LED			2
#define		B3_LED			1
#define		B4_LED			0

#endif
/**/

#ifdef		HARD_VER_111122

#define		A1_LED			3
#define		A2_LED			2
#define		A3_LED			1
#define		A4_LED			0
#define		B1_LED			7
#define		B2_LED			6
#define		B3_LED			5
#define		B4_LED			4

#else

//#ifdef		HARD_VER_090426

#define		A1_LED			0
#define		A2_LED			1
#define		A3_LED			2
#define		A4_LED			3
#define		B1_LED			4
#define		B2_LED			5
#define		B3_LED			6
#define		B4_LED			7

//#endif

#endif


#define     DIP1_LED      	A2_LED
#define     DIP2_LED   		A2_LED
#define     CARD_LED      	A1_LED
#define     TAX2_LED   		A3_LED
#define     HOST_LED   		A4_LED
#define     RUN_LED        	B4_LED
#define     WRITE_CARD_LED  B1_LED
#define     CARD_FULL_LED  	B2_LED

//----------------------------------------------------------------------------
// 名    称：   void HardInit(void)
// 功    能:    硬件初始化
// 入口参数：	
// 出口参数：	
//----------------------------------------------------------------------------
extern	void HardInit(void);

//----------------------------------------------------------------------------
// 名    称：   void SysInit(void)
// 功    能:    系统初始化
// 入口参数：
// 出口参数：
//----------------------------------------------------------------------------
extern	void SysInit(void);

//----------------------------------------------------------------------------
// 名    称：   void SoftInit(void)
// 功    能:    软件初始化
// 入口参数： 
// 出口参数：
//----------------------------------------------------------------------------
extern	void SoftInit(void);

//----------------------------------------------------------------------------
// 名    称：   uint8 JudgeStartEvt(void)
// 功    能:    返回开机事件标识
// 入口参数： 
// 出口参数：
//----------------------------------------------------------------------------
extern	uint8 	JudgeStartEvt(void);

extern	void	SysHoldTast(void);
extern	void	SysFastHoldTast(void);
extern	void	ReCalPara(uint8 Flg);

#endif

