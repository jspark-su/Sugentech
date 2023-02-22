#ifndef __MAIN_H
 #define __MAIN_H

#include <userint.h>
#include <cvirte.h>
#include "Main_UI.h"

#include "Process_Inspection_Mode.h"
#include "Finished_Goods_Inspection_Mode.h"
#include "BLE_Config.h" 
#include "Serial_Config.h"
#include "Delay.h"
#include "parson.h"
#include "Command_Ctrl.h"
#include "QA_Setting_Mode.h"
#include "QA_Test_Mode.h"
#include "Test_Report.h"
#include "Connection_Settings.h"
#include "Connection_Settings_Ctrl.h"
#include "FW_Update_UI.h"
#include "FW_Update_Process.h"
#include "Read_PTR_Mode.h"


#include "Windows.h"
#include <utility.h>
#include "toolbox.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "RS232.h"

#define END         0
#define START       1

#define RESET       0
#define SET         1

#define BT_OFF      0
#define BT_ON		1

#define UNCHECKED	0	// Check Box Unchecked
#define CHECKED		1	// Check Box checked

#define LED_OFF		0
#define LED_ON		1

#define DEV_SERIAL_INIT_STR     "TEST-RND-"

typedef struct
{
  char start;
  int cnt;
  
}USER_TIMER;

typedef struct
{
  int CALIBRATION_DATA[8]; //Calibration data (DAC value)
  int INITIAL_STICK_SENSING_DATA[8]; // Unused stick PTR sensing data
  
  char BLE_NAME_STR[BLE_NAME_BUF_SIZE];
  
  float QA_STD_STICK_RAW_DATA[8];
  int QA_TOLERANCE_DATA[8];
  char QA_RESULT[8];
  
  int STD_STICK_SENSING_RAW_DATA[8];
  int STICK_SENSING_RAW_DATA[8];
  
  float STD_STICK_RATIO_DATA[8];
  
  float CHANNEL_RATIO_DATA[8];
  
  float RSLT_T1_RAWDATA;
  float RSLT_T2_RAWDATA;
  float RSLT_T3_RAWDATA;
  float RSLT_COLOR_RAWDATA;
  
}DATA_BANK_TypeDef;


extern int tabPanel_PI;
extern int tabPanel_FGI;
extern int tabPanel_QA;
extern int tabPanel_QA_Test;
extern int tabPanel_FW_Update;
extern int tabPanel_RP;

extern int mainPanel;
extern int subPanel;

extern char auto_setting_mode_state;

extern PI_VALIABLE_TypeDef PI_VALIABLE;
extern PI_VALIABLE_TypeDef QA_STICK_SENSING_MODE_VARIABLE;
extern PI_VALIABLE_TypeDef QA_CALIB_VARIABLE;

extern FGI_VALIABLE_Typedef    FGI_VALIABLE;

extern SERIAL_CONFIG_TypeDef SERIAL_CONFIG;

extern BLUETOOTH_FLAG BLE_FLAG;
extern BLUETOOTH_RX BLE_RX;

extern char ble_mac_id[BLE_MAC_BUF_SIZE];
extern char ble_name[BLE_NAME_BUF_SIZE];
extern char ble_name_check_string[BLE_NAME_BUF_SIZE];

extern int ble_rx_index;

extern DATA_BANK_TypeDef DATA_BANK;
extern TEST_REPORT_VARIABLE_Type_Def TEST_REPORT_VAR;
extern SETTING_RESULT_Type_Def SET_RESULT;

extern RX_MSG_CHECK_Typedef rx_msg_check;
extern RX_MSG_FUNCTION_Typedef rx_msg_function;

extern char stick_state_check;
extern char calibration_proc_state;

extern TIME_CHECK_TypeDef	TIME_CHECK;
extern INSPECTION_ITEMS_Typedef    INSPECTION_ITEMS;

extern char qa_stick_check;
extern char qa_proc_state;

extern char std_stick_number;

extern char production_mode_initializing;
extern char dev_uid[25];
extern char fw_ver[20];

// extern void RD_Next_Process(char func_num);

/* ---------------------------------- Read PTR Mode ---------------------------------- */
extern PI_VALIABLE_TypeDef RP_VARIABLE;
extern PRODUCT_MODE_READ_PTR_Typedef PRODUCT_MODE_PTR;
extern void RP_Process(void);

#endif /* __MAIN_H */


