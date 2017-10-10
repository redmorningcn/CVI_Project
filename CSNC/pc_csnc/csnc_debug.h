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

#define  CSNC_DEBUG                       1       /* callback function: PanelCB */
#define  CSNC_DEBUG_COM1_SEND_DATA        2       /* control type: command, callback function: SendCsncDataCallback */
#define  CSNC_DEBUG_OPENCOM1              3       /* control type: command, callback function: OpenCom1Callback */
#define  CSNC_DEBUG_QUIT_2                4       /* control type: command, callback function: ClearTextBoxCallback */
#define  CSNC_DEBUG_QUIT                  5       /* control type: command, callback function: Quit */
#define  CSNC_DEBUG_TIMER                 6       /* control type: timer, callback function: SysTickCallback */
#define  CSNC_DEBUG_INFOTEXTBOX           7       /* control type: textBox, callback function: (none) */
#define  CSNC_DEBUG_DECORATION_2          8       /* control type: deco, callback function: (none) */
#define  CSNC_DEBUG_SOURCEADDR            9       /* control type: numeric, callback function: (none) */
#define  CSNC_DEBUG_FRAMCODE              10      /* control type: numeric, callback function: (none) */
#define  CSNC_DEBUG_DESTADDR              11      /* control type: numeric, callback function: (none) */
#define  CSNC_DEBUG_DELAYMS               12      /* control type: numeric, callback function: (none) */
#define  CSNC_DEBUG_FRAMNUM               13      /* control type: numeric, callback function: (none) */
#define  CSNC_DEBUG_TEXTMSG_2             14      /* control type: textMsg, callback function: (none) */
#define  CSNC_DEBUG_COM1SENDTEXTBOX       15      /* control type: string, callback function: (none) */
#define  CSNC_DEBUG_RECVFMT               16      /* control type: radioButton, callback function: (none) */
#define  CSNC_DEBUG_SENDFMT               17      /* control type: radioButton, callback function: (none) */
#define  CSNC_DEBUG_TEXTMSG_3             18      /* control type: textMsg, callback function: (none) */
#define  CSNC_DEBUG_TEXTMSG_4             19      /* control type: textMsg, callback function: (none) */
#define  CSNC_DEBUG_DECORATION            20      /* control type: deco, callback function: (none) */
#define  CSNC_DEBUG_TEXTMSG               21      /* control type: textMsg, callback function: (none) */
#define  CSNC_DEBUG_DECORATION_3          22      /* control type: deco, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK ClearTextBoxCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OpenCom1Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PanelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SendCsncDataCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SysTickCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
