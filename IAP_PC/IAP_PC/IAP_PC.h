/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2015. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_SELECTDIP1COM              2       /* control type: ring, callback function: (none) */
#define  PANEL_DIP1COMBOUT                3       /* control type: ring, callback function: (none) */
#define  PANEL_OPENDIP1COM                4       /* control type: command, callback function: OpenComCallback */
#define  PANEL_QUIT_2                     5       /* control type: command, callback function: ClearTextBoxCallback */
#define  PANEL_QUIT                       6       /* control type: command, callback function: QuitCallback */
#define  PANEL_TIMER                      7       /* control type: timer, callback function: SysTickCallback */
#define  PANEL_INFOTEXTBOX                8       /* control type: textBox, callback function: (none) */
#define  PANEL_BINPAHT                    9       /* control type: string, callback function: (none) */
#define  PANEL_COMMANDBUTTON              10      /* control type: command, callback function: OpenBinFileCallback */
#define  PANEL_DECORATION                 11      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_2               12      /* control type: deco, callback function: (none) */
#define  PANEL_DECORATION_3               13      /* control type: deco, callback function: (none) */
#define  PANEL_TEXTMSG_2                  14      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG                    15      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_4                  16      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_3                  17      /* control type: textMsg, callback function: (none) */
#define  PANEL_PRE                        18      /* control type: scale, callback function: (none) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ClearTextBoxCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OpenBinFileCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OpenComCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SysTickCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
