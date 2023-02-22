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

#define  MAIN_PANEL                       1       /* callback function: MAIN_PANEL_FUNC */
#define  MAIN_PANEL_TAB_SETTING_MODE      2       /* control type: tab, callback function: TAB_SETTING_MODE_FUNC */

     /* tab page panel controls */
#define  TABPANEL_INPUT_MANUF_NAME        2       /* control type: string, callback function: (none) */
#define  TABPANEL_BT_CLEAR_MON            3       /* control type: command, callback function: BT_CLEAR_MON */
#define  TABPANEL_LOG_MONITOR_PI          4       /* control type: textBox, callback function: (none) */
#define  TABPANEL_TEXTMSG_16              5       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_BT_SAVE_REPORT_PI       6       /* control type: command, callback function: BT_SAVE_REPORT_PI */
#define  TABPANEL_TIMER_1msec             7       /* control type: timer, callback function: TIMER_1msec */
#define  TABPANEL_TIMER_100msec           8       /* control type: timer, callback function: TIMER_100msec */
#define  TABPANEL_PICTURE                 9       /* control type: picture, callback function: (none) */
#define  TABPANEL_TIMER_10msec            10      /* control type: timer, callback function: TIMER_10msec */
#define  TABPANEL_BT_AUTO_SETTINGS        11      /* control type: textButton, callback function: BT_AUTO_SETTINGS */
#define  TABPANEL_BT_CONNECT_SETTINGS     12      /* control type: command, callback function: BT_CONNECT_SETTINGS */

     /* tab page panel controls */
#define  TABPANEL_2_INPUT_DEV_SERIAL_NUM  2       /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_BLE_MAC_ADDR    3       /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_INSPECTOR_NAME  4       /* control type: string, callback function: (none) */
#define  TABPANEL_2_LOG_MONITOR_FGI       5       /* control type: textBox, callback function: (none) */
#define  TABPANEL_2_BT_RESET_ITEM_FGI     6       /* control type: command, callback function: BT_RESET_ITEM_FGI */
#define  TABPANEL_2_BT_CLEAR_MONITOR_FGI  7       /* control type: command, callback function: BT_CLEAR_MONITOR_FGI */
#define  TABPANEL_2_TEXTMSG_32            8       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_39            9       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_37            10      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_BT_SSN1_CAL           11      /* control type: textButton, callback function: BT_SSN1_CAL */
#define  TABPANEL_2_TEXTMSG_38            12      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_34            13      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_46            14      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_45            15      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_44            16      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_43            17      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_42            18      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_57            19      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_58            20      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_55            21      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_56            22      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_53            23      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_54            24      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_51            25      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_52            26      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_49            27      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_50            28      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_48            29      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_41            30      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_36            31      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_33            32      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_64            33      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_27            34      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_22            35      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_25            36      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_21            37      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_24            38      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_20            39      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_23            40      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_30            41      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_29            42      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_28            43      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_19            44      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_SPLITTER_29           45      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_27           46      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_26           47      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_25           48      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_24           49      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_23           50      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_22           51      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_21           52      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_11           53      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_7            54      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_12           55      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_9            56      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_10           57      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_3            58      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_13           59      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_19           60      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_18           61      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_17           62      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_30           63      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_16           64      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_15           65      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_14           66      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_28           67      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_6            68      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_5            69      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_SPLITTER_2            70      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSN1_RSLT   71      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM5_RSLT   72      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM4_RSLT   73      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM3_RSLT   74      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM2_RSLT   75      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM1_RSLT   76      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSN1_RSLT   77      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM5_RSLT   78      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM4_RSLT   79      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM3_RSLT   80      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM2_RSLT   81      /* control type: string, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM1_RSLT   82      /* control type: string, callback function: (none) */
#define  TABPANEL_2_SPLITTER_4            83      /* control type: splitter, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_47            84      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_35            85      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_DECORATION            86      /* control type: deco, callback function: (none) */
#define  TABPANEL_2_BT_SSN1_DATA_READ     87      /* control type: command, callback function: BT_SSN1_DATA_READ */
#define  TABPANEL_2_BT_SSM5_DATA_READ     88      /* control type: command, callback function: BT_SSM5_DATA_READ */
#define  TABPANEL_2_BT_SSM4_DATA_READ     89      /* control type: command, callback function: BT_SSM4_DATA_READ */
#define  TABPANEL_2_BT_SSM3_DATA_READ     90      /* control type: command, callback function: BT_SSM3_DATA_READ */
#define  TABPANEL_2_BT_SSM2_DATA_READ     91      /* control type: command, callback function: BT_SSM2_DATA_READ */
#define  TABPANEL_2_TEXTMSG_40            92      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_BT_SSM1_DATA_READ     93      /* control type: command, callback function: BT_SSM1_DATA_READ */
#define  TABPANEL_2_INS_INAPPRO_CHKBOX5   94      /* control type: radioButton, callback function: INS_INAPPRO_CHKBOX5 */
#define  TABPANEL_2_INS_APPRO_CHKBOX5     95      /* control type: radioButton, callback function: INS_APPRO_CHKBOX5 */
#define  TABPANEL_2_INS_INAPPRO_CHKBOX4   96      /* control type: radioButton, callback function: INS_INAPPRO_CHKBOX4 */
#define  TABPANEL_2_INS_APPRO_CHKBOX4     97      /* control type: radioButton, callback function: INS_APPRO_CHKBOX4 */
#define  TABPANEL_2_INS_INAPPRO_CHKBOX3   98      /* control type: radioButton, callback function: INS_INAPPRO_CHKBOX3 */
#define  TABPANEL_2_INS_APPRO_CHKBOX3     99      /* control type: radioButton, callback function: INS_APPRO_CHKBOX3 */
#define  TABPANEL_2_INS_INAPPRO_CHKBOX2   100     /* control type: radioButton, callback function: INS_INAPPRO_CHKBOX2 */
#define  TABPANEL_2_INS_APPRO_CHKBOX2     101     /* control type: radioButton, callback function: INS_APPRO_CHKBOX2 */
#define  TABPANEL_2_INS_INAPPRO_CHKBOX1   102     /* control type: radioButton, callback function: INS_INAPPRO_CHKBOX1 */
#define  TABPANEL_2_INS_APPRO_CHKBOX1     103     /* control type: radioButton, callback function: INS_APPRO_CHKBOX1 */
#define  TABPANEL_2_BT_CONNECT_SETTINGS   104     /* control type: command, callback function: BT_CONNECT_SETTINGS */
#define  TABPANEL_2_SSN1_RSLT_LED         105     /* control type: LED, callback function: (none) */
#define  TABPANEL_2_SSM5_RSLT_LED         106     /* control type: LED, callback function: (none) */
#define  TABPANEL_2_SSM4_RSLT_LED         107     /* control type: LED, callback function: (none) */
#define  TABPANEL_2_SSM3_RSLT_LED         108     /* control type: LED, callback function: (none) */
#define  TABPANEL_2_SSM2_RSLT_LED         109     /* control type: LED, callback function: (none) */
#define  TABPANEL_2_CAL_RSLT_LED          110     /* control type: LED, callback function: (none) */
#define  TABPANEL_2_SSM1_RSLT_LED         111     /* control type: LED, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_31            112     /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_59            113     /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_BT_SAVE_REPORT_FGI    114     /* control type: command, callback function: BT_SAVE_REPORT_FGI */
#define  TABPANEL_2_TEXTMSG_26            115     /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_INPUT_SSN1_RANG_BELOW 116     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSN1_RANG_ABOVE 117     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM5_RANG_BELOW 118     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM5_RANG_ABOVE 119     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM4_RANG_BELOW 120     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM4_RANG_ABOVE 121     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM3_RANG_BELOW 122     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM3_RANG_ABOVE 123     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM2_RANG_BELOW 124     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM2_RANG_ABOVE 125     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM1_RANG_BELOW 126     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_SSM1_RANG_ABOVE 127     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSN1_LEVEL  128     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM5_LEVEL  129     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM4_LEVEL  130     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM3_LEVEL  131     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM2_LEVEL  132     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSN1_LEVEL  133     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM5_LEVEL  134     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM4_LEVEL  135     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM3_LEVEL  136     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM2_LEVEL  137     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM1_LEVEL  138     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_REF_SSM1_LEVEL  139     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSN1_RAW    140     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM5_RAW    141     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM4_RAW    142     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM3_RAW    143     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM2_RAW    144     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_INPUT_MEA_SSM1_RAW    145     /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_PICTURE_2             146     /* control type: picture, callback function: (none) */
#define  TABPANEL_2_PICTURE               147     /* control type: picture, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_60            148     /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_62            149     /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_66            150     /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_61            151     /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_CB_SSN1_READ          152     /* control type: radioButton, callback function: CB_SSN1_READ */

     /* tab page panel controls */
#define  TABPANEL_3_BT_CONNECT_SETTINGS   2       /* control type: command, callback function: BT_CONNECT_SETTINGS */
#define  TABPANEL_3_BT_ENTER_IAP_MODE     3       /* control type: command, callback function: BT_ENTER_IAP_MODE */
#define  TABPANEL_3_CHKBOX_FORCE_FW_UPD   4       /* control type: radioButton, callback function: CHKBOX_FORCE_FW_UPD */
#define  TABPANEL_3_BT_FW_FILE_OPEN       5       /* control type: command, callback function: BT_FW_FILE_OPEN */
#define  TABPANEL_3_BT_FW_FILE_SEND       6       /* control type: textButton, callback function: BT_FW_FILE_SEND */
#define  TABPANEL_3_INDI_FW_DOWNLOADING   7       /* control type: scale, callback function: (none) */
#define  TABPANEL_3_INDI_FW_DOWN_PERCENT  8       /* control type: string, callback function: (none) */
#define  TABPANEL_3_LOG_MONITOR_FW        9       /* control type: textBox, callback function: (none) */
#define  TABPANEL_3_BT_CLEAR_MON_FW       10      /* control type: command, callback function: BT_CLEAR_MON_FW */
#define  TABPANEL_3_INDI_FW_FILE_NAME     11      /* control type: string, callback function: (none) */
#define  TABPANEL_3_DECORATION            12      /* control type: deco, callback function: (none) */
#define  TABPANEL_3_TEXTMSG               13      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_4_BT_CONNECT_SETTINGS   2       /* control type: command, callback function: BT_CONNECT_SETTINGS */
#define  TABPANEL_4_BT_CLEAR_MON_RAW      3       /* control type: command, callback function: BT_CLEAR_MON_RAW */
#define  TABPANEL_4_LOG_MONITOR_RAW       4       /* control type: textBox, callback function: (none) */
#define  TABPANEL_4_BT_READ_CAL           5       /* control type: textButton, callback function: BT_READ_CAL */
#define  TABPANEL_4_BT_READ_PTR           6       /* control type: textButton, callback function: BT_READ_PTR */
#define  TABPANEL_4_IMG_PCB               7       /* control type: picture, callback function: (none) */
#define  TABPANEL_4_RING_SEL_CH           8       /* control type: ring, callback function: (none) */
#define  TABPANEL_4_SLIDE_DAC_INTERVAL    9       /* control type: scale, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_9_BT_QA_CLIB_START      2       /* control type: command, callback function: BT_QA_CLIB_START */
#define  TABPANEL_9_BT_READ_STICK_RAW     3       /* control type: command, callback function: BT_READ_STICK_RAW */
#define  TABPANEL_9_MONITOR_QA_TEST       4       /* control type: textBox, callback function: (none) */
#define  TABPANEL_9_MONITOR_QA_TEST_CLEAR 5       /* control type: command, callback function: MONITOR_QA_TEST_CLEAR */
#define  TABPANEL_9_BT_CONNECT_SETTINGS   6       /* control type: command, callback function: BT_CONNECT_SETTINGS */


     /* Control Arrays: */

#define  BLE_CONNECTION                   1
#define  BLE_NAME                         2
#define  CONFIG_FILE                      3
#define  CTRLARRAY                        4
#define  CTRLARRAY_2                      5
#define  CTRLARRAY_3                      6
#define  CTRLARRAY_4                      7
#define  CTRLARRAY_5                      8
#define  CTRLARRAY_6                      9
#define  DEVICE_INFORMATION               10
#define  PRODUCTION_MODE                  11
#define  PRODUCTION_MODE_AUTO_START       12
#define  SERIAL_CONNECTION                13
#define  SERIAL_MONITOR                   14
#define  TEST_REPORT                      15

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BT_AUTO_SETTINGS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_CLEAR_MON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_CLEAR_MON_FW(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_CLEAR_MON_RAW(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_CLEAR_MONITOR_FGI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_CONNECT_SETTINGS(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_ENTER_IAP_MODE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_FW_FILE_OPEN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_FW_FILE_SEND(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_QA_CLIB_START(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_READ_CAL(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_READ_PTR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_READ_STICK_RAW(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_RESET_ITEM_FGI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SAVE_REPORT_FGI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SAVE_REPORT_PI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SSM1_DATA_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SSM2_DATA_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SSM3_DATA_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SSM4_DATA_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SSM5_DATA_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SSN1_CAL(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BT_SSN1_DATA_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CB_SSN1_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CHKBOX_FORCE_FW_UPD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_APPRO_CHKBOX1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_APPRO_CHKBOX2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_APPRO_CHKBOX3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_APPRO_CHKBOX4(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_APPRO_CHKBOX5(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_INAPPRO_CHKBOX1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_INAPPRO_CHKBOX2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_INAPPRO_CHKBOX3(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_INAPPRO_CHKBOX4(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK INS_INAPPRO_CHKBOX5(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MAIN_PANEL_FUNC(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MONITOR_QA_TEST_CLEAR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TAB_SETTING_MODE_FUNC(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_100msec(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_10msec(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_1msec(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif