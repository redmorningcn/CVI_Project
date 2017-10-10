#ifndef  _FIFO_h_
#define  _FIFO_h_


typedef	unsigned char  uint8;					/* 无符号8位整型变量						*/
typedef	    	 char  int8;					/* 有符号8位整型变量						*/
//typedef	unsigned char  uint8;					/* 无符号8位整型变量						*/
//typedef	signed	 char  int8;					/* 有符号8位整型变量						*/
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

typedef struct _stcFIFO                     //定义先进先出缓冲器的数据结构
{
    uint16  pWrite;                         //缓冲区写指针
    uint16  pRead;                          //缓冲区读指针
    uint16  Size;                           //缓冲区大小
//	uint16	Rec;
    uint8   *BufAddr;                       //缓冲区首地址
}stcFIFO; 

//------------------------------------------------------------------------
// 名    称：   InitFIFO(stcFIFO *FIFO,uint8 *Buf,uint16 Size)
// 功    能：   初始化FIFO缓冲区
// 入口参数：	FIFO:要初始化的FIFO缓冲区结构,Buf:缓冲区地址,SIZE:缓冲区大小
// 出口参数：
//------------------------------------------------------------------------
extern	void   InitFIFO(stcFIFO *FIFO,uint8 *Buf,uint16 Size);

//------------------------------------------------------------------------
// 名    称：   uint16  GetNoReadSizeFIFO(stcFIFO *FIFO)
// 功    能：   求FIFO缓冲区中未读空间大小
// 入口参数：	FIFO:的FIFO缓冲区结构
// 出口参数：	缓冲区未读数据大小
//------------------------------------------------------------------------
extern	uint16  GetNoReadSizeFIFO(stcFIFO *FIFO);

//------------------------------------------------------------------------
// 名    称：   uint16  GetNoWriteSizeFIFO(stcFIFO *FIFO)
// 功    能：   求FIFO缓冲区中未写空间大小
// 入口参数：	FIFO:的FIFO缓冲区结构
// 出口参数：	缓冲区未写数据大小
//------------------------------------------------------------------------
extern	uint16  GetNoWriteSizeFIFO(stcFIFO *FIFO);

//------------------------------------------------------------------------
// 名    称：   uint8   WriteFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen)
// 功    能：   写FIFO缓冲区
// 入口参数：	FIFO:的FIFO缓冲区结构,DatBuf:写入数据,DatLen:数据长度
// 出口参数：	写FIFO缓冲区成功返回1,错误返回0
//------------------------------------------------------------------------
extern	uint8   WriteFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen);

//------------------------------------------------------------------------
// 名    称：   uint8   ReadFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen)
// 功    能：   读FIFO缓冲区
// 入口参数：	FIFO:的FIFO缓冲区结构,DatBuf:读数据缓冲位置,DatLen:要读数据长度数据长度
// 出口参数：	读FIFO缓冲区成功1,返回为0
//------------------------------------------------------------------------
extern	uint8   ReadFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen);

//------------------------------------------------------------------------
// 名    称：   void   ClearFIFO(stcFIFO *FIFO)
// 功    能：   初始化FIFO缓冲区
// 入口参数：	FIFO:要初始化的FIFO缓冲区结构,Buf:缓冲区地址,SIZE:缓冲区大小
// 出口参数：
//------------------------------------------------------------------------
extern	void   ClearFIFO(stcFIFO *FIFO);

extern	uint8   OnlyReadFIFO(stcFIFO *FIFO,uint8 *DatBuf,uint16 DatLen);
#endif                      
