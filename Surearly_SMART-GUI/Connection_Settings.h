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

#define  SUB_PANEL                        1       /* callback function: SUB_PANEL_FUNC */
#define  SUB_PANEL_sSERIAL_BAUDRATE_SEL   2       /* control type: ring, callback function: (none) */
#define  SUB_PANEL_sSERIAL_PORT_SEL       3       /* control type: ring, callback function: (none) */
#define  SUB_PANEL_TEXTMSG_2              4       /* control type: textMsg, callback function: (none) */
#define  SUB_PANEL_MONITOR_CON_SET        5       /* control type: textBox, callback function: (none) */
#define  SUB_PANEL_sBT_CLEAR_MON          6       /* control type: command, callback function: sBT_CLEAR_MON */
#define  SUB_PANEL_DECORATION_4           7       /* control type: deco, callback function: (none) */
#define  SUB_PANEL_DECORATION_3           8       /* control type: deco, callback function: (none) */
#define  SUB_PANEL_sINPUT_BLE_MAC_ADDR    9       /* control type: string, callback function: (none) */
#define  SUB_PANEL_sBT_SERIAL_PORT_OPEN   10      /* control type: textButton, callback function: sBT_SERIAL_PORT_OPEN */
#define  SUB_PANEL_TEXTMSG                11      /* control type: textMsg, callback function: (none) */
#define  SUB_PANEL_sBT_BLE_SCAN           12      /* control type: textButton, callback function: sBT_BLE_SCAN */
#define  SUB_PANEL_sBT_BLE_CONNECT        13      /* control type: command, callback function: sBT_BLE_CONNECT */
#define  SUB_PANEL_DECORATION_5           14      /* control type: deco, callback function: (none) */
#define  SUB_PANEL_TEXTMSG_3              15      /* control type: textMsg, callback function: (none) */
#define  SUB_PANEL_sBT_CLOSE              16      /* control type: command, callback function: sBT_CLOSE */
#define  SUB_PANEL_sINDI_FW_VER           17      /* control type: string, callback function: (none) */
#define  SUB_PANEL_sINDI_BLE_NAME         18      /* control type: string, callback function: (none) */
#define  SUB_PANEL_sINDI_MCU_UID          19      /* control type: string, callback function: (none) */
#define  SUB_PANEL_DEVICE_CONNECT_LED     20      /* control type: LED, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK sBT_BLE_CONNECT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sBT_BLE_SCAN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sBT_CLEAR_MON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sBT_CLOSE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK sBT_SERIAL_PORT_OPEN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SUB_PANEL_FUNC(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif