/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2014. All Rights Reserved.          */
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
#define  PANEL_SELECTCOM                  2       /* control type: ring, callback function: (none) */
#define  PANEL_OPENCOM                    3       /* control type: command, callback function: OpenComCallback */
#define  PANEL_QUIT                       4       /* control type: command, callback function: QuitCallback */
#define  PANEL_PHONENUM                   5       /* control type: string, callback function: (none) */
#define  PANEL_NETCONNECT                 6       /* control type: command, callback function: NetConnectCallback */
#define  PANEL_RECEIVEMESSAGE             7       /* control type: command, callback function: RecMessageCallback */
#define  PANEL_SENDMESSAGE                8       /* control type: command, callback function: SendMessageCallback */
#define  PANEL_COMMANDBUTTON              9       /* control type: command, callback function: TelephonelCallback */
#define  PANEL_TELEPHONEL                 10      /* control type: command, callback function: TelephonelCallback */
#define  PANEL_COMTEXT                    11      /* control type: textBox, callback function: (none) */
#define  PANEL_MESSAGE                    12      /* control type: textBox, callback function: (none) */
#define  PANEL_TIMER                      13      /* control type: timer, callback function: SysTickCallback */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK NetConnectCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OpenComCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RecMessageCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendMessageCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SysTickCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TelephonelCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
