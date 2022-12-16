#ifndef __MAIN_H
 #define __MAIN_H

#define FW_VER  "2.0.18_alpha.2"

#define END                           0
#define START                         1
#define READY                         2
#define BUSY                          3

#define DEV_SLEEP                     1
#define DEV_WAKEUP                    2

#define SW_POWER_ON_PRESS_TIME        1000 //ms
#define SW_POWER_OFF_PRESS_TIME       2000 //ms
#define SW_FACTORY_RESET_PRESS_TIME   15000 //ms

#define TIMER_MAX_COUNT               60000

#define TIMER_VAR_UPDATE_NONE         0
#define TIMER_VAR_UPDATE_COMPLETE     1
#define TIMER_VAR_UPDATE_BUSY         2

#define TIMER_COUNT_DISABLE           0
#define TIMER_COUNT_ENABLE            1

#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "CircularQueue.h"
#include "Delay.h"
#include "Device_Setup.h"
#include "Device_Initial_Check.h"

#include "Battery_Ctrl.h"
#include "BLE_BoT_nLE521.h"
#include "BLE_RF_BM_4044B4.h"
#include "BLE_Ctrl.h"
#include "API_Message_Ctrl.h"
#include "LCD_Ctrl.h"
#include "LED_Ctrl.h"
#include "Memory_Ctrl.h"
#include "PTR_Ctrl.h"
#include "Temperature_Ctrl.h"
#include "Admin_Mode.h"
#include "Uart_Ctrl.h"
#include "Time_Ctrl.h"
#include "User_Mode.h"
#include "Parson.h"
#include "Production_Mode.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct
{
  char start;
  int count;
  char update_block; //Polling 함수에서 USER_TIMER 변수 접근 시 우선 순위가 높은 작업에 의해 값이 변경 되는 것을 막기 위해 block 처리 변수 사용
}USER_TIMER;

extern USER_MODE_PROCESS_TypeDef USER_MODE_PROCESS;
extern USER_TIMER  TIMER_100ms;
extern USER_TIMER  TIMER_500ms;
extern API_DATA_TypeDef  API_DATA;
extern BLUETOOTH_FLAG BLE_FLAG;
extern BLUETOOTH_RX BLE_RX;
extern BLUETOOTH_TX BLE_TX;

extern CQueue BLE_BOT_QUEUE;
//extern CQueue BLE_RF_QUEUE;

extern MEMORY_BANK MEM_BANK_1; // User mode - Data/Parameter 저장 메모리

extern SWITCH_CTRL_Typedef POWER_SWITCH;
extern SWITCH_CTRL_Typedef STRIP_CHECK_SWITCH;

extern volatile char device_mode_select; //system 작동 모드 선택 (main/demo/test mode)
extern volatile char dev_power_status;
extern char forced_power_down;

extern char mcu_uid[25];
//extern char ble_mac_id[BLE_MAC_ADDR_BUF_SIZE];
extern char ble_mac_id[BLE_MAC_ADDR_LEN];
extern char ble_name[BLE_NAME_LEN];
extern uint8_t ble_uart_block;
extern uint16_t ble_rx_index;

extern uint16_t calibration_data[8];

extern uint16_t initial_band_sensing_data[6];
extern uint16_t recent_band_sensing_data[6];
extern uint16_t initial_color_sensing_data[2];
extern uint16_t recent_color_sensing_data[2];
extern const uint16_t ptr_num[7];
extern const uint8_t led_num[8];
extern double y_target;

extern char result_display_enable;

//extern char memory_save_process;

extern uint8_t vbat_update;
extern uint8_t vbat_status;

extern uint8_t factory_reset_cnt;

extern USER_TIMER TIMER_COUNT_UP_1;
extern USER_TIMER TIMER_COUNT_UP_2;
extern USER_TIMER TIMER_COUNT_DOWN_1;

extern int sample_react_step[5];
extern char cur_sample_reaction_progress_step;

extern char once_operation;

extern PRODUCT_MODE_VAL_TypeDef PRODUCT_MODE_VAL;
extern PRODUCT_MODE_PROCESS_TypeDef PRODUCT_MODE_PROCESS;
extern PRODUCT_MODE_READ_PTR_Typedef PRODUCT_MODE_PTR;

extern uint8_t calibration_check;

extern uint8_t check_sample_flow_process;

void Timer_Count_Setup(USER_TIMER *TIMER_COUNT, int init_count_value, char enable_status);

extern uint8_t fw_update_mode_execution;

#endif /* __MAIN_H*/