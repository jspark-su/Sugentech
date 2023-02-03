#ifndef __FGI_MODE_H
 #define __FGI_MODE_H

/* FGI: Finished Goods Inspections */


#define CHECKLIST_NONE          0
#define CHECKLIST_APPROVE       1
#define CHECKLIST_INAPPROVE     2

#define RESULT_MINUS            0
#define RESULT_PLUS_1           1
#define RESULT_PLUS_2           2
#define RESULT_PLUS_3           3

#define STD_STICK_SSM1          0
#define STD_STICK_SSM2          1
#define STD_STICK_SSM3          2
#define STD_STICK_SSM4          3
#define STD_STICK_SSM5          4
#define STD_STICK_SSN1          5
#define STD_STICK_ERR           9

#define FGI_CALIBRATION_MODE_START		1
#define FGI_CALIBRATION_STICK_CHECK		2
#define FGI_CALIBRATION_START 		    3
#define FGI_CALIBRATION_STATE			4
#define FGI_STD_STICK_READ_START		5
#define FGI_STD_STICK_READ_END			6


typedef struct
{
    char one_time_process;
    char process;
	char process_state;
    char function_number;
    char cmd_check;
    char cmd_retry_count;
    char cmd_retry_timer;
}FGI_VALIABLE_Typedef;

typedef struct
{
  char visual_inspection;
  char device_status_check;
  char inspection_stick_check;
  char bluetooth_check;
  char color_intensity_measure;
}INSPECTION_ITEMS_Typedef;

void FGI_Read_Checklist_Item_Status(int panel);
void FGI_Read_Reference_Value_Range(int panel, int* ref_value[6][2]);
void FGI_Read_Reference_Level(int panel, int* ref_level[6]);
void FGI_Read_Reference_Result(int panel, int* ref_rslt[6]);
void FGI_Clear_Items(int panel);
char FGI_Checklist_Items_Inspection(int panel);

void FGI_Read_Std_Stick_Process();
void FGI_Read_Std_Stick_Init(int panel);
void FGI_Calibration_Mode_Start(int panel);
void FGI_Calibration_Stick_Check(int panel);
void FGI_Calibration_Start(int panel);
void FGI_Calibration_State(int panel);
void FGI_Read_Std_Stick_Start(int panel);
void FGI_Read_Std_Stick_End(int panel);
void FGI_Compare_Ref_and_Measure_Value(int panel, int std_stick_num);
void FGI_Display_Measure_Result(int panel, int std_stick_num, int rslt_value);
void FGI_Display_Mesure_LED(int panel, int std_stick_num, int detect_stick_num);

void FGI_Next_Process(char func_num);
void FGI_Stop();

#endif /* __FGI_MODE_H*/