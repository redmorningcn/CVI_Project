

#define		FALSE	0
#define		TRUE	1

#include	"FIFO.h"

//------------------------------------------------------------------------
// 名    称：   InitFIFO(stcFIFO *FIFO,unsigned char *Buf,unsigned short Size)
// 功    能：   初始化FIFO缓冲区
// 入口参数：	FIFO:要初始化的FIFO缓冲区结构,Buf:缓冲区地址,SIZE:缓冲区大小
// 出口参数：
//------------------------------------------------------------------------
void   InitFIFO(stcFIFO *FIFO,unsigned char *Buf,unsigned short Size)
{
    FIFO->pWrite = 0;
    FIFO->pRead  = 0;
    FIFO->Size   = Size;
    FIFO->BufAddr= Buf;
}

//------------------------------------------------------------------------
// 名    称：   void   ClearFIFO(stcFIFO *FIFO)
// 功    能：   初始化FIFO缓冲区
// 入口参数：	FIFO:要初始化的FIFO缓冲区结构,Buf:缓冲区地址,SIZE:缓冲区大小
// 出口参数：
//------------------------------------------------------------------------
void   ClearFIFO(stcFIFO *FIFO)
{
    FIFO->pWrite = 0;
    FIFO->pRead  = 0;	
}

//------------------------------------------------------------------------
// 名    称：   unsigned short  GetNoReadSizeFIFO(stcFIFO *FIFO)
// 功    能：   求FIFO缓冲区中未读空间大小
// 入口参数：	FIFO:的FIFO缓冲区结构
// 出口参数：	缓冲区未读数据大小
//------------------------------------------------------------------------
unsigned short  GetNoReadSizeFIFO(stcFIFO *FIFO)
{
    unsigned short  pWrite = FIFO->pWrite;
    unsigned short  pRead  = FIFO->pRead;
    unsigned short  pBufSize = FIFO->Size;

	
    return (pWrite + pBufSize - pRead) % pBufSize;
}

//------------------------------------------------------------------------
// 名    称：   unsigned short  GetNoWriteSizeFIFO(stcFIFO *FIFO)
// 功    能：   求FIFO缓冲区中未写空间大小
// 入口参数：	FIFO:的FIFO缓冲区结构
// 出口参数：	缓冲区未写数据大小
//------------------------------------------------------------------------
unsigned short  GetNoWriteSizeFIFO(stcFIFO *FIFO)
{
    unsigned short  pBufSize = FIFO->Size;
    return  pBufSize - GetNoReadSizeFIFO(FIFO);
}

//------------------------------------------------------------------------
// 名    称：   unsigned char   WriteFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
// 功    能：   写FIFO缓冲区
// 入口参数：	FIFO:的FIFO缓冲区结构,DatBuf:写入数据,DatLen:数据长度
// 出口参数：	写FIFO缓冲区成功返回1,错误返回0
//------------------------------------------------------------------------
unsigned char   WriteFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
{
    unsigned short  i,j;
    unsigned short  NoWriteSize;
    unsigned short  pWrite = FIFO->pWrite;
    unsigned short  pBufSize = FIFO->Size;
    unsigned char   *FIFOBuf = FIFO->BufAddr;

    NoWriteSize =  GetNoWriteSizeFIFO(FIFO);   		//确认缓冲区还没有未写空间大小

    if(DatLen >= NoWriteSize)
    {
        return  FALSE;                     	 		//数据比缓冲区剩余空间大,返回写错误
    }

    for(i = 0; i < DatLen; i++)             		//将数据写入缓冲区
    {
        j =(pWrite + i) % pBufSize;         		//计算写地址

        FIFOBuf[j] =  DatBuf[i];            		//写数据
    }

    FIFO->pWrite = (pWrite + DatLen ) % pBufSize; 	//写指针++

    return  TRUE;
}

//------------------------------------------------------------------------
// 名    称：   unsigned char   ReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
// 功    能：   读FIFO缓冲区
// 入口参数：	FIFO:的FIFO缓冲区结构,DatBuf:读数据缓冲位置,DatLen:要读数据长度数据长度
// 出口参数：	读FIFO缓冲区成功1,返回为0
//------------------------------------------------------------------------
unsigned char   ReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
{
    unsigned short  i,j;
    unsigned short  NoReadSize;
    unsigned short  pRead = FIFO->pRead;
    unsigned short  pBufSize = FIFO->Size;
    unsigned char   *FIFOBuf = FIFO->BufAddr;

    NoReadSize = GetNoReadSizeFIFO(FIFO);      	//求未读空间大小


    if(DatLen > NoReadSize )                 	//读空间小于要读数据,返回读取错误
    {
        return FALSE;
    }

    for(i = 0; i < DatLen; i++)
    {
        j = (pRead + i) % pBufSize;         	//计算读地址

        DatBuf[i] = FIFOBuf[j];             	//读取数据		
    }

    FIFO->pRead = (pRead + DatLen) % pBufSize;  //读指针++

    return  TRUE;
}


//------------------------------------------------------------------------
// 名    称：   unsigned char   ReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
// 功    能：   读FIFO缓冲区
// 入口参数：	FIFO:的FIFO缓冲区结构,DatBuf:读数据缓冲位置,DatLen:要读数据长度数据长度
// 出口参数：	读FIFO缓冲区成功1,返回为0
//------------------------------------------------------------------------
unsigned char   OnlyReadFIFO(stcFIFO *FIFO,unsigned char *DatBuf,unsigned short DatLen)
{
    unsigned short  i,j;
    unsigned short  NoReadSize;
    unsigned short  pRead = FIFO->pRead;
    unsigned short  pBufSize = FIFO->Size;
    unsigned char   *FIFOBuf = FIFO->BufAddr;

    NoReadSize = GetNoReadSizeFIFO(FIFO);      	//求未读空间大小

    if(DatLen > NoReadSize )                 	//读空间小于要读数据,返回读取错误
    {
        return FALSE;
    }

    for(i = 0; i < DatLen; i++)
    {
        j = (pRead + i) % pBufSize;         	//计算读地址

        DatBuf[i] = FIFOBuf[j];             	//读取数据
    }

	//FIFO->pRead = (pRead + DatLen) % pBufSize;  //读指针++
	FIFO->pRead = (pRead ) % pBufSize;  //读指针++

    return  TRUE;
}
