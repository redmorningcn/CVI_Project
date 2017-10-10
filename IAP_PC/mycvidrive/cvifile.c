//#include "ndpcom.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "includes.h"

int32	l_binfilehandle = 0;
uint32	l_filedealLen = 0;
uint32	l_filesize = 0;
void	GetHexFileDir(char * filedir)
{
	uint8 curparhstring[512];
	GetProjectDir(curparhstring);
	
	FileSelectPopup ("curparhstring", "*.bin", "*.txt;*.doc;*.bin", "选择下载文件", VAL_OK_BUTTON, 0, 0, 1, 0, filedir); 
	
	l_binfilehandle = OpenFile (filedir, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY); 
	
	
	GetFileInfo (filedir,&l_filesize );
	
}

void	CloseBinFile(void)
{
	CloseFile (l_binfilehandle);
}


extern	void printftexbox(uint8 *g_textboxbuf);
//
uint32	GetFileData(uint8 *buf)
{
	uint32	len = 0;	
	uint32	i;
	
	if(l_binfilehandle == 0)
		return len;
	
	
	len = ReadFile (l_binfilehandle, buf, 128);
	
	l_filedealLen += len;
	
//	printftexbox(buf);  
	return len;
}

void	StartFile(void)
{
	SetFilePtr (l_binfilehandle, 0, 0);
	l_filedealLen = 0;

}


uint8	SetFileDealPer(void)
{
	uint8	per = 0;
	
	if((l_filesize /100)&&l_filedealLen)
	{
		per	 =  l_filedealLen/(l_filesize /100) ;
	}	
	
	return per;
}


