/*******************************************************************************
 *   Filename:       main.c
 *   Revised:        All copyrights reserved to redmorningcn.
 *   Revision:       v1.0
 *   Writer:	     redmorningcn.
 *
 *   Description:
					 1、测试文件
/////////////////////////////////////////////////////////////////////////////
					 双击选中 otr 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 Otr 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   双击选中 OTR 按 Ctrl + H, 钩选 Match the case, Replace with
 *                   输入您要的名字，点击 Replace All
 *                   在 app_cfg.h 中指定本任务的 优先级  （ APP_TASK_COMM_PRIO     ）
 *                                            和 任务堆栈（ APP_TASK_COMM_STK_SIZE ）大小
 *
 *   Notes:
 *     				E-mail: redmorningcn@qq.com
 *
 *******************************************************************************/

/********************************************************************************************/
/* Include files																			*/
/********************************************************************************************/
#include "asynctmr.h"
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>
#include <userint.h>

#include <cvi_ansytimer.h>		 									//定时器变量

stcAnsyTimerMgr   TimeTmp[10];
char	var[10];
int		timeout[10];
/*---------------------------------------------------------------------------*/
/* This is the Async. timer callback routine.                                */
//	测试文件。   （redmorningcn 2017-09-23）
/*---------------------------------------------------------------------------*/
void	TestAnsyTimer(void)
{
	int		i;
	int		times = 0;

	for(i = 0; i< CVI_ANSYTIME_VAR_MAX_NUM;i++)
	{
		 
		 TimeTmp[i].flg 		= 0 ;
		 TimeTmp[i].Settime 	= (short)(100*(i+1));
		 TimeTmp[i].poverFlg 	= &var[i];
		 TimeTmp[i].ptimeout 	= &timeout[i];
		 timeout[i] = 0;
		 TimeTmp[i].startflg 	= 1;	//马上开始
	}


	cvi_InitAnsyTimer();

	times = 0;
	
	AddTimeOutVarToAnsyTimer(TimeTmp[0]);
	timeout[0] = 0;			//启动
	
	printf("start test;systime = %d\r\n",g_AnsyTime);
	while(1)
	{
		for(i =0 ;i < CVI_ANSYTIME_VAR_MAX_NUM; i++)
		{
			if(var[i] == 1 ) 				//有效
			{
				times++;
				var[i] = 0;
				printf("time[%d] Time over;systime = %d\r\n",i,g_AnsyTime);
				if(times == 4)
					AddTimeOutVarToAnsyTimer(TimeTmp[1]);

				if(times == 10)
					AddTimeOutVarToAnsyTimer(TimeTmp[2]);

				if(times == 20)
					AddTimeOutVarToAnsyTimer(TimeTmp[3]);

				timeout[i] = 0;

			}
		}
	}
}

/********************************************************************************************/
/* Application entry point.																	*/
/********************************************************************************************/
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)					/* Initialize CVI libraries */
		return -1;	/* out of memory */

	TestAnsyTimer();
	RunUserInterface ();								//运行

	return 0;
}
