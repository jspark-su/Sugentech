#ifndef __QA_SETTING_MODE_H
 #define __QA_SETTING_MODE_H

#define QA_RESULT_PASS  1
#define QA_RESULT_FAIL  2


enum
{
 QA_DATA_SETTING_START = 1,
 QA_STICK_INSERT_CHECK,
 QA_READ_STD_STICK_START,
 QA_READ_STD_STICK_RESULT,
 QA_DATA_SETTING_END,
};
/*
void QA_Read_Standard_Stick_Process();
void QA_Data_Setting_Start(int panel);
void QA_Stick_Insert_Check(int panel);
void QA_Read_Standard_Stick_Start(int panel);
void QA_Read_Standard_Stick_Result(int panel);
void QA_Data_Setting_End(int panel);

void QA_Stop();
void QA_Next_Process(char func_num);
void QA_Stick_Sensing_Rawdata_Display(int panel);
void QA_Set_Standard_Stick_Data(int panel);
*/

#endif /* __QA_SETTING_MODE_H*/
