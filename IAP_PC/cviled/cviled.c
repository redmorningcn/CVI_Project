
#include <userint.h>
#include "ndpcom.h"  
#include "includes.h"



#define		Led0	0							
#define		Led1	0							
#define		Led2	0							
#define		Led3	0							
#define		Led4	0
#define		Led5	0
#define		Led6	0
#define		Led7	0





#define		LED_MAX	8

typedef struct _stcLed_
                {
                    volatile 	uint32  WorkingFlag;            //指示灯开启标志
                    volatile 	uint32  times;            		//指示灯闪烁的次数
                    volatile 	uint32  WorkXms;              	//指示灯亮的时间
                    volatile 	uint32  unWorkXms;             	//指示灯不亮的时间
                    volatile 	uint32	TotalTime;
                    volatile 	uint32  StarTimer;
                }stcLed;

stcLed	sLed[8] =
			{
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
				{0,0,0,0,1,0},
			};
			
uint8  	HLAB_i;
uint32  CurentTime;

//------------------------------------------------------------------------
//	名	称 ：void LedUnLight(void)
//	功	能 ：指示灯持续灭
// 入口参数：
// 出口参数：
//------------------------------------------------------------------------
void UnLightLed(uint8 num)
{
	uint8 buf[]={Led0,Led1,Led2,Led3,Led4,Led5,Led6,Led7};
	SetCtrlVal(panelHandle,buf[num],0);	
}
//------------------------------------------------------------------------
//	名	称 ：void LedLight(void)
//	功	能 ：指示灯持续亮
// 入口参数：
// 出口参数：
//------------------------------------------------------------------------
void LightLed(uint8 num)
{
	uint8 buf[]={Led0,Led1,Led2,Led3,Led4,Led5,Led6,Led7};
	SetCtrlVal(panelHandle,buf[num],1);	
}

//------------------------------------------------------------------------
//	名	称 ：void OnLed(uint8 num,uint32 times,uint16 LightTime,uint16 unLightTime)
//	功	能 ：控制指示灯
// 入口参数：uint8 Num			亮灯的对应序号	 参照 Driver.h 有关声明
//			 uint16	times			蜂鸣器响的次数
//			 uint16	LightTime		响的时间  以 0.1s为单位
//			 uint16	unLightTime		不响的时间 以 0.1s 为单位
// 出口参数：操作成功返回 TRUE	操作失败返回 FAULSE
//------------------------------------------------------------------------
void OnLed(uint8 num,uint32 times,uint16	LightTime,uint16 unLightTime)
{
	
	if(num >= LED_MAX || times == 0 || sLed[num].WorkingFlag)
	{
	    return;
	}
	if( LightTime ==0 || times == 0 )
	{
		return;
	}
	
	sLed[num].times 		= times;
	sLed[num].WorkXms		= LightTime * 10;
	sLed[num].unWorkXms 	= unLightTime * 10;
	sLed[num].WorkXms		= LightTime;
	sLed[num].unWorkXms 	= unLightTime;	
	sLed[num].TotalTime 	= sLed[num].WorkXms + sLed[num].unWorkXms ;
	sLed[num].StarTimer		= GetSysTime();
	sLed[num].WorkingFlag	= 1;
	LightLed(num);	
}

//------------------------------------------------------------------------
//	名	称 ：void OnAllLed(void)
//	功	能 ：全开
// 入口参数：
// 出口参数：
//------------------------------------------------------------------------
void OnAllLed(void)
{
	uint8 num;
	for(num = 0; num < LED_MAX; num++)
	{
		OnLed(num,100,10000,0);						//开灯
    }
}

//------------------------------------------------------------------------
//	名	称 ：void LedOff(uint8 LedNum)
//	功	能 ：关闭对应序号指示灯
// 入口参数：无 
// 出口参数：
//------------------------------------------------------------------------
void OffLed(uint8 num)
{
	sLed[num].WorkingFlag	= 0;            //指示灯开启标志
    sLed[num].times			= 0;            //指示灯闪烁的次数
    sLed[num].WorkXms		= 0;            //指示灯亮的时间
    sLed[num].unWorkXms		= 0;          	//指示灯不亮的时间
    sLed[num].TotalTime		= 1;
    sLed[num].StarTimer		= 0;
    UnLightLed(num);
}

//------------------------------------------------------------------------
//	名	称 ：void OffAllLed(void)
//	功	能 ：灯全关
// 入口参数：无
// 出口参数：
//------------------------------------------------------------------------
void OffAllLed(void)
{
	uint8 num;
	for(num = 0; num < LED_MAX; num++)
	{
		OffLed(num);								//关灯
    }
}

//------------------------------------------------------------------------
//	名	称 ：void HoldLed(void)
//	功	能 ：指示灯维护
// 入口参数：
// 出口参数：
//------------------------------------------------------------------------
void HoldLed(void)
{
	CurentTime = GetSysTime();
	for(HLAB_i =	0; HLAB_i < LED_MAX; HLAB_i++)
	{
		if(sLed[HLAB_i].WorkingFlag)
		{
			if( ( (CurentTime - sLed[HLAB_i].StarTimer) % sLed[HLAB_i].TotalTime) 
					< sLed[HLAB_i].WorkXms)
				LightLed(HLAB_i);
			else
			{
				UnLightLed(HLAB_i);
			}
			if( ( (CurentTime	- sLed[HLAB_i].StarTimer) / sLed[HLAB_i].TotalTime) 
					>= (sLed[HLAB_i].times))
		   	{
		   	 	OffLed(HLAB_i);
		   	}
		}
	}
}

//------------------------------------------------------------------------
//	名	称 ：void InitLed( void )
//	功	能 ：指示灯初始化
// 入口参数：
// 出口参数：
//------------------------------------------------------------------------
void InitLed( void )
{
	uint8	i;	
//	PINSEL2 |= 0x00000010;					//初始化发光二极管端口
//	
//	IO2DIR  |=  (0x0000001<<Led0)
//	          +(0x0000001<<Led1)
//	          +(0x0000001<<Led2)
//             +(0x0000001<<Led3)
//             +(0x0000001<<Led4)
//              +(0x0000001<<Led5)
//              +(0x0000001<<Led6)
//              +(0x0000001<<Led7);					
    
	for(i = 0;i < 2; i++)					
	{
		OnAllLed();								
	
	    DelayX10ms(25);

		OffAllLed();						
		
		DelayX10ms(25);
	}
}
