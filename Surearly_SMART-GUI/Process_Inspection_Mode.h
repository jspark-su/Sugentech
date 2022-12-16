#ifndef __PI_MODE_H
 #define __PI_MODE_H

/* 'ASM' == "Auto Setting Mode" */

#define END_AUTO_SETTING_MODE       0
#define START_AUTO_SETTING_MODE     1

#define STICK_EJECTED	0
#define STICK_INSERTED	1

#define STATE_NONE		0
#define STATE_DOING		1
#define STATE_COMPLETE	2
#define STATE_FAIL		3

#define CMD_RETRY_COUNT_NUM		20
#define CMD_RETRY_TIME			5 //CMD_RETRY_TIME * 100 msec = 500 msec

#define STICK_EJECT_CHECK_COUNT     60 //STICK_EJECT_CHECK_INTERVAL_TIME * STICK_EJECT_CHECK_COUNT * 100 msec = 30 sec
#define STICK_INSERT_CHECK_COUNT    60 //STICK_INSERT_CHECK_INTERVAL_TIME * STICK_INSERT_CHECK_COUNT * 100 msec = 30 sec

#define STICK_EJECT_CHECK_INTERVAL_TIME    5 //STICK_EJECT_CHECK_INTERVAL_TIME * 100 msec = 500 msec
#define STICK_INSERT_CHECK_INTERVAL_TIME   5 //STICK_INSERT_CHECK_INTERVAL_TIME * 100 msec = 500 msec

enum
{
	PI_START = 1,
	PI_CALIBRATION_STICK_CHECK,
	PI_CALIBRATION_START,
	PI_CALIBRATION_STATE,
	PI_CALIBRATION_RESULT,
	PI_BLE_NAME_SETUP_START,
	PI_END,
};

typedef struct
{
    char one_time_process;
    char process;
	char process_state;
    char function_number;
    char cmd_check;
    char cmd_retry_count;
    char cmd_retry_timer;
	
}PI_VALIABLE_TypeDef;


typedef struct
{
  char start;
  int count;
}TIME_CHECK_TypeDef;

void PI_Process();
void PI_Next_Process(char func_num);


void PI_Calibration_Stick_Check(int panel);
void PI_Calibration_Start(int panel);
void PI_Calibration_State(int panel);
void PI_Calibration_Result(int panel);
void PI_BLE_Name_Setup_Start(int panel);
void PI_End();
void PI_Variable_Init();
void LED_Calibration_Result_Display(int panel);





#endif /* __PI_MODE_H*/
