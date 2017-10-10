/*******************************************************************************
 *   Filename:       cvi_senddata.c
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Description:    采用csnc协议格式发送数据。
////////////////////////////////////////////////////////////////////////////
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/


/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <rs232.h>
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <formatio.h>
#include "cvi_file.h"



/********************************************************************************************/
/* Constants																				*/
/********************************************************************************************/
stcCsnrProtocolPara	gsCsnrProtocolPara;

unsigned char	senddatabuf[256];
unsigned char 	txdatabuf[256];

extern	int		gPanelHandle;
extern	int		gBinFilePanelHandle;
void 	printftext(int	panel,int id,const char *format, ...) ;

#define		MY_ADDR				0xca


/*******************************************************************************************
数据发送： SendCsnrDataDeal
将数据按照协议格式发送。
格式的值  	系统运行时自动更改。
*******************************************************************************************/
void	SendCsnrDataDeal(char *buf,int len)
{
	 int i;

	 gsCsnrProtocolPara.databuf =  senddatabuf;		//发送指针赋值
	 gsCsnrProtocolPara.rxtxbuf =  txdatabuf;

	 memcpy(gsCsnrProtocolPara.databuf,buf,len);	//数据幅值
	 gsCsnrProtocolPara.datalen = (unsigned char)len;

	 DataPackage_CSNC(&gsCsnrProtocolPara);			//按协议打包

	 SendCom1(gsCsnrProtocolPara.rxtxbuf,gsCsnrProtocolPara.rxtxlen);  //从串发送数据


#ifdef	SEND_PRINTF
	for (i = 0 ;i < gsCsnrProtocolPara.rxtxlen; i++)
	{
		printftext(gBinFilePanelHandle,IAP_PANEL_INFOTEXTBOX,"%02X",gsCsnrProtocolPara.rxtxbuf[i]);
	}
	printftext(gBinFilePanelHandle,IAP_PANEL_INFOTEXTBOX,"\r\n");
#endif
}


/*******************************************************************************************
数据发送： 取SENDTEXTBOX的内容，将string转换为hex
*******************************************************************************************/
int	GetSendTextBoxData(uint8 buf[])
{
	uint8	string[512];
	uint32	len,i,j;
	uint8	charbuf[4]="0";

	GetCtrlVal(gPanelHandle,PANEL_COM1SENDTEXTBOX,string);  	//取控件中的内容

	len = 0;

	for(i = 0,j = 0;i<strlen(string);i++)							//数据转换
	{
		if(string[i] == ' ')								//去空格
		{
			continue;
		}else{

			//判断非法字符
			charbuf[j%2] = string[i];
			if(j %2 == 1)
			{
			    charbuf[2] 	= 	'\0';
				buf[len++]  = 	(unsigned char)strtol(charbuf,NULL,16);
			}
			j++;
		}
	}

	return	len;
}

/*******************************************************************************************
数据发送： SendTextBox()
发送SENDTEXTBOX的内容
*******************************************************************************************/
void	SendTextBox(void)
{
	char	buf[256];
	int		len;

	len = GetSendTextBoxData(buf);		//取发送区值

	SendCsnrDataDeal(buf,len);
}

