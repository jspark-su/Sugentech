#ifndef __TEST_REPORT_H
 #define __TEST_REPORT_H

#define TR_PASS     (char)1
#define TR_FAIL     (char)2



typedef struct
{
    int date_year;
    int date_month;
    int date_day;
    int date_hours;
    int date_minutes;
    int date_seconds;
    char manufacturer_name[200];
    char inspector_name[200];
    char dev_name[200];
    char dev_serial_num[100];
    //char dev_uid[25];
    //char fw_ver[20];
    char ble_mac_id[BLE_MAC_BUF_SIZE];
    char ble_name_str[BLE_NAME_BUF_SIZE];
    int led_calib_dac[8];
    int led_calib_ptr[8];
    char overall_test_result;
    int measure_test_line_1[6];
    int measure_level[6];
    int measure_result[6][4];
}TEST_REPORT_VARIABLE_Type_Def;

typedef struct
{
    char led_calibration;
    char ble_name;
}SETTING_RESULT_Type_Def;

int PI_Save_Test_Report(int panel, char *filename);
int FGI_Save_Test_Report(int panel, char *filename);
char Overall_Test_Result_Check(SETTING_RESULT_Type_Def* P_SET_RESULT);
void Set_Test_End_Time();
void Set_User_Dev_Info();

#endif /* __DATA_BANK_H*/
