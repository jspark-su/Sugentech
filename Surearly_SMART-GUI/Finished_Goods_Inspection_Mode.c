#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define COMPLETED	0
#define FAIL		1

/* Private variables ---------------------------------------------------------*/
FGI_VALIABLE_Typedef    FGI_VALIABLE;
INSPECTION_ITEMS_Typedef    INSPECTION_ITEMS;

char std_stick_number = 0;
char FGI_CAL_Result = 0;

/* Private function prototypes -----------------------------------------------*/
void FGI_CAL_Stop();
void SetResultLine(char count);

// xx
char buf_debug[100] = {0};
// xx

/* ============================================================================
			##### Finished Goods Inspection Mode functions #####
 =============================================================================*/
void FGI_Read_Checklist_Item_Status(int panel)
{
    char checklist_item[6][3] = {0,};
                             
    GetCtrlVal (panel, TABPANEL_2_INS_APPRO_CHKBOX1, &checklist_item[0][0]);
	GetCtrlVal (panel, TABPANEL_2_INS_INAPPRO_CHKBOX1, &checklist_item[0][1]);
    if(checklist_item[0][0]){
        INSPECTION_ITEMS.visual_inspection = CHECKLIST_APPROVE;
    }
    else if (checklist_item[0][1]){
        INSPECTION_ITEMS.visual_inspection = CHECKLIST_INAPPROVE;
    }
    else{
        INSPECTION_ITEMS.visual_inspection = CHECKLIST_NONE;
    }
    
	GetCtrlVal (panel, TABPANEL_2_INS_APPRO_CHKBOX2, &checklist_item[1][0]);
	GetCtrlVal (panel, TABPANEL_2_INS_INAPPRO_CHKBOX2, &checklist_item[1][1]);
    if(checklist_item[1][0]){
        INSPECTION_ITEMS.device_status_check = CHECKLIST_APPROVE;
    }
    else if (checklist_item[1][1]){
        INSPECTION_ITEMS.device_status_check = CHECKLIST_INAPPROVE;
    }
    else{
        INSPECTION_ITEMS.device_status_check = CHECKLIST_NONE;
    }
    
	GetCtrlVal (panel, TABPANEL_2_INS_APPRO_CHKBOX3, &checklist_item[2][0]);
    GetCtrlVal (panel, TABPANEL_2_INS_INAPPRO_CHKBOX3, &checklist_item[2][1]);
    if(checklist_item[2][0]){
        INSPECTION_ITEMS.inspection_stick_check = CHECKLIST_APPROVE;
    }
    else if (checklist_item[2][1]){
        INSPECTION_ITEMS.inspection_stick_check = CHECKLIST_INAPPROVE;
    }
    else{
        INSPECTION_ITEMS.inspection_stick_check = CHECKLIST_NONE;
    }
    
    GetCtrlVal (panel, TABPANEL_2_INS_APPRO_CHKBOX4, &checklist_item[3][0]);
    GetCtrlVal (panel, TABPANEL_2_INS_INAPPRO_CHKBOX4, &checklist_item[3][1]);
    if(checklist_item[3][0]){
        INSPECTION_ITEMS.bluetooth_check = CHECKLIST_APPROVE;
    }
    else if (checklist_item[3][1]){
        INSPECTION_ITEMS.bluetooth_check = CHECKLIST_INAPPROVE;
    }
    else{
        INSPECTION_ITEMS.bluetooth_check = CHECKLIST_NONE;
    }
    
    GetCtrlVal (panel, TABPANEL_2_INS_APPRO_CHKBOX5, &checklist_item[4][0]);
    GetCtrlVal (panel, TABPANEL_2_INS_INAPPRO_CHKBOX5, &checklist_item[4][1]);
    if(checklist_item[4][0]){
        INSPECTION_ITEMS.color_intensity_measure = CHECKLIST_APPROVE;
    }
    else if (checklist_item[4][1]){
        INSPECTION_ITEMS.color_intensity_measure = CHECKLIST_INAPPROVE;
    }
    else{
        INSPECTION_ITEMS.color_intensity_measure = CHECKLIST_NONE;
    }
}


void FGI_Read_Reference_Value_Range(int panel, int value[6][2])
{
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM1_RANG_ABOVE, &value[0][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM1_RANG_BELOW, &value[0][1]);
    
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM2_RANG_ABOVE, &value[1][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM2_RANG_BELOW, &value[1][1]);
    
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM3_RANG_ABOVE, &value[2][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM3_RANG_BELOW, &value[2][1]);
    
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM4_RANG_ABOVE, &value[3][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM4_RANG_BELOW, &value[3][1]);
    
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM5_RANG_ABOVE, &value[4][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSM5_RANG_BELOW, &value[4][1]);
    
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSN1_RANG_ABOVE, &value[5][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_SSN1_RANG_BELOW, &value[5][1]);    
}

void FGI_Read_Reference_Level(int panel, int ref_level[6])
{
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM1_LEVEL, &ref_level[0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM2_LEVEL, &ref_level[1]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM3_LEVEL, &ref_level[2]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM4_LEVEL, &ref_level[3]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM5_LEVEL, &ref_level[4]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSN1_LEVEL, &ref_level[5]);   
}

void FGI_Read_Reference_Result(int panel, int ref_rslt[6])
{
    char ref_rslt_str[6][4] = {0,};
    char i = 0;
                          
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM1_RSLT, &ref_rslt_str[0][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM2_RSLT, &ref_rslt_str[1][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM3_RSLT, &ref_rslt_str[2][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM4_RSLT, &ref_rslt_str[3][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSM5_RSLT, &ref_rslt_str[4][0]);
    GetCtrlVal (panel, TABPANEL_2_INPUT_REF_SSN1_RSLT, &ref_rslt_str[5][0]);
    
    for(i = 0; i < 6; i++)
    {
        if(!strcmp(&ref_rslt_str[i][0],"+")){
            *ref_rslt = RESULT_PLUS_1;
        }
        else if(!strcmp(&ref_rslt_str[i][0],"++")){
            *ref_rslt = RESULT_PLUS_2;
        }
        else if(!strcmp(&ref_rslt_str[i][0],"+++")){
            *ref_rslt = RESULT_PLUS_3;
        }
        else if(!strcmp(&ref_rslt_str[i][0],"-")){
            *ref_rslt = RESULT_MINUS;
        }
        else{
            *ref_rslt = RESULT_MINUS;
        }
        ref_rslt++;
    }
}

void FGI_Clear_Checkbox(int panel)
{
    int checkbox_appro_index[] = {TABPANEL_2_INS_APPRO_CHKBOX1, TABPANEL_2_INS_APPRO_CHKBOX2, TABPANEL_2_INS_APPRO_CHKBOX3, TABPANEL_2_INS_APPRO_CHKBOX4, TABPANEL_2_INS_APPRO_CHKBOX5};
    int checkbox_inappro_index[] = {TABPANEL_2_INS_INAPPRO_CHKBOX1, TABPANEL_2_INS_INAPPRO_CHKBOX2, TABPANEL_2_INS_INAPPRO_CHKBOX3, TABPANEL_2_INS_INAPPRO_CHKBOX4, TABPANEL_2_INS_INAPPRO_CHKBOX5};
	
    int idx = 0;
    for(idx = 0; idx < 5; idx++)
    {
        if(idx < 4) //검사 항목 1 ~ 4는 항목 초기화 진행 시 부적합은 체크 해제, 적합은 체크 활성화 함
        {
            SetCtrlVal (panel, checkbox_appro_index[idx], CHECKED);
            SetCtrlVal (panel, checkbox_inappro_index[idx], UNCHECKED);
        }
        else //검사 항목 5는 적합, 부적합 모두 체크 해제함 
        {
            SetCtrlVal (panel, checkbox_appro_index[idx], UNCHECKED);
            SetCtrlVal (panel, checkbox_inappro_index[idx], UNCHECKED);
        }
    }
}


void FGI_Clear_Result(int panel)
{	
	// 측정값 Reset
    int rawdata_content_index[] = {TABPANEL_2_INPUT_MEA_SSM1_RAW, TABPANEL_2_INPUT_MEA_SSM2_RAW, TABPANEL_2_INPUT_MEA_SSM3_RAW, 
                                   TABPANEL_2_INPUT_MEA_SSM4_RAW, TABPANEL_2_INPUT_MEA_SSM5_RAW, TABPANEL_2_INPUT_MEA_SSN1_RAW};
    int result_content_index[] = {TABPANEL_2_INPUT_MEA_SSM1_RSLT, TABPANEL_2_INPUT_MEA_SSM2_RSLT, TABPANEL_2_INPUT_MEA_SSM3_RSLT,
                                 TABPANEL_2_INPUT_MEA_SSM4_RSLT, TABPANEL_2_INPUT_MEA_SSM5_RSLT, TABPANEL_2_INPUT_MEA_SSN1_RSLT};
    int level_content_index[] = {TABPANEL_2_INPUT_MEA_SSM1_LEVEL, TABPANEL_2_INPUT_MEA_SSM2_LEVEL, TABPANEL_2_INPUT_MEA_SSM3_LEVEL,
                                TABPANEL_2_INPUT_MEA_SSM4_LEVEL, TABPANEL_2_INPUT_MEA_SSM5_LEVEL, TABPANEL_2_INPUT_MEA_SSN1_LEVEL};
    int result_led_index[] = {TABPANEL_2_SSM1_RSLT_LED, TABPANEL_2_SSM2_RSLT_LED, TABPANEL_2_SSM3_RSLT_LED,
                             TABPANEL_2_SSM4_RSLT_LED, TABPANEL_2_SSM5_RSLT_LED, TABPANEL_2_SSN1_RSLT_LED};
    
    int idx = 0;
	
    for(idx = 0; idx < 6; idx++)
	{
        SetCtrlVal (panel, rawdata_content_index[idx], 0);
        SetCtrlVal (panel, result_content_index[idx], 0);
        SetCtrlVal (panel, level_content_index[idx], 0);
        SetCtrlVal (panel, result_led_index[idx], 0);
    }
	
	// 적합/부적합 Reset
	SetCtrlVal (panel, TABPANEL_2_INS_APPRO_CHKBOX5, UNCHECKED);
	SetCtrlVal (panel, TABPANEL_2_INS_INAPPRO_CHKBOX5, UNCHECKED);
	
	// 캘리브레이션 결과 LED Off
	SetCtrlVal (panel, TABPANEL_2_CAL_RSLT_LED, LED_OFF);
}

char FGI_Checklist_Items_Inspection(int panel)
{
    char error = 0;
    FGI_Read_Checklist_Item_Status(panel);
    
    if(INSPECTION_ITEMS.visual_inspection == CHECKLIST_APPROVE){}
    else if(INSPECTION_ITEMS.visual_inspection == CHECKLIST_INAPPROVE){}
    else{
        error = 1;
    }
    
    if(INSPECTION_ITEMS.device_status_check == CHECKLIST_APPROVE){}
    else if(INSPECTION_ITEMS.device_status_check == CHECKLIST_INAPPROVE){}
    else{
        error = 1;
    }
    
    if(INSPECTION_ITEMS.inspection_stick_check == CHECKLIST_APPROVE){}
    else if(INSPECTION_ITEMS.inspection_stick_check == CHECKLIST_INAPPROVE){}
    else{
        error = 1;
    }
    
    if(INSPECTION_ITEMS.bluetooth_check == CHECKLIST_APPROVE){}
    else if(INSPECTION_ITEMS.bluetooth_check == CHECKLIST_INAPPROVE){}
    else{
        error = 1;
    }

    if(INSPECTION_ITEMS.color_intensity_measure == CHECKLIST_APPROVE){}
    else if(INSPECTION_ITEMS.color_intensity_measure == CHECKLIST_INAPPROVE){}
    else{
        error = 1;
    }
    
    return error;
}


void FGI_Read_Std_Stick_Process()
{
	if(FGI_VALIABLE.process == START)
	{
	    switch(FGI_VALIABLE.function_number)
	    {
          case FGI_CALIBRATION_MODE_START :
        	FGI_Calibration_Mode_Start(tabPanel_FGI);
	        break;
          case FGI_CALIBRATION_STICK_CHECK :
        	FGI_Calibration_Stick_Check(tabPanel_FGI);
	        break;
          case FGI_CALIBRATION_START :
        	FGI_Calibration_Start(tabPanel_FGI);
	        break;
          case FGI_CALIBRATION_STATE :
        	FGI_Calibration_State(tabPanel_FGI);
	        break;				
          case FGI_STD_STICK_READ_START :
        	FGI_Read_Std_Stick_Start(tabPanel_FGI);
	        break;  
		  case FGI_STD_STICK_READ_END :
        	FGI_Read_Std_Stick_End(tabPanel_FGI);
	        break;
        }
    }
}


void FGI_Read_Std_Stick_Init(int panel)
{
    if(FGI_VALIABLE.one_time_process == RESET)
	{
		FGI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_START))
	{
        FGI_Next_Process(FGI_STD_STICK_READ_START);
	}
	else
	{
		FGI_VALIABLE.cmd_retry_timer++;
		if(FGI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			FGI_VALIABLE.cmd_retry_timer = 0;
			if(FGI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
				FGI_VALIABLE.cmd_retry_count++;
			}
			else
			{
				FGI_Stop();	
			}
		}
	}
}


void FGI_Calibration_Mode_Start(int panel)
{
	if(FGI_VALIABLE.one_time_process == RESET)
	{
		// 검사항목 5번, 6번 초기화
		FGI_Clear_Result(panel);
		
		FGI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_START))
	{
		SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 캘리브레이션 모드 시작");
		FGI_Next_Process(FGI_CALIBRATION_STICK_CHECK);
	}
	else
	{
		FGI_VALIABLE.cmd_retry_timer++;
		if(FGI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			FGI_VALIABLE.cmd_retry_timer = 0;
			if(FGI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
				FGI_VALIABLE.cmd_retry_count++;
			}
			else 
			{
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, "\r\n 오류: 기기 전원, 블루투스 상태를 확인하십시오.\r\n");
				FGI_CAL_Stop();
				FGI_Stop();
			}
		}
	}		
}

void FGI_Calibration_Stick_Check(int panel)
{
	if(FGI_VALIABLE.one_time_process == RESET)
	{
		SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " & 스틱 삽입 확인 ");
		
		FGI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_STICK_CHECK);
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_STICK_CHECK))
	{
		if((stick_state_check == STICK_INSERTED) && (calibration_proc_state == STATE_NONE))
		{
			SetResultLine(FGI_VALIABLE.cmd_retry_count);
			SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [OK]\r\n");
			FGI_Next_Process(FGI_CALIBRATION_START);
		}
		else //STICK_EJECTED
		{
			SetResultLine(FGI_VALIABLE.cmd_retry_count);
			SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 캘리브레이션 스틱 분리\r\n");
			SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 캘리브레이션 스틱 삽입 후 설정을 재시작하십시오.\r\n");
			FGI_CAL_Stop();
			FGI_Stop();
		}
	}
	else
	{
		FGI_VALIABLE.cmd_retry_timer++;
		if(FGI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			FGI_VALIABLE.cmd_retry_timer = 0;
			if(FGI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_STICK_CHECK);
				FGI_VALIABLE.cmd_retry_count++;
				
				if(FGI_VALIABLE.cmd_retry_count % 2 == 0) SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, "-");
			}
			else 
			{
				SetResultLine(FGI_VALIABLE.cmd_retry_count);
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 블루투스 연결을 확인해주십시오.\r\n");
        		FGI_CAL_Stop();
				FGI_Stop();
			}
		}
	}	
}


void FGI_Calibration_Start(int panel)
{
	if(FGI_VALIABLE.one_time_process == RESET)
	{
		FGI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_START);
		
		SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " > 캘리브레이션 진행 ");
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && ((rx_msg_function == CMD_CALIBRATION_START) || (rx_msg_function == CMD_CALIBRATION_STATE_CHECK)))
	{
		SetResultLine(FGI_VALIABLE.cmd_retry_count);
		SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [OK]\r\n");
		FGI_Next_Process(FGI_CALIBRATION_STATE);
	}
	else
	{
		FGI_VALIABLE.cmd_retry_timer++;
		if(FGI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			FGI_VALIABLE.cmd_retry_timer = 0;
			if(FGI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_START);
				FGI_VALIABLE.cmd_retry_count++;
				
				if(FGI_VALIABLE.cmd_retry_count % 2 == 0) SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, "-");
			}
			else 
			{
				SetResultLine(FGI_VALIABLE.cmd_retry_count);
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 블루투스 연결을 확인해주십시오.\r\n");
				FGI_CAL_Stop();
        		FGI_Stop();
			}
		}
	}	
}

void FGI_Calibration_State(int panel)
{
	if(FGI_VALIABLE.one_time_process == RESET)
	{
		FGI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
		
		SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " > 캘리브레이션 설정 ");
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_STATE_CHECK))
	{
		if(stick_state_check == STICK_INSERTED)
		{
			if(calibration_proc_state == STATE_COMPLETE)
			{
				SetCtrlAttribute (panel, TABPANEL_2_CAL_RSLT_LED, ATTR_ON_COLOR, VAL_GREEN);
				SetCtrlVal (panel, TABPANEL_2_CAL_RSLT_LED, LED_ON);
				SetCtrlVal(panel, TABPANEL_2_BT_SSN1_CAL, BT_OFF);
				
				SetResultLine(FGI_VALIABLE.cmd_retry_count);
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [Completed]\r\n");
	            //SET_RESULT.led_calibration = TR_PASS;
				
				FGI_CAL_Result = COMPLETED;
				
				int CheckState;
				GetCtrlVal (panel, TABPANEL_2_CB_SSN1_READ, &CheckState);
				if(CheckState == CHECKED)
				{
					SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > SSN1 표준 스틱 측정 중..\r\n");
					FGI_Next_Process(FGI_STD_STICK_READ_START);
				}
				else // UNCHECKED
				{
					FGI_Stop();
				}
			}
			else if(calibration_proc_state == STATE_FAIL)
			{
				SetResultLine(FGI_VALIABLE.cmd_retry_count);
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 기기의 광학부 상태를 점검해주십시오.\n");
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 기기 상태 확인 후 설정을 재시작하십시오.\n");
		        //SET_RESULT.led_calibration = TR_FAIL;
				FGI_CAL_Stop();
				FGI_Stop();
			}
			else {}
		}
		else
		{
			SetResultLine(FGI_VALIABLE.cmd_retry_count);
			SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [Stick Eject]\r\n");
			SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 캘리브레이션 스틱이 삽입되지 않았습니다.\r\n");
			FGI_CAL_Stop();
    		FGI_Stop();
		}
	}
	else
	{
		FGI_VALIABLE.cmd_retry_timer++;
		if(FGI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{	
			FGI_VALIABLE.cmd_retry_timer = 0;		
			if(FGI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
				FGI_VALIABLE.cmd_retry_count++;

				if(FGI_VALIABLE.cmd_retry_count % 2 == 0) SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, "-");
			}
			else 
			{
				SetResultLine(FGI_VALIABLE.cmd_retry_count);
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 오류: 기기의 광학부 상태를 점검해주십시오.\r\n");
				SetCtrlVal(panel, TABPANEL_2_LOG_MONITOR_FGI, " - 기기 상태 확인 후 설정을 재시작하십시오.\r\n");
				//SET_RESULT.led_calibration = TR_FAIL;
				FGI_CAL_Stop();
        		FGI_Stop();
			}
		}
	}	
}

void FGI_Read_Std_Stick_Start(int panel)
{
    if(FGI_VALIABLE.one_time_process == RESET)
	{
		FGI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_QA_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_QA_START))
	{
		FGI_Next_Process(FGI_STD_STICK_READ_END);
	}
	else
	{
		FGI_VALIABLE.cmd_retry_timer++;
		if(FGI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			FGI_VALIABLE.cmd_retry_timer = 0;
			if(FGI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_QA_START);
				FGI_VALIABLE.cmd_retry_count++;
			}
			else 
			{
				//Read std stick start fail
				FGI_Stop();	
			}
		}
	}
}

void FGI_Read_Std_Stick_End(int panel)
{
	if(FGI_VALIABLE.one_time_process == RESET)
	{
		FGI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_QA_RESULT);
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_QA_RESULT))
	{
		if(qa_proc_state == STATE_COMPLETE)
		{
            SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 표준 스틱 측정 완료.\r\n");
            FGI_Compare_Ref_and_Measure_Value(tabPanel_FGI, std_stick_number);
			FGI_Stop();
		}
		else //STATE_DOING
        {
            rx_msg_check = 0;
            API_Tx_Command_Process(CMD_QA_RESULT);
        }
	}
	else
	{
		FGI_VALIABLE.cmd_retry_timer++;
		if(FGI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			FGI_VALIABLE.cmd_retry_timer = 0;
			if(FGI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_QA_RESULT);
				FGI_VALIABLE.cmd_retry_count++;
			}
			else 
			{
				//Retry fail
                FGI_Stop();
			}
		}
	}	
}

void FGI_Compare_Ref_and_Measure_Value(int panel, int std_stick_num)
{
    int ref_value[6][2] = {0,};
    int ref_rslt[6] = {0};
    int ref_level[6] = {0};
    int index = 0;
    
    float tmp_val = (DATA_BANK.STD_STICK_RATIO_DATA[2] / DATA_BANK.STD_STICK_RATIO_DATA[1]) * 1000.0; //T1 line
    int test_line_1 = (int)(tmp_val + 0.5);
    
    TEST_REPORT_VAR.measure_test_line_1[std_stick_num] = test_line_1;
    
    FGI_Read_Reference_Value_Range(panel, ref_value);
    
    if((ref_value[0][0] <= test_line_1) && (test_line_1 < ref_value[0][1])){
        index = STD_STICK_SSM1;
    }
    else if((ref_value[1][0] <= test_line_1) && (test_line_1 < ref_value[1][1])){
        index = STD_STICK_SSM2;
    }
    else if((ref_value[2][0] <= test_line_1) && (test_line_1 < ref_value[2][1])){
        index = STD_STICK_SSM3;
    }
    else if((ref_value[3][0] <= test_line_1) && (test_line_1 < ref_value[3][1])){
        index = STD_STICK_SSM4;
    }
    else if((ref_value[4][0] <= test_line_1) && (test_line_1 < ref_value[4][1])){
        index = STD_STICK_SSM5;
    }
    else if((ref_value[5][0] <= test_line_1) && (test_line_1 < ref_value[5][1])){
        index = STD_STICK_SSN1;
    }
    else {
        index = STD_STICK_ERR;
    }
    
    
    if(index != STD_STICK_ERR)
    {
        FGI_Read_Reference_Result(panel, ref_rslt);
        FGI_Read_Reference_Level(panel, ref_level);
        
        if(std_stick_num == STD_STICK_SSM1){
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM1_RAW, test_line_1);
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM1_LEVEL, ref_level[index]);
        }
        else if(std_stick_num == STD_STICK_SSM2){
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM2_RAW, test_line_1);
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM2_LEVEL, ref_level[index]);
        }
        else if(std_stick_num == STD_STICK_SSM3){
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM3_RAW, test_line_1);
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM3_LEVEL, ref_level[index]);
        }
        else if(std_stick_num == STD_STICK_SSM4){
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM4_RAW, test_line_1);
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM4_LEVEL, ref_level[index]);
        }
        else if(std_stick_num == STD_STICK_SSM5){
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM5_RAW, test_line_1);
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSM5_LEVEL, ref_level[index]);
        }
        else if(std_stick_num == STD_STICK_SSN1){
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSN1_RAW, test_line_1);
            SetCtrlVal (panel, TABPANEL_2_INPUT_MEA_SSN1_LEVEL, ref_level[index]);
        }
        
        FGI_Display_Measure_Result(panel, std_stick_num, ref_rslt[index]);
        FGI_Display_Mesure_LED(panel, std_stick_num, index);
        
        TEST_REPORT_VAR.measure_level[std_stick_num] = ref_level[index];
    }
    else
    {
        //Reference value range error
        SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 설정된 표준 스틱 범위를 벗어남.\r\n");
        SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, " > 삽입된 스틱 확인 및 표준 스틱 범위 확인 필요.\r\n");
    }
}

void FGI_Display_Measure_Result(int panel, int std_stick_num, int rslt_value)
{
    int content_index[6] = {TABPANEL_2_INPUT_MEA_SSM1_RSLT, TABPANEL_2_INPUT_MEA_SSM2_RSLT, TABPANEL_2_INPUT_MEA_SSM3_RSLT, TABPANEL_2_INPUT_MEA_SSM4_RSLT, TABPANEL_2_INPUT_MEA_SSM5_RSLT, TABPANEL_2_INPUT_MEA_SSN1_RSLT};
    
    if(rslt_value == RESULT_MINUS)
    {
        SetCtrlVal (panel, content_index[std_stick_num], "-");
        memcpy(&TEST_REPORT_VAR.measure_result[std_stick_num][0], "-", 2);
    }
    else if(rslt_value == RESULT_PLUS_1)
    {
        SetCtrlVal (panel, content_index[std_stick_num], "+");
        memcpy(&TEST_REPORT_VAR.measure_result[std_stick_num][0], "+", 2);
    }
    else if(rslt_value == RESULT_PLUS_2)
    {
        SetCtrlVal (panel, content_index[std_stick_num], "++");
        memcpy(&TEST_REPORT_VAR.measure_result[std_stick_num][0], "++", 3);
    }
    else if(rslt_value == RESULT_PLUS_3)
    {
        SetCtrlVal (panel, content_index[std_stick_num], "+++");
        memcpy(&TEST_REPORT_VAR.measure_result[std_stick_num][0], "+++", 4);
    }
}

void FGI_Display_Mesure_LED(int panel, int std_stick_num, int detect_stick_num)
{
    int content_index[6] = {TABPANEL_2_SSM1_RSLT_LED, TABPANEL_2_SSM2_RSLT_LED, TABPANEL_2_SSM3_RSLT_LED, TABPANEL_2_SSM4_RSLT_LED, TABPANEL_2_SSM5_RSLT_LED, TABPANEL_2_SSN1_RSLT_LED};
    
    if(std_stick_num == detect_stick_num){
        SetCtrlAttribute (panel, content_index[std_stick_num], ATTR_ON_COLOR, VAL_GREEN);
    }
    else{
        SetCtrlAttribute (panel, content_index[std_stick_num], ATTR_ON_COLOR, VAL_RED);
        SetCtrlVal (panel, TABPANEL_2_INS_APPRO_CHKBOX5, 0);
        SetCtrlVal (panel, TABPANEL_2_INS_INAPPRO_CHKBOX5, 1);
    }
    SetCtrlVal (panel, content_index[std_stick_num], 1);
}

void FGI_Next_Process(char func_num)
{
  FGI_VALIABLE.function_number = func_num;
  FGI_VALIABLE.cmd_retry_timer = 0;
  FGI_VALIABLE.cmd_retry_count = 0;
  FGI_VALIABLE.one_time_process = RESET;
  rx_msg_check = MSG_NONE;
  rx_msg_function = CMD_NONE;
}

void FGI_Stop()
{
	FGI_VALIABLE.process = END;
	FGI_VALIABLE.cmd_retry_count = 0;
	FGI_VALIABLE.cmd_retry_timer = 0;
	FGI_VALIABLE.function_number = 0;
	FGI_VALIABLE.one_time_process = RESET;
	rx_msg_check = MSG_NONE;
	rx_msg_function = CMD_NONE;
	qa_stick_check = 0;
	qa_proc_state = 0;
}

void FGI_CAL_Stop()
{
	FGI_CAL_Result = FAIL;
	
	// 캘리브레이션 시작 버튼 - Reset
	SetCtrlVal(tabPanel_FGI, TABPANEL_2_BT_SSN1_CAL, BT_OFF);
	
	// 캘리브레이션 결과 LED - RED 점등
	SetCtrlAttribute (tabPanel_FGI, TABPANEL_2_CAL_RSLT_LED, ATTR_ON_COLOR, VAL_RED);
	SetCtrlVal (tabPanel_FGI, TABPANEL_2_CAL_RSLT_LED, LED_ON);
	
	// 적합/부적합 체크박스 - 부적합 체크
	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX5, UNCHECKED);
	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX5, CHECKED);
}


void SetResultLine(char count)
{
	int value = count / 2;
	
	switch(value)
	{
		case 0:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "----------");
			break;
		case 1:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "---------");
			break;
		case 2:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "--------");
			break;
		case 3:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "-------");
			break;
		case 4:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "------");
			break;
		case 5:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "-----");
			break;
		case 6:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "----");
			break;
		case 7:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "---");
			break;
		case 8:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "--");
			break;
		case 9:
			SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "-");
			break;
		default:
			break;
	}
}