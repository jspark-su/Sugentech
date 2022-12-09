#ifndef __ADMIN_MODE_H
 #define __ADMIN_MODE_H
#include "stm8l15x.h"

#define ADMIN_MENU_DISPLAY_OFF            0
#define ADMIN_MENU_DISPLAY_ON             1

#define BAND_CUTOFF_DATA_NUM              5 //라인별 Cutoff 개수 ex) level 1~5
#define CONTENT_CUTOFF_DATA_NUM           3 //Color 인식 Cutoff 개수 ex level 1~3

#define ADMIN_SAVE_OK                     1
#define ADMIN_SAVE_NO                     2


enum //Main menu list
{
  MAIN_MENU_VIEW = 0,
  MAIN_MEMORY_BANK_ERASE,
  MAIN_READ_MEMORY_BANK,
  MAIN_DEVICE_INFORMATION,
/*
  MAIN_LED_CALIBRATION,
  MAIN_STICK_CUTOFF_SETTING,
  MAIN_CORRECTION_FACTOR_SETTING,
  MAIN_REACT_TIME_SETTING,
  MAIN_BLE_NAME_SETTING,
  MAIN_W_FIRMWARE_UPDATE,  
  MAIN_READ_STICK_RAW_DATA,
  MAIN_MULTI_TIME_TEST,
*/  
};


enum //Sub menu list
{
  SUB_MEMORY_BANK_ERASE = 1,
  SUB_INIT_OPERATION_MODE,
  
/*
  SUB_SAVE_FACTORY_DATA,
  SUB_MULTI_TIME_TEST_PROC,
  SUB_CUTOFF_CONTENT_SELECT,
  SUB_CUTOFF_DATA_SETTING,  
  SUB_W_FIRMWARE_UPDATE,  
  SUB_BLE_NAME_SETTING,
  SUB_REACT_TIME_CONTENT_SELECT,
  SUB_REACT_TIME_SETTING,
  SUB_CORRECTION_FACTOR_CONTENT_SELECT,
  SUB_CORRECTION_FACTOR_SETTING,
*/
};

/*
enum //Cutoff Data Setting Type
{
  T1_CUTOFF_SET = 1,
  T2_CUTOFF_SET,
  T3_CUTOFF_SET,
};
*/

enum //Save Factory Data Type
{
  CALIBRATION_SAVE_FACTORYDATA = 1,
  CUTOFFDATA_SAVE_FACTORYDATA,
  BLE_NAME_SAVE_FACTORYDATA,
  INIT_OPERATION_MODE_SAVE_FACTORYDATA,
  REACT_TIME_SAVE_FACTORYDATA,
  CORRECTION_FACTOR_SAVE_FACTORYDATA,
};

void Admin_BLE_Rx_Process();
void Admin_Menu_Display();
void Admin_Menu_Process();
void Admin_LED_Calibration();
void Admin_Multi_Time_Test();
void Admin_Multi_Time_Test_Display();
void Admin_Device_Information();
void Admin_Variable_Init();
void Admin_Cutoff_Content_Select();
void Admin_Cutoff_Data_Setting();
void Admin_Read_Memory_Bank();
void Admin_Erase_Memory_Bank();
void Admin_Read_Stick_Rawdata();
void Admin_Wireless_FW_Update();
void Admin_Save_Factory_data();
void Admin_BLE_Name_Setting();
void Admin_Reacttime_Content_Select();
void Admin_Reacttime_Setting();
void Admin_Correction_Factor_Content_Select();
void Admin_Correction_Factor_Setting();
void Admin_Init_Operation_Mode();
void Run_Adminmode();
//void admin_color_cutoff_setting();
//void admin_current_time();
//void tester_cutoff_line_select();
//void admin_rtc_date_settting();
//void admin_rtc_time_settting();
//void admin_read_receive_buffer();
//void admin_one_time_display();
//void admin_color_test();
#endif /* __ADMIN_MODE_H*/