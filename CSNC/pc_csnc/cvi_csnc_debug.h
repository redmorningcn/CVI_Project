/*******************************************************************************
 *   Filename:       cvi_bspComm.h
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *   DATE:		     20170927
 *   Notes:
 *     				 E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/
#ifndef	CVI_CVI_CSNC_DEBUG_H
#define	CVI_CVI_CSNC_DEBUG_H   

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>
#include <formatio.h>

#include "cvi_com1.h"
#include "cvi_config.h"  
#include "cvi_bspComm.h"    
#include "cvi_Comm.h"    


/********************************************************************************************/
// Constants
/********************************************************************************************/
#define		COMM_DATA_BUF_SZIE		512
#define		MASTE_ADDR_HOST			0xCA
#define		MNAI_PANEL_FILE			"cvi_main_myinfo.ini"      
#define		UIR_CSNCDEBUG_NAME   	"csnc_debug.uir"


/********************************************************************************************/
// struct
/********************************************************************************************/
//保存面板属性
typedef	struct	_stcmianpanelpara{
	struct	_csnchead_
	{
		char	source;
		char	denst;
		char 	code;
		char	framenum;
	}stccsnchead ;
	
	struct	_sendpara_
	{
		int		sendstyle;		//数据类型  0 string：1： hex
		int		recvstyle;	   	//数据类型  0 string：1： hex    
		int	 	autosend;		//定时发送	0 no；1：yes		
		int		autotime;		//定时间隔  X 10ms
	}sendpara ;	

	char	sentbuf[256];
}stcmianpanelpara;


/********************************************************************************************/
// globe
/********************************************************************************************/
extern	int 	gCsncDebugPanelHandle;

/********************************************************************************************/
//载并显示面板。	redmoringcn 20170927						            
/********************************************************************************************/
void LoadCsncDebugPanel(void);



#endif
