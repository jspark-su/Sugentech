#ifndef __USER_MODE_H
 #define __USER_MODE_H


#define DEV_APP_CONNECTION_MODE         0
#define DEV_ADMIN_MODE                  1
#define DEV_PRODUCTION_MODE             2

#define NONE            0

#define TEST_NONE       0
#define TEST_START      1
#define TEST_END        2

#define STICK_COLOR_NONE        0
#define STICK_COLOR_RED         1
#define STICK_COLOR_GREEN       2
#define STICK_COLOR_YELLOW      3
#define STICK_COLOR_WHITE       4
#define STICK_COLOR_ERROR       5


#define CONTENT_COLOR_CUTOFF_1    (float)0.400 //Red max., Yellow min.
#define CONTENT_COLOR_CUTOFF_2    (float)0.900 //Yellow max.
#define CONTENT_COLOR_CUTOFF_3    (float)1.500 //Green min.
#define CONTENT_COLOR_CUTOFF_4    (float)4.000 //Green max.
#define CONTENT_COLOR_CUTOFF_5    (float)0.900 //White min.
#define CONTENT_COLOR_CUTOFF_6    (float)1.500 //White max.

#define T1_LINE         1
#define T2_LINE         2
#define T3_LINE         3

#define SANDWICH_REACTION       1
#define COMPETITIVE_REACTION    2


#define APP_CONNECTION_WATIT_TIME     900 //15 min
#define STICK_INSERT_WATIT_TIME       900 //15 min
#define SAMPLE_LOADING_WATIT_TIME     900 //15 min

#define RESULT_DISPLAY_TIME           300 //5 min 결과 표시
#define ERROR_SITCK_CHECK_TIME        120 //2 min
#define SITCK_EJECTION_TIME           120 //2 min
#define STICK_RE_INSERTION_TIME       120 //2 min
//
//#define DIPPING_ALARM_RATIO        (float)85.0
#define DIPPING_ALARM_RATIO        (float)90.0
#define BAND_NOR_RATIO             (float)95.0//(float)98.0

#define TEST_VALID                 1
#define TEST_INVALID               2

//ERROR
#define ERROR_STICK_COLOR_DETECT      1
#define ERROR_COMP_FACTORY_PTR_DATA   2
#define ERROR_COMP_AGING_PTR_DATA     3
#define ERROR_FORCE_STICK_EJECT       4
#define ERROR_PTR_SENSING_DATA        5


#define SWAP(x, y, t) ( (t) = (x), (x) = (y), (y) = (t) )


enum //User Mode Process List
{
  MAIN_APP_CONNECTION_WAITING = 3,
  MAIN_STICK_INSERT_WAITING,
  MAIN_STICK_CHECK,
  MAIN_ERROR_STICK_CHECK,
  MAIN_SAMPLE_LOAD_WAITING,
  MAIN_SAMPLE_REACT_WAITING,
  MAIN_SAMPLE_ANALYZE,
  MAIN_RESULT_DISPLAY,
  MAIN_STICK_EJECTION,
};

enum //Result level
{
  RESULT_LEVEL_0 = 0,//음성
  RESULT_LEVEL_1, //양성1
  RESULT_LEVEL_2, //양성2
  RESULT_LEVEL_3, //양성3
  RESULT_LEVEL_4, //양성4
  RESULT_LEVEL_5, //양성5
  RESULT_LEVEL_6, //양성6
  RESULT_FAIL = 9,    
};

typedef struct
{
  char flag;
  char function_number;
}USER_MODE_PROCESS_TypeDef;

void User_Mode_Process();
void User_Mode_Variable_Init();
void Next_User_Mode_Process(char process_number);

void App_Connection_Waiting();
void Stick_Insert_Waiting();
void Stick_Check();
void Sample_Loading_Waiting();
void Sample_React_Waiting();
void Sample_Analyze();
void Result_Display();
void Stick_Ejection();
void Error_Stick_Check();
void Error_Stick_Insert_Check();
char Get_Stick_Color();

void Run_Usermode();

#endif /* __USER_MODE_H*/