#include <userint.h>
#include	"includes.h"

void  refreshComtext(int panel,int id)
{
	char 	recBuf[256];
	int		recLen;
	
	if(GetOpenCom0Flg())
	{
		recLen = ReadCom0(recBuf,sizeof(recBuf));
	
		if(recLen)
		{
			recBuf[recLen] = 0;
			SetCtrlVal(panel,id,recBuf);
		}
	}
}

