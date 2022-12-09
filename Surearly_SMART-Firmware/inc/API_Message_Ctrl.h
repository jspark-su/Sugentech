#ifndef __API_MESSAGE_CTRL_H
 #define __API_MESSAGE_CTRL_H

#include "Parson.h"
#include "Time_Ctrl.h"

//API (string) array length
#define API_CMD_LEN             13
#define API_STICK_COLOR_LEN     1
#define API_COVID_CONTENT_LEN   4
#define API_TEST_STATE_LEN      6
#define API_STICK_STATE_LEN     12
#define API_REACT_CHK_LEN       9
#define API_CONTENT_LEN         6
#define API_DIP_LEN             4
#define API_LOT_VER_LEN         10
#define API_T1_CUTOFF_LEN       5
#define API_T2_CUTOFF_LEN       5
#define API_T3_CUTOFF_LEN       5
#define API_RAWDATA_LEN         3

//API produnction mode
#define API_PRODUCT_MODE_STATE_LEN      6
#define API_CALIB_STATE_LEN             6
#define API_CALIB_STICK_STATE_LEN       7
#define API_CALIB_PROC_STATE_LEN        9
#define API_CORRECTION_FACTOR_ARRY_CNT  3
#define API_REACT_TIME_ARRY_CNT         5
#define API_BLE_NAME_LEN                21
#define API_RAW_DATA_TEST_STATE_LEN     6
#define API_RAW_DATA_STATE_LEN          9
#define API_DEV_MODE_SEL_LEN            12
#define API_BLE_NAME_SETUP_STATE_LEN    6
#define API_BLE_NAME_STRING_LEN         21
//

typedef struct
{
  char stick_color; //App으로부터 수신된 stick color 정보
  char stick_color_detect; //Device에 삽입된 stick의 color 정보
  int react_time;
  int stick_insert_wait_time;
  char proc_step;
  char test_state[API_TEST_STATE_LEN];
  char stick_state[API_STICK_STATE_LEN];
  int fin_time;
  int cur_react_time;
  char react_chk[API_REACT_CHK_LEN];
  float rslt_rawdata[API_RAWDATA_LEN];
  char rslt_level;
  char rslt_level_recv_chk;
  char product_mode_state[API_PRODUCT_MODE_STATE_LEN];
  char calib_stick_stat[API_CALIB_STICK_STATE_LEN];
  char calib_proc_stat[API_CALIB_PROC_STATE_LEN];
  char raw_data_state[API_RAW_DATA_STATE_LEN];
  float channel_ratio_data[6];//Channel 1~5 ratio rawdata
  char ble_name_setup_state[API_BLE_NAME_SETUP_STATE_LEN];
  char ble_name_str[API_BLE_NAME_STRING_LEN];
}API_DATA_TypeDef;

enum //API CMD SET
{
  DEV_INFO_CMD = 1,
  STAT_CHK_CMD,
  DEV_RST_CMD,
  RSLT_DATA_CMD,
  PRODUCT_MODE_CMD,
  CALIB_STAT_CMD,
  CALIB_RSLT_CMD,
  CORR_FACTOR_CMD,
  BLE_NAME_CMD,
  RAW_DATA_CMD,
  DEV_MODE_SEL_CMD,
  FW_UPD_CMD,
  READ_PTR_CMD,
  ERROR_CMD,
};
//

enum //API DATA-KEY SET
{
  TEST_STATE_KEY = 1,      
  STICK_STATE_KEY,     
  REACT_CHK_KEY,       
  CALIB_STICK_STATE_KEY,
  CALIB_PROC_STATE_KEY,
  RAW_DATA_STATE_KEY,
};

enum //API DATA-VALUE SET
{
  NONE_VAL = 0,
  START_VAL,
  END_VAL,
  FAIL_VAL,
  NORM_VAL,
  ERR_CONTENT_VAL,
  ERR_USED_VAL,
  ERR_EJECT_VAL,
  ERR_CTRL_VAL,
  DOING_VAL,
  COMPLETE_VAL,
  CHANG_STICK_VAL,
  RESTART_TEST_VAL,
  OFF_VAL,
  ON_VAL,
  CALIB_STICK_EJECTED_VAL,
  CALIB_STICK_INSERTED_VAL,
};


void API_Rx_Message_Parsing();
void API_Tx_Message(const JSON_Object *object, char cmd, uint16_t code);
void API_Key_Value_Init(API_DATA_TypeDef* p_data);
void API_Key_Value_Setting(API_DATA_TypeDef* p_data, char key, char value);
void Get_Channel_Ratio_And_Raw_data(API_DATA_TypeDef *P_API_DATA);
#endif /* __API_MESSAGE_CTRL_H*/