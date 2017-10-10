
#include <ansi_c.h>
#include "includes.h"


extern 	uint8	GetProgramFlg(void);
extern	void	ClearProgramFlg(void); 
extern	void 	printftexbox(uint8 *g_textboxbuf);
extern	uint32	GetFileData(uint8 *buf);
extern	void	DataSend_CSNR(unsigned char SourceAddr,unsigned char DistAddr,unsigned char	*DataBuf,unsigned int	DataLen);


uint8	 GetFramRecEndFlgIAP(void)
{
	return	GetRecCom0EndFlg();
}

void	 ClearFramRecEndFlgIAP(void)
{
	ClearRecCom0EndFlg();
}

uint32	 ReadRs485DataIAP(uint8 *buf)
{
	return  ReadCom0(buf,256); 
}


extern	uint8	GetFramNum(void);
extern	void	AddFramNum(void);
extern	uint8	GetRecFramNum(void);
extern	uint8	GetDestAddr(void);
extern	void	ClearFramNum(void);

#define		RST_SYS		0x100000		//系统复位

void	IAP_ProgramTask(void)
{
	static	uint32		time;
	static	uint8		filebuf[256];
	static	uint32		filelen =0;
	uint8	databuf[256];
	uint8	recbuf[256];
	
	uint32	tmp = 0;
	uint32	datalen =0;
	static	uint8	startflg = 0;
	uint8	startstring[]={"IAP_pragram start!"};
	uint8	endstring[]={"IAP_pragram end!"};	
	
	uint32	i;
	uint8	tmpbuf[16];
	static 	uint32	norectimes = 0;
	static	uint32	errtime = 0;
	uint32	rstsyscode = RST_SYS;
	static	uint32	starttimes = 0;

	uint8	destaddr;
	
//	DelayX10ms(1);
	
	datalen =0; 
	
	destaddr = GetDestAddr();
	if(GetProgramFlg())		//编程
	{
		if(startflg == 0)
		{
			if(	GetSysTime() - time > 50 )
			{
				time = GetSysTime();		

				if(starttimes++ %2 == 0)
				{
					DataSend_CSNR(0x80,destaddr,(unsigned char *)&startstring,strlen(startstring));	
				}
				else
				{  
					DataSend_CSNR(0xc2,0x80,(unsigned char *)&rstsyscode,sizeof(rstsyscode));
				}
				
				ClearFramNum();
			}
		}
		
		DelayX10ms(2);
		
		if(    
				GetFramRecEndFlgIAP() 
			|| (GetSysTime() - time > 50  &&  startflg )
		  )
		{
			norectimes++;
			if(GetFramRecEndFlgIAP())
			{
				norectimes = 0;
			}
																																
			if(norectimes == 20)
			{
				startflg = 0;
				ClearProgramFlg();
				printftexbox("\r\ntime	no rec"); 					
			}
			
			
			time = GetSysTime();		

			ClearFramRecEndFlgIAP();
			
			tmp = ReadRs485DataIAP(recbuf);

			CSNR_GetData(recbuf,tmp,databuf,(uint8 *)&datalen);  
		
			databuf[datalen] = '\0';
			printftexbox(databuf); 
			
			if(startflg == 0)			
			{	
				if(strcmp(databuf,startstring) == 0)				//编程判定
				{
					startflg = 1;
					ClearFramNum();
				}
			}
			
																					   
			if(startflg == 1)
			{
				sprintf(tmpbuf,"接收%x,发送%x "	,GetFramNum(),GetRecFramNum());
				printftexbox(tmpbuf);
			
				if(GetRecFramNum() == GetFramNum())   //流量控制	 有回复序号则加1
				{
					AddFramNum();					  //流量控制

					filelen = GetFileData(filebuf);
					errtime = 0;

				}
//				else
//				{
//					errtime++;
//					if(errtime > 5)
//					{
//						AddFramNum();					  //流量控制
//						errtime = 0;
//					}
//
//				}
				
				DataSend_CSNR(0x80,destaddr,filebuf,filelen);
				
				
				if(filelen != 128)					//结束判读
				{
					startflg = 0;
					ClearProgramFlg();
					printftexbox("\r\nend file"); 
					DelayX10ms(10);
					DataSend_CSNR(0x80,destaddr,(unsigned char *)&endstring,strlen(endstring));
				}
			}										
		}
		
		if(GetSysTime() - time > 400)
		{
			startflg = 0;
			ClearProgramFlg();
			printftexbox("\r\nno response"); 			
		}
	}
}
