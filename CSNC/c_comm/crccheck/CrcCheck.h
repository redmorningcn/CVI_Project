
#ifndef  _CrcCheck_h_
#define  _CrcCheck_h_

//----------------------------------------------------------------------------
// 名    称：   unsigned short GetCrc16Check(uint8 *Buf, unsigned short Len)
// 功    能：   取CRC16校验和
// 入口参数：  
// 出口参数：   
//----------------------------------------------------------------------------
extern	unsigned short GetCrc16Check(unsigned char *Buf, unsigned short Len);
    
extern	unsigned short GetModBusCrc16(unsigned char *puchMsg,unsigned short  usDataLen);    

extern	unsigned short GetModBusCrc16Up(unsigned char *puchMsg,unsigned short  usDataLen);

extern	unsigned char GetCheckSum(unsigned char  Buf[], unsigned int Len);

extern	unsigned char GetCheckSumNR(unsigned char  Buf[], unsigned int Len); 

#endif


