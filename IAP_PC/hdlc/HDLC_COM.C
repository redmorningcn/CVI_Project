#include <ansi_c.h>
#include "includes.h"



//extern	GetSetHdlcDataBuf


//////////////////////////////////////////
const	 uint8	dipbuf1[3] = {0xc5,0x64,01};
const	 uint8	dipbuf2[3] = {0xb5,0x02,01};
const	 uint8	dipbuf3[116] = {0xb6,0x02,0x01,0x26,0x01,0x00,0x00,0x00,0x00,0x00,
								0x45,0x50,0x01,0x46,0x01,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x41,
								0x45,0x49,0x20,0x54,0x61,0x67,0x20,0x43,0x6f,
								0x6d,0x6d,0x20,0x46,0x61,0x69,0x6c,0x65,0x64,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
								0x00,0x00,0x00,0x00,0x00,0x00,0x00
							};
	
	
uint8	GetFramRecEndFlgDip(void)
{
	return GetRecCom0EndFlg();  
}

//清接收完成标志  南瑞
void	ClearFramRecEndFlgDip(void)
{
	ClearRecCom0EndFlg();   
}

uint16	ReadRs485DataDip(uint8 *Buf)
{
		return	ReadCom0(Buf,256);  
}


uint8	l_hdlcframeendflg = 0;
uint8	GetHdlcFrameEndFlg(void)
{							   
	return	GetFramRecEndFlgDip();
}

void	ClearHdlcFrameEndFlg(void)
{
	ClearFramRecEndFlgDip();
}

uint32		l_hdlc485point;
uint8		ReadHdlcFrame(uint8	*buf)
{
		static	uint8	recbuf[256];
		uint8	reclen,datalen =0;
		uint8	i;
		static	uint32	lastlen = 0;
		
		if(GetHdlcFrameEndFlg())
		{
			ClearHdlcFrameEndFlg(); 	
			reclen =	ReadRs485DataDip(recbuf); 
		
			
				l_hdlc485point=0;
				lastlen = reclen;
				
//				printfcom2("\r\n ");
//				for(i = 0;i< reclen;i++)
//				printfcom2("%02x ",recbuf[i]);
//				printfcom2("\r\n ");   
					
				CSNR_GetData(&recbuf[l_hdlc485point],reclen,buf,(uint8 *)&datalen); 
//				printfcom2("\r\n l_hdlc485point %d",l_hdlc485point);
			
		}
		


		if((l_hdlc485point + 10) < lastlen)
		{
//			printfcom2("\r\n ");
//			for(i = 0;i< reclen;i++)
//			printfcom2("%02x ",recbuf[i]);
//			printfcom2("\r\n ");   
//					
			CSNR_GetData(&recbuf[l_hdlc485point],reclen,buf,(uint8 *)&datalen); 
		
			if( datalen)
			{
				l_hdlcframeendflg = 1;	
			}
			
//			printfcom2("\r\n 485reclen %d,hdlcdatalen %d",reclen,datalen);  
		}
		{
			l_hdlcframeendflg = 0;
		}
		return	datalen;
}

extern	uint16 	GetCRCR16_8408_FFFF_FFFF_Table(unsigned char *crcBuff, unsigned char crcLen) ;  

void	PrintfHdlcBufInfo(uint8 *buf,uint32	buflen)
{
	uint32		i;
	uint16 		crc16,crcrec;		
	uint8		len =0;
	uint8		typebyte;
	uint8		movesendbit = 1,moverecbit = 5,moveflgbit = 4,movecodebit=2; 
	uint8		addr;
	uint16		dipval;
	
	
			
	if(buflen < 4)
	return;

	crc16 =  GetCRCR16_8408_FFFF_FFFF_Table(buf,buflen-2);
	memcpy((uint8 *)&crcrec,&buf[buflen-2],2);
	
	if(crc16 != crcrec)
	{
		printfcom2("CRC ERR %x,%x",crc16,crcrec);
		return;
	}
	len = 0;
    addr = buf[len++];
//	printfcom2("\r\n 地址%x：",addr);
	buflen--;

	typebyte = buf[len++];
	buflen--;

	switch( typebyte &0x03 )
	{
		uint8	code=0; 
		uint8	slavesendnum;
		uint8	slaverecnum;
		uint8	flgbit;
		
		case  	0x01:
				code 		= (typebyte>>movecodebit)&0x03;
				slaverecnum = (typebyte>>moverecbit)&0x07;

//				printfcom2("监控帧 code %x,recnun %x",code,slaverecnum); 			   
				break;
		case   0x03:

//			   printfcom2("无编号帧 typebyte %x",typebyte);
				
			   break;
		default:
			
				slavesendnum = (typebyte>>movesendbit)&0x07;			
				slaverecnum = (typebyte>>moverecbit)&0x07;  
				flgbit      = (typebyte>>moveflgbit)&0x01;

//			   	printfcom2("数据帧 slavesendnum %x,slaverecnum %x,",slavesendnum,slaverecnum);  				
				
	}
	
	if(buflen>2)
	{
#if 0    /* formerly excluded lines */
		printfcom2("  数据 datalen %d:",buflen);
		for(i =0 ;i<buflen;i++)
		{
			printfcom2(" %02x",buf[i+2]);
		}
#endif   /* formerly excluded lines */
		
		if(buf[2] == 0xc5 &&
		   buf[3] == 0x68 &&
		   buf[4] == 0x01 &&
		   (buflen)>4
	    )
		{
			memcpy((uint8 *)&dipval,&buf[5],2);
			printfcom2("\r\n油量：%d",dipval);
		}
		if(buf[2] == 0xc5 &&
		   buf[3] == 0x67 &&
		   buf[4] == 0x01 &&
		   (buflen)>4
	    )
		{
			memcpy((uint8 *)&dipval,&buf[5],2);
			printfcom2("\r\n高度：%d",dipval);
		}		
	}
}


extern	void	HdlcDeal(void);
void  CSNR_Testdip(void)   
{
	if(GetOpenCom0Flg())
	 {
		HdlcDeal();
	 }
}
