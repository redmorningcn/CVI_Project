#ifndef  _LocoModTypeDef_h_
#define  _LocoModTypeDef_h_

//TAXII前-------------------------------------------------------------------------------------
typedef struct _stcLocoModNum_     //
{
    uint32  LocoType;      		//机车类型
    uint32  LocoNum;       		//机车编号
    uint32  ModeNum;      		//模型编号
    uint8   LocoStr[12];      	//机车号  
}stcLocoModNum;

#endif
