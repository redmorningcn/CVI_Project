/*******************************************************************************
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/

#ifndef	CVI_CONFIG_H
#define	CVI_CONFIG_H

#include 	<cvi_com_operate.h>
#include 	<cvi_BspComm.h>

typedef struct _stcSysCtrl {
	stcThreadCommCtrl	PC;
} stcSysCtrl;

extern	stcSysCtrl		sCtrl;

/*******************************************************************************
 *              end of file                                                    *
 *******************************************************************************/

#endif
