#include "main.h"

TEST_REPORT_VARIABLE_Type_Def TEST_REPORT_VAR = {0,};
SETTING_RESULT_Type_Def SET_RESULT = {0,};


int PI_Save_Test_Report(int panel, char *filename)  
{
    int ret = 0;
	FILE *fp;
	
	if((fp = fopen(filename, "wt")) != NULL)
	{
        fprintf(fp, "1.,테스트 모델,Surearly SMART\n");
        
        fprintf(fp, "2.,설정 완료 날짜,%d-%d-%d/%d:%d:%d\n", TEST_REPORT_VAR.date_year, TEST_REPORT_VAR.date_month, TEST_REPORT_VAR.date_day, TEST_REPORT_VAR.date_hours, TEST_REPORT_VAR.date_minutes, TEST_REPORT_VAR.date_seconds);
        
        fprintf(fp, "3.,생산자 이름,%s\n", TEST_REPORT_VAR.manufacturer_name);
        
        fprintf(fp, "4.,펌웨어 버전,v%s\n", fw_ver);
        
        fprintf(fp, "5.,기기 UID,%s\n", dev_uid);
        
        fprintf(fp, "6.,블루투스 이름,%s\n", ble_name);
        
        fprintf(fp, "7.,블루투스 MAC 주소,%s\n", TEST_REPORT_VAR.ble_mac_id);
        
        if(Overall_Test_Result_Check(&SET_RESULT) == TR_PASS){
            fprintf(fp, "8.,설정 결과,성공\n");            
        }
        else{
            fprintf(fp, "8.,설정 결과,실패\n");        
        }

        fprintf(fp, "9.,설정 항목\n");
        
        fprintf(fp, ",Num,항목,결과\n");
        
        if(SET_RESULT.led_calibration == TR_PASS){
            fprintf(fp, ",1),LED calibration,성공\n");    
        }
        else{
            fprintf(fp, ",1),LED calibration,실패\n");    
        }        
        
        fprintf(fp, "10.,설정 완료 데이터\n");
        fprintf(fp, ",1),LED calibration,Ch1,Ch2,Ch3,Ch4,Ch5,Ch6,Ch7\n");
        
        fprintf(fp, ",,DAC,%d,%d,%d,%d,%d,%d,%d\n", 
                TEST_REPORT_VAR.led_calib_dac[1], TEST_REPORT_VAR.led_calib_dac[2], TEST_REPORT_VAR.led_calib_dac[3], TEST_REPORT_VAR.led_calib_dac[4], TEST_REPORT_VAR.led_calib_dac[5], TEST_REPORT_VAR.led_calib_dac[6], TEST_REPORT_VAR.led_calib_dac[7]);
        
        
        float tmp_led_calib_ptr[8] = {0};
        for(char i  = 1; i < 8; i++){
            tmp_led_calib_ptr[i] = (float) (TEST_REPORT_VAR.led_calib_ptr[i] / 10.0);
        }
        fprintf(fp, ",,PTR,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\n", 
                tmp_led_calib_ptr[1], tmp_led_calib_ptr[2], tmp_led_calib_ptr[3], tmp_led_calib_ptr[4], tmp_led_calib_ptr[5], tmp_led_calib_ptr[6], tmp_led_calib_ptr[7]);       
        
		fclose(fp);
		ret = 1;
	}
	else{
		ret = 0;	
	}
	return ret;   
}

int FGI_Save_Test_Report(int panel, char *filename)
{
    int ret = 0;
	FILE *fp;
	
	if((fp = fopen(filename, "wt")) != NULL)
	{
        Set_Test_End_Time();
        Set_User_Dev_Info();
        FGI_Read_Checklist_Item_Status(tabPanel_FGI);
        
        fprintf(fp, "1.,테스트 모델,Surearly SMART\n");
        
        fprintf(fp, "2.,테스트 완료 날짜,%d-%d-%d/%d:%d:%d\n", TEST_REPORT_VAR.date_year, TEST_REPORT_VAR.date_month, TEST_REPORT_VAR.date_day, TEST_REPORT_VAR.date_hours, TEST_REPORT_VAR.date_minutes, TEST_REPORT_VAR.date_seconds);
        
        fprintf(fp, "3.,검사자 이름,%s\n", TEST_REPORT_VAR.inspector_name);
        
        fprintf(fp, "4.,기기 시리얼 번호,%s\n", TEST_REPORT_VAR.dev_serial_num);
        
        fprintf(fp, "5.,펌웨어 버전,v%s\n", fw_ver);
        
        fprintf(fp, "6.,MCU UID,%s\n", dev_uid);
        
        fprintf(fp, "7.,블루투스 이름,%s\n", ble_name);
        
        fprintf(fp, "8.,블루투스 MAC 주소,%s\n", TEST_REPORT_VAR.ble_mac_id);
        
        fprintf(fp, "9.,테스트 결과\n");
        
        
        if(INSPECTION_ITEMS.visual_inspection == CHECKLIST_APPROVE){
            fprintf(fp, ",1),기기 외관 검사,적합\n");
        }
        else if(INSPECTION_ITEMS.visual_inspection == CHECKLIST_INAPPROVE){
            fprintf(fp, ",1),기기 외관 검사,부적합\n");
        }
        else{
            fprintf(fp, ",1),기기 외관 검사,선택 없음\n");
        }
        
        if(INSPECTION_ITEMS.device_status_check == CHECKLIST_APPROVE){
            fprintf(fp, ",2),기기 상태 표시 확인,적합\n");
        }
        else if(INSPECTION_ITEMS.device_status_check == CHECKLIST_INAPPROVE){
            fprintf(fp, ",2),기기 상태 표시 확인,부적합\n");
        }
        else{
            fprintf(fp, ",2),기기 상태 표시 확인,선택 없음\n");
        }
        
        if(INSPECTION_ITEMS.inspection_stick_check == CHECKLIST_APPROVE){
            fprintf(fp, ",3),검사 스틱 삽입 확인,적합\n");
        }
        else if(INSPECTION_ITEMS.inspection_stick_check == CHECKLIST_INAPPROVE){
            fprintf(fp, ",3),검사 스틱 삽입 확인,부적합\n");
        }
        else{
            fprintf(fp, ",3),검사 스틱 삽입 확인,선택 없음\n");
        }
        
        if(INSPECTION_ITEMS.bluetooth_check == CHECKLIST_APPROVE){
            fprintf(fp, ",4),블루투스 기능 확인,적합\n");
        }
        else if(INSPECTION_ITEMS.bluetooth_check == CHECKLIST_INAPPROVE){
            fprintf(fp, ",4),블루투스 기능 확인,부적합\n");
        }
        else{
            fprintf(fp, ",4),블루투스 기능 확인,선택 없음\n");    
        }

        if(INSPECTION_ITEMS.color_intensity_measure == CHECKLIST_APPROVE){
            fprintf(fp, ",5),발색 세기 측정,적합\n");
        }
        else if(INSPECTION_ITEMS.color_intensity_measure == CHECKLIST_INAPPROVE){
            fprintf(fp, ",5),발색 세기 측정,부적합\n");
        }
        else{
            fprintf(fp, ",5),발색 세기 측정,선택 없음\n");
        }
        
        
        fprintf(fp, "9.,결과 데이터\n");
        fprintf(fp, ",번호,Rawdata,양음성,Level\n");
        fprintf(fp, ",SSM1,%d,%s,%d\n", TEST_REPORT_VAR.measure_test_line_1[0], &TEST_REPORT_VAR.measure_result[0][0], TEST_REPORT_VAR.measure_level[0]);
        fprintf(fp, ",SSM2,%d,%s,%d\n", TEST_REPORT_VAR.measure_test_line_1[1], &TEST_REPORT_VAR.measure_result[1][0], TEST_REPORT_VAR.measure_level[1]);
        fprintf(fp, ",SSM3,%d,%s,%d\n", TEST_REPORT_VAR.measure_test_line_1[2], &TEST_REPORT_VAR.measure_result[2][0], TEST_REPORT_VAR.measure_level[2]);
        fprintf(fp, ",SSM4,%d,%s,%d\n", TEST_REPORT_VAR.measure_test_line_1[3], &TEST_REPORT_VAR.measure_result[3][0], TEST_REPORT_VAR.measure_level[3]);
        fprintf(fp, ",SSM5,%d,%s,%d\n", TEST_REPORT_VAR.measure_test_line_1[4], &TEST_REPORT_VAR.measure_result[4][0], TEST_REPORT_VAR.measure_level[4]);
        fprintf(fp, ",SSN1,%d,%s,%d\n", TEST_REPORT_VAR.measure_test_line_1[5], &TEST_REPORT_VAR.measure_result[5][0], TEST_REPORT_VAR.measure_level[5]);
        
        fclose(fp);
		ret = 1;
	}
	else{
		ret = 0;	
	}
	return ret;   
}

char Overall_Test_Result_Check(SETTING_RESULT_Type_Def* P_SET_RESULT)
{
    int i = 0;
    int struct_size = sizeof(SETTING_RESULT_Type_Def);
    char result_chk = 0;
        
    char* m_set_result = (char *)malloc(sizeof(char) * struct_size);

    memcpy(m_set_result, P_SET_RESULT, struct_size);
    
    for(i = 0; i < struct_size; i++)
    {
         if(m_set_result[i] == TR_PASS){}
         else{
            result_chk = 1;    
         }
    }
    
    free(m_set_result);
    
    if(result_chk){
        return TR_FAIL;    
    }
    else{
        return TR_PASS;    
    }
}


void Set_Test_End_Time()
{
    if(GetSystemDate (&TEST_REPORT_VAR.date_month, &TEST_REPORT_VAR.date_day, &TEST_REPORT_VAR.date_year))
    {
        TEST_REPORT_VAR.date_day = 1;
        TEST_REPORT_VAR.date_month = 1;
        TEST_REPORT_VAR.date_year = 2020;
    }
    
    if(GetSystemTime(&TEST_REPORT_VAR.date_hours, &TEST_REPORT_VAR.date_minutes, &TEST_REPORT_VAR.date_seconds))
    {
        TEST_REPORT_VAR.date_hours = 12;
        TEST_REPORT_VAR.date_minutes = 0;
        TEST_REPORT_VAR.date_seconds = 0;
    }        
}

void Set_User_Dev_Info()
{
    GetCtrlVal (tabPanel_PI, TABPANEL_INPUT_MANUF_NAME, TEST_REPORT_VAR.manufacturer_name);
    GetCtrlVal (tabPanel_FGI, TABPANEL_2_INPUT_INSPECTOR_NAME, TEST_REPORT_VAR.inspector_name);
    GetCtrlVal (subPanel, SUB_PANEL_sINPUT_BLE_MAC_ADDR, TEST_REPORT_VAR.ble_mac_id);
    GetCtrlVal (tabPanel_FGI, TABPANEL_2_INPUT_DEV_SERIAL_NUM, TEST_REPORT_VAR.dev_serial_num);
}
