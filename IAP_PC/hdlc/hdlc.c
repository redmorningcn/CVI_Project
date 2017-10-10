#include <ansi_c.h>
#include "includes.h"



extern	unsigned char GetSendHdlcAddr(void);
extern	uint8	GetSetHdlcDataBuf(uint8 *buf) ;
extern	uint16 	GetCRCR16_8408_FFFF_FFFF_Table(unsigned char *crcBuff, unsigned char crcLen) ;
extern	uint8	GetHdlcFrameEndFlg(void);
extern	void	ClearHdlcFrameEndFlg(void);
extern	uint8	ReadHdlcFrame(uint8	*buf);
//extern	uint16	g_dipval;

#define	HDLC_UI_FRAM		0x03
#define	HDLC_SNRM_FRAM		0x83
#define	HDLC_DISC_FRAM		0x43
#define	HDLC_UP_FRAM		0x23
#define	HDLC_UA_FRAM		0x63
#define	HDLC_TEST_FRAM		0xe3
#define	HDLC_SIM_FRAM		0x07
#define	HDLC_FRMR_FRAM		0x87
#define	HDLC_SARM_FRAM		0x0f
#define	HDLC_REST_FRAM		0x8f
#define	HDLC_SARME_FRAM		0x4f
#define	HDLC_SNRME_FRAM		0xcf
#define	HDLC_SABM_FRAM		0x2f
#define	HDLC_XID_FRAM		0x5f
#define	HDLC_SABME_FRAM		0x3f



typedef struct	_stcHdlcPara_
{
	uint8		type;	
	uint8		sendconter;
	uint8		recconter;
	uint8		flg;	
	uint8		code;
	
	uint8		newframeflg;
	uint8		tmp[2];
	
}stcHdlcPara;
static	stcHdlcPara	s_hdlcpara;

uint8	GetUframAnswer(uint8 type)
{
	uint8	code;
	
	
	code =  HDLC_UA_FRAM;
	switch(type)
	{
		case HDLC_UI_FRAM:
			break;
		case HDLC_SNRM_FRAM:
			break;
		case HDLC_DISC_FRAM:
			break;
		case HDLC_UP_FRAM:
			break;
		case HDLC_UA_FRAM:
			break;
		case HDLC_TEST_FRAM:
			break;
		case HDLC_SIM_FRAM:
			break;
		case HDLC_FRMR_FRAM:
			break;
		case HDLC_SARM_FRAM:
			break;
		case HDLC_REST_FRAM:
			break;
		case HDLC_SARME_FRAM:
			break;
		case HDLC_SNRME_FRAM:
			break;
		case HDLC_SABM_FRAM:
			break;
		case HDLC_XID_FRAM:
			break;
		case HDLC_SABME_FRAM:
			break;

		default:;
			
	}
	return code;
}

extern	void	PrintfHdlcBufInfo(uint8 *buf,uint32	buflen) ;
//返回接收参数，数据
uint8	HdlcRecDeal(uint8* databuf,uint32	*datalen)
{
		uint32		len;
		uint16		crc16,crcrec;
		uint8		addr;
		uint8		type;
		uint8		buf[256];
		uint8		movesendbit = 1,moverecbit = 5,moveflgbit = 4,movecodebit=2;
		uint8		i;
		
		*datalen = 0; 		
		len 	= ReadHdlcFrame(buf);
		
		if(len<4)
		return 0;

		printftextbox("\r\n接收：");		
		
		for(i = 0;i< len;i++)
		printftextbox("%02x ",buf[i]);
		
		PrintfHdlcBufInfo(buf,len);		
		
		crc16 =  GetCRCR16_8408_FFFF_FFFF_Table(buf,len-2);	
		memcpy((uint8 *)&crcrec,&buf[len-2],2);	
		if(crc16 != crcrec)
		return 0;
		
		addr = buf[0];
		
//		if(addr != )
//		return;
		
		type = buf[1];
		
		switch( type &0x03 )
		{
				case  	0x01: 
							s_hdlcpara.type			= 0x01;
							s_hdlcpara.code 		= (type>>movecodebit)&0x03;
							s_hdlcpara.sendconter	= (((type>>moverecbit)&0x07)+1)%8;		//发送帧++
	
							break;			
				case  	0x03: 
							s_hdlcpara.type			= 0x03;
							s_hdlcpara.code 		= type;
							s_hdlcpara.flg     		= (type>>moveflgbit)&0x01;
							s_hdlcpara.sendconter	= 0;																		//清零
							s_hdlcpara.recconter	= 0;
							break;	
				default	:	
							s_hdlcpara.type			= 0x00;
							s_hdlcpara.recconter	= (type>>movesendbit)&0x07;							//接收帧赋值
							s_hdlcpara.flg     		= (type>>moveflgbit)&0x01;
							*datalen							= len - 2;	
							memcpy(databuf,&buf[2],len-2);
							
						
							if(databuf[0] == 0xc5 &&
							   databuf[1] == 0x68 &&
							   databuf[2] == 0x01 &&
							   (len-2)>4
							   )
							{
								memcpy((uint8 *)&g_dipval,&databuf[3],2);
								printftextbox(" 油量：%d",g_dipval);
							}
		}
		
		s_hdlcpara.newframeflg = 1;

		return 1;			 	
}



extern	void	ClearSoftStart(void);
extern	uint8	GetSoftStart(void);

uint8	HdlcSendDeal(void)
{
	uint8		buf[256];
	uint8		len;
	uint8		movesendbit = 1,moverecbit = 5,moveflgbit = 4,movecodebit=2;	
	uint16		crc16;
	uint32		i;
	
	static	uint32	time = 0;	

	if(
			(s_hdlcpara.newframeflg == 1)		|| 
			(GetSysTime() - time > 500)		 	||
			GetSoftStart()
		)
		{
			
			ClearSoftStart() ;
			len = 0;
			buf[len++] = GetSendHdlcAddr();
			
			if(s_hdlcpara.newframeflg == 1)																	//刚接收到数据
			{
				
				
				switch(s_hdlcpara.type)
				{
					case 0x01:
						//s_hdlcpara.type = 03;									//改变
						return	0;					
						break;
					case 0x03:
						buf[len++] = GetUframAnswer(s_hdlcpara.type);   		//返回相同,具体后续考虑
						break;
					case 0x00:
						buf[len++] = (s_hdlcpara.recconter << moverecbit) + (s_hdlcpara.flg<< moveflgbit) + 0x01;	//应答RR帧
						break;
					default:;
				}
				
				s_hdlcpara.newframeflg = 0; 
			}
			else
			{
				time = GetSysTime();
				
				len = 0;
				buf[len++] = GetSendHdlcAddr();
				
				buf[len++] =((s_hdlcpara.sendconter<<movesendbit)) + (s_hdlcpara.flg<< moveflgbit);  
				
				len += GetSetHdlcDataBuf(&buf[len]); 				
			}
			
			crc16 =  GetCRCR16_8408_FFFF_FFFF_Table(buf,len); 
			memcpy(&buf[len],(uint8 *)&crc16,sizeof(crc16));
			len += sizeof(crc16) ;		
			
			DataSend_CSNR(0x80,0xA1,(unsigned char *)&buf,len);	
					
			printftextbox("\r\n发送：");
			for(i=0;i<len;i++)
			{
				printftextbox("%02x ",buf[i]);	
			}
			
			PrintfHdlcBufInfo(buf,len); 
	}
	
	return 1;
}

void HdlcDeal(void)
{
	uint8		buf[256];
	uint32		len;
	uint8		flg;
	
//	if(GetHdlcFrameEndFlg())			//hdlc 从串口接收  
	{
//		ClearHdlcFrameEndFlg(); 
		
		HdlcRecDeal(buf,(uint32 *)&len);
	}
	
	HdlcSendDeal();			//hdlc 发送到串口
}


