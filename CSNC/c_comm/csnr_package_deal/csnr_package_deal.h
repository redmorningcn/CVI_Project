/*******************************************************************************
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/
#ifndef  _csnr_package_deal_h_
#define  _csnr_package_deal_h_

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/


/********************************************************************************************/
// Constants
/********************************************************************************************/
#define     FRAM_HERD0     		0x10        //报文头
#define     FRAM_HERD1     		0x28       	//报文头				

#define    	FRAM_END0     		0x10        //报文尾
#define     FRAM_END1     		0x2C       	//报文尾	
	   
#define		RS485_CHK_SUM		0x02		//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define		RS485_CHK_CRC		0x01		//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 
#define		RS485_CHK_RESUM		0x03		//b0001：CRC方式；b0010：累加和方式； b0011;累加和二进制补码方式 


/********************************************************************************************/
// struct
/********************************************************************************************/
typedef	struct _stcCsnrProtocolPara_
{
	unsigned char	sourceaddr;
	unsigned char 	destaddr;
	unsigned char 	framnum;
	unsigned char 	framcode;
	unsigned char *	databuf;	  	//数据区
	unsigned char *	rxtxbuf;		//收发缓冲区
	unsigned char 	datalen;
	unsigned char 	rxtxlen;
	unsigned char 	tmp[2];
}stcCsnrProtocolPara;	

/********************************************************************************************/
/* Globals																					*/
/********************************************************************************************/
//数据打包，根据stcCsnrProtocolPara信息，将数据打包成CNSR数据
//
extern	void	DataPackage_CSNC(stcCsnrProtocolPara * sprotocolpara);

//数据解包，将接受到的数据解包到，stcCsnrProtocolPara信息
//简析正确，返回1；否则，返回0
extern unsigned char   DataUnpack_CSNC(stcCsnrProtocolPara *sprotocolpara);

#endif
