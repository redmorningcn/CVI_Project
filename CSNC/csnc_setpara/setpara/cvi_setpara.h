/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  SETP_PANEL                       1       /* callback function: SetParaPanelCB */
#define  SETP_PANEL_DECORATION_14         2       /* control type: deco, callback function: (none) */
#define  SETP_PANEL_DECORATION_13         3       /* control type: deco, callback function: (none) */
#define  SETP_PANEL_DECORATION_12         4       /* control type: deco, callback function: (none) */
#define  SETP_PANEL_DECORATION_11         5       /* control type: deco, callback function: (none) */
#define  SETP_PANEL_DECORATION_10         6       /* control type: deco, callback function: (none) */
#define  SETP_PANEL_QUIT                  7       /* control type: command, callback function: QuitCallback */
#define  SETP_PANEL_SETLOCO_4             8       /* control type: command, callback function: SetLocoCallBack */
#define  SETP_PANEL_SETLOCO_3             9       /* control type: command, callback function: SetSelCallBack */
#define  SETP_PANEL_LOCONUM               10      /* control type: numeric, callback function: (none) */
#define  SETP_PANEL_LOCOTYPE              11      /* control type: numeric, callback function: (none) */
#define  SETP_PANEL_SETTIME               12      /* control type: command, callback function: SetTimeCallBack */
#define  SETP_PANEL_SETLOCO_2             13      /* control type: command, callback function: SetHigCallBack */
#define  SETP_PANEL_SEL                   14      /* control type: numeric, callback function: (none) */
#define  SETP_PANEL_RESTART               15      /* control type: command, callback function: RstSysCallBack */
#define  SETP_PANEL_SETREADDATA_3         16      /* control type: command, callback function: ClearAllCallBack */
#define  SETP_PANEL_SETCALCPARA           17      /* control type: command, callback function: SetCalcParaCallBack */
#define  SETP_PANEL_HIG                   18      /* control type: numeric, callback function: (none) */
#define  SETP_PANEL_DATEANDTIME           19      /* control type: textMsg, callback function: CardStaticIndicateCallBack */
#define  SETP_PANEL_CALCPARA              20      /* control type: numeric, callback function: (none) */
#define  SETP_PANEL_DECORATION_16         21      /* control type: deco, callback function: (none) */
#define  SETP_PANEL_CALCPARA_             22      /* control type: textMsg, callback function: (none) */
#define  SETP_PANEL_STORE_STYPE           23      /* control type: binary, callback function: SetStoreRecordRecCallback */
#define  SETP_PANEL_SETPARA_TIMER         24      /* control type: timer, callback function: SetParaTimerCallback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK CardStaticIndicateCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ClearAllCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RstSysCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetCalcParaCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetHigCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetLocoCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetParaPanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetParaTimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetSelCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetStoreRecordRecCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetTimeCallBack(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
