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
#include "IAP.h"
#include "panel1.h"
#include "csnr_package_deal.h"
#include "cvi_com_operate.h"
#include "includes.h"
#include "recvdata.h"


/********************************************************************************************/
/* Constants																				*/
/********************************************************************************************/
stcCsnrProtocolPara	gsCsnrProtocolPara;

unsigned char	senddatabuf[256];
unsigned char 	txdatabuf[256];

extern	int	gPanelHandle;
extern	int	gBinFilePanelHandle;
void printftext(int	panel,int id,const char *format, ...) ;

#define		MY_ADDR				0xca

//打印发送信息
//#define		SEND_PRINTF	    1

/*******************************************************************************************
数据发送： SendCsnrDataDeal
将数据按照协议格式发送。
格式的值  	系统运行时自动更改。
*******************************************************************************************/
void	SendRecordRecEcho(void)
{

	 stcCsnrProtocolPara	sCsnrProtocolPara;

//////////////////////////////////////////////////////////
	 sCsnrProtocolPara.databuf 		=  senddatabuf;		//发送指针赋值
	 sCsnrProtocolPara.rxtxbuf 		=  txdatabuf;
	 sCsnrProtocolPara.sourceaddr 	=  MY_ADDR;
	 sCsnrProtocolPara.destaddr 	=  0x80;
 	 sCsnrProtocolPara.framcode 	=  gsRecCsnrProtocolPara.framcode;
 	 sCsnrProtocolPara.framnum  	=  gsRecCsnrProtocolPara.framnum;

	 sCsnrProtocolPara.datalen = 0;

	 DataPackage_CSNC(&sCsnrProtocolPara);			//按协议打包

	 SendCom1(sCsnrProtocolPara.rxtxbuf,sCsnrProtocolPara.rxtxlen);  //从串发送数据
}


#define		DATA_CARD			0x0001				//êy?Y?¨
#define		IC_ADDR				0xc1

void DataComReadAsk(unsigned int startnum,unsigned int endnum)
{
	uint32	datalen;
	uint32	cardflg;
	unsigned char  buf[32];
	stcCsnrProtocolPara	sCsnrProtocolPara;

	datalen = 0;

	cardflg = DATA_CARD;

	memcpy(&buf[datalen],(unsigned char *)&cardflg,sizeof(cardflg));
	datalen += sizeof(cardflg);
	memcpy(&buf[datalen],(unsigned char *)&startnum,sizeof(startnum));
	datalen += sizeof(startnum);
	memcpy(&buf[datalen],(unsigned char *)&endnum,sizeof(endnum));
	datalen += sizeof(endnum);

//////////////////////////////////////////////////////////
	sCsnrProtocolPara.databuf 		=  buf;					//发送指针赋值
	sCsnrProtocolPara.rxtxbuf 		=  txdatabuf;
	sCsnrProtocolPara.sourceaddr 	=  IC_ADDR;
	sCsnrProtocolPara.destaddr 		=  0x80;
	sCsnrProtocolPara.framcode 		=  gsRecCsnrProtocolPara.framcode;
	sCsnrProtocolPara.framnum  		=  gsRecCsnrProtocolPara.framnum;

	 sCsnrProtocolPara.datalen 		= datalen;

	 DataPackage_CSNC(&sCsnrProtocolPara);			//按协议打包

	 SendCom1(sCsnrProtocolPara.rxtxbuf,sCsnrProtocolPara.rxtxlen);  //从串发送数据
}

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


extern	stcFileInfo		gsBinFileInfo;
extern	char	SetFileDealPer(void);
extern	stcIapCtrl	gsIapCtrl;

void InitIPACtrl(void)
{
	gsIapCtrl.echoflg 	= 0;
	gsIapCtrl.endflg 	= 0;
	gsIapCtrl.filesize 	= gsBinFileInfo.size;
	gsIapCtrl.recvnum	= 0;
	gsIapCtrl.resendflg = 0;
	gsIapCtrl.sendnum	= 0;
	gsIapCtrl.sendsize  = 0;
	gsIapCtrl.startflg 	= 0;
}

typedef  struct   _stcIAPPara_
{
    short  	hardver;        //硬件版本
    short  	softver;        //软件版本
    int  	softsize;       //软件大小
    int  	addr;           //当前地址
    int  	framenum;       //帧序号
    short  	code;           //指令码 01，标示有正确的可用
    short  	crc16;
}stcIAPPara;

stcIAPPara	gsIapPara =
{
	0x10,
	0x20,
	0,
	0,
	0,
	0
};
void printftext(int	panel,int id,const char *format, ...);
extern int   g_systime;

/*******************************************************************************************
数据发送： SendFileData()
发送SendFileData的内容
*******************************************************************************************/
void	SendFileData(void)
{
	static	char	buf[256];
	static	int		len = 0;
	static	int 	resendtimes = 0;
	int		addr;
	int		per;
	short	tmp16;

	static	int	sendtime;
	//addr = 0;
	//
	if(			gsIapCtrl.startflg
			|| 	gsIapCtrl.echoflg
			|| 	gsIapCtrl.resendflg
	  )
	{//
		if(gsIapCtrl.startflg)						//开始下载，发送开始下载指令
		{
			InitIPACtrl();							//初始化控制变量

			gsIapPara.softsize 		= gsBinFileInfo.size;
			gsBinFileInfo.deallen 	= 0;

			len = 0;

			tmp16 = 0x01;							//开始指令
			memcpy(&buf[len],(char *)&tmp16,sizeof(tmp16));
			len += sizeof(tmp16);

			memcpy(&buf[len],(char *)&gsIapPara,sizeof(gsIapPara));
			len +=sizeof(gsIapPara);

			printf("\r\n发送启动命令，发送的序号: %d",gsIapPara.framenum);

			SendCsnrDataDeal(buf,len);											//发送数据

		}else if(gsIapCtrl.echoflg)												//收到正确的应答，继续数据发送
		{
			gsIapCtrl.echoflg = 0;
			len = 0;

			resendtimes = 10;		//重发控制
			sendtime =  g_systime;

			printftext(gBinFilePanelHandle,IAP_PANEL_INFOTEXTBOX,"发送序号%d\r\n",gsIapCtrl.sendnum);

			if(gsBinFileInfo.deallen <gsBinFileInfo.size)						//已出里文件长度小于文件长度
			{
				tmp16 = 0x02;
				memcpy(&buf[len],(char *)&tmp16,sizeof(tmp16));   				//指令
				len += sizeof(tmp16);

				memcpy(&buf[len],(char *)&gsIapCtrl.sendnum,sizeof(tmp16));   	//序号，在接收区累加
				len += sizeof(gsIapCtrl.sendnum);

				int readlen = ReadFileData(gsBinFileInfo.deallen,&buf[len],128);//从文件中读取数据
		 		len +=  readlen;

				per = SetFileDealPer();
				SetCtrlVal (gBinFilePanelHandle, IAP_PANEL_PRE, per);			//设置百分比(下载进度)

				int i =10000;
				while(i--);
				SendCsnrDataDeal(buf,len);										//发送数据

				gsBinFileInfo.deallen += readlen;								//已处理数据 长度累加len

				if(gsBinFileInfo.deallen >= gsBinFileInfo.size)					//数据发送完成
				{
					i = 0;
				}

				gsIapPara.framenum 		= gsIapCtrl.sendnum;					//记录已发序号

			}
			else	//数据发送完成，发送结束标示
			{
				tmp16 = 0x03;													//发送结束指令
				memcpy(&buf[len],(char *)&tmp16,sizeof(tmp16));   				//指令
				len += sizeof(tmp16);
				resendtimes = 0;		//重发控制

				memcpy(&buf[len],(char *)&gsIapPara,sizeof(gsIapPara));
				len +=sizeof(gsIapPara);
				gsIapPara.framenum  = 0;

				SendCsnrDataDeal(buf,len);										//发送数据

				printftext(gBinFilePanelHandle,IAP_PANEL_INFOTEXTBOX,"发送序号%d，数据发送完成\r\n",gsIapCtrl.sendnum);
			}
		}
		else if(gsIapCtrl.resendflg)											//未收到正确指令，重发
		{
			 gsIapCtrl.resendflg = 0;
			 SendCsnrDataDeal(buf,len);											//发送数据上次的数据
		}
	}

	//重发数据
	if(  	g_systime - sendtime > 100	//1s
		&&  resendtimes					//重发次数大于1
		)
	{
		sendtime =  g_systime;
		resendtimes--;

		printftext(gBinFilePanelHandle,IAP_PANEL_INFOTEXTBOX,"发送序号%d\r\n",gsIapCtrl.sendnum);

		SendCsnrDataDeal(buf,len);											//发送数据上次的数据
	}

	return;
}
