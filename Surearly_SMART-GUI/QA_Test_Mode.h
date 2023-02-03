#ifndef __QA_TEST_MODE_H
 #define __QA_TEST_MODE_H


enum
{
 QA_STICK_SENSING_MODE_START = 1,
 QA_STICK_SENSING_START,
 QA_STICK_SENSING_RESULT,
};

enum
{
 QA_CALIBRATION_MODE_START = 1,
 QA_CALIBRATION_STICK_CHECK,
 QA_CALIBRATION_START,
 QA_CALIBRATION_STATE,
 QA_CALIBRATION_RESULT,
};

void QA_Stick_Sensing_Mode_Process();
void QA_Stick_Sensing_Mode_Start(int panel);
void QA_Stick_Sensing_Start(int panel);
void QA_Stick_Sensing_Result(int panel);

void QA_Stick_Sensing_Mode_Stop();
void QA_Stick_Sensing_Mode_Next_Process(char func_num);
void QA_Stick_Sensing_Result_Display(int panel);

void QA_Calibration_Process();
void QA_Calibration_Mode_Start(int panel);
void QA_Calibration_Stick_Check(int panel);
void QA_Calibration_Start(int panel);
void QA_Calibration_State(int panel);
void QA_Calibration_Result(int panel);

void QA_Calibration_Result_Display(int panel);
void QA_Calib_Stop();
void QA_Calib_Next_Process(char func_num);
#endif /* __QA_TEST_MODE_H*/
