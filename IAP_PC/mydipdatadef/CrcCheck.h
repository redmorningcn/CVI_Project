
#ifndef  _CrcCheck_h_
#define  _CrcCheck_h_

//----------------------------------------------------------------------------
// 名    称：   uint16 GetCrc16Check(uint8 *Buf, uint16 Len)
// 功    能：   取CRC16校验和
// 入口参数：  
// 出口参数：   
//----------------------------------------------------------------------------
extern	uint16 GetCrc16Check(uint8 *Buf, uint16 Len);
    
extern	uint16 GetModBusCrc16(uint8 *puchMsg,uint16  usDataLen);    

extern	uint16 GetModBusCrc16Up(unsigned char *puchMsg,unsigned short  usDataLen);
#endif


