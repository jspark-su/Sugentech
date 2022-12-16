#include "main.h" 

PI_VALIABLE_TypeDef	PI_VALIABLE;

TIME_CHECK_TypeDef	TIME_CHECK;


char auto_setting_mode_state = END_AUTO_SETTING_MODE;
char stick_state_check = 0;
char calibration_proc_state = 0;
char qa_stick_check = 0;
char qa_proc_state = 0;


void PI_Start(int panel);
void PI_Calibration_Stick_Check(int panel);
void PI_Calibration_Start(int panel);
void PI_Calibration_State(int panel);
void PI_Calibration_Result(int panel);
void PI_BLE_Name_Setup_Start(int panel);
void PI_End();
void PI_Variable_Init();
void LED_Calibration_Result_Display(int panel);


void PI_Process()
{
	if(PI_VALIABLE.process == START)
	{
	    switch(PI_VALIABLE.function_number)
	    {
	      case PI_START :
        	PI_Start(tabPanel_PI);
	        break;
	      case PI_CALIBRATION_STICK_CHECK :
        	PI_Calibration_Stick_Check(tabPanel_PI);
	        break;
	      case PI_CALIBRATION_START :
        	PI_Calibration_Start(tabPanel_PI);
	        break;
		  case PI_CALIBRATION_STATE :
        	PI_Calibration_State(tabPanel_PI);
	        break;
		  case PI_CALIBRATION_RESULT :
        	PI_Calibration_Result(tabPanel_PI);
	        break;
	      case PI_BLE_NAME_SETUP_START :
        	PI_BLE_Name_Setup_Start(tabPanel_PI);
	        break;
	      case PI_END :
        	PI_End(tabPanel_PI);
	        break;
	    }
	}
}

void PI_Start(int panel)
{
	SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n*** 자동 설정 모드 시작 ***\r\n\r\n");
	
    memset(&SET_RESULT, 0, sizeof(SET_RESULT));
    memset(&TEST_REPORT_VAR, 0, sizeof(TEST_REPORT_VAR));
    
    PI_Next_Process(PI_CALIBRATION_STICK_CHECK);
}

void PI_Calibration_Stick_Check(int panel)
{
	if(PI_VALIABLE.one_time_process == RESET)
	{
		PI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_STICK_CHECK);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_STICK_CHECK))
	{
		if((stick_state_check == STICK_INSERTED) && (calibration_proc_state == STATE_NONE))
		{
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n1. 캘리브레이션 스틱 삽입 확인 -------------------- [OK]\r\n");
			PI_Next_Process(PI_CALIBRATION_START);
		}
		else //STICK_EJECTED, STATE_DOING, STATE_COMPLETE, STATE_FAIL
		{
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n1. 캘리브레이션 스틱 삽입 확인 -------------------- [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 오류: 캘리브레이션 스틱 분리\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 캘리브레이션 스틱 삽입 후 설정을 재시작하십시오.\n\n");
			PI_Variable_Init();
		}
	}
	else
	{
		PI_VALIABLE.cmd_retry_timer++;
		if(PI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			PI_VALIABLE.cmd_retry_timer = 0;
			if(PI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_STICK_CHECK);
				PI_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n1. 캘리브레이션 스틱 삽입 확인 -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n - 오류: 블루투스 연결을 확인해주십시오.\r\n");
				PI_Variable_Init();	
			}
		}
	}	
}

void PI_Calibration_Start(int panel)
{
	if(PI_VALIABLE.one_time_process == RESET)
	{
		PI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && ((rx_msg_function == CMD_CALIBRATION_START) || (rx_msg_function == CMD_CALIBRATION_STATE_CHECK)))
	{
		if(stick_state_check == STICK_INSERTED)
		{
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\n2. LED 캘리브레이션 시작 -------------------- [OK]\r\n");
			PI_Next_Process(PI_CALIBRATION_STATE);
		}
		else //STICK_EJECTED
		{
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n2. LED 캘리브레이션 시작 -------------------- [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 오류: 캘리브레이션 스틱 분리\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 캘리브레이션 스틱 삽입 후 설정을 재시작하십시오.\n\n");
			PI_Variable_Init();
		}
	}
	else
	{
		PI_VALIABLE.cmd_retry_timer++;
		if(PI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			PI_VALIABLE.cmd_retry_timer = 0;
			if(PI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_START);
				PI_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n2. LED 캘리브레이션 시작 -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n - 오류: 블루투스 연결을 확인해주십시오.\r\n");
				PI_Variable_Init();	
			}
		}
	}	
}

void PI_Calibration_State(int panel)
{
	if(PI_VALIABLE.one_time_process == RESET)
	{
		PI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_STATE_CHECK))
	{
		if((stick_state_check == STICK_INSERTED) && (calibration_proc_state == STATE_COMPLETE)) //STATE_COMPLETE
		{
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n3. LED 캘리브레이션 설정 -------------------- [Complete]\r\n");
            SET_RESULT.led_calibration = TR_PASS;
			PI_Next_Process(PI_CALIBRATION_RESULT);
		}
		else if((stick_state_check == STICK_INSERTED) && (calibration_proc_state == STATE_FAIL)) //STATE_FAIL
		{
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n3. LED 캘리브레이션 설정 -------------------- [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 오류: 기기의 광학부 상태를 점검해주십시오.\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 기기 상태 확인 후 설정을 재시작하십시오.\n\n");
            SET_RESULT.led_calibration = TR_FAIL;
			PI_Next_Process(PI_CALIBRATION_RESULT);
			//PI_Variable_Init();
		}
		else if(stick_state_check == STICK_EJECTED)
		{
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n3. LED 캘리브레이션 설정 -------------------- [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 오류: 캘리브레이션 스틱 분리\r\n");
			SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " - 캘리브레이션 스틱 삽입 후 설정을 재시작하십시오.n\n");
			PI_Variable_Init();
		}
	}
	else
	{
		PI_VALIABLE.cmd_retry_timer++;
		if(PI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			PI_VALIABLE.cmd_retry_timer = 0;
			if(PI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
				PI_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n3. LED 캘리브레이션 설정 -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n - 오류: 블루투스 연결을 확인해주십시오.\r\n");
                SET_RESULT.led_calibration = TR_FAIL;
				PI_Variable_Init();	
			}
		}
	}	
}

void PI_Calibration_Result(int panel)
{
	if(PI_VALIABLE.one_time_process == RESET)
	{
		PI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_RESULT);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_RESULT))
	{
		SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n4. LED 캘리브레이션 결과 -------------------- [OK]\r\n");
		LED_Calibration_Result_Display(panel);
        
        if(BLE_FLAG.ble_name_setup_complete_check == BLE_NAME_SETUP_CHECK_SUCCESS) //BLE name 설정 정상으로 설정 진행 필요 없음
        {
            SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\n5. BLE name 설정 -------------------- [PASS]\r\n");
		    SET_RESULT.ble_name = TR_PASS;
		    PI_Next_Process(PI_END);
        }
        else if(BLE_FLAG.ble_name_setup_complete_check == BLE_NAME_SETUP_CHECK_FAIL) //BLE name 설정 문제로 설정 진행 필요
        {
            PI_Next_Process(PI_BLE_NAME_SETUP_START);
        }
	}
	else
	{
		PI_VALIABLE.cmd_retry_timer++;
		if(PI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			PI_VALIABLE.cmd_retry_timer = 0;
			if(PI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_RESULT);
				PI_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n4. LED 캘리브레이션 결과 -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n - 오류: 블루투스 연결을 확인해주십시오.\r\n");
				PI_Variable_Init();	
			}
		}
	}	
}

void PI_BLE_Name_Setup_Start(int panel)
{
	if(PI_VALIABLE.one_time_process == RESET)
	{
		PI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_BLE_NAME_SETUP_START);
        SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\n5. BLE name 설정 진행");
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_BLE_NAME_SETUP_START))
	{
        SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " -------------------- [Complete]\r\n");
        SET_RESULT.ble_name = TR_PASS;
    	PI_Next_Process(PI_END);
	}
	else
	{
		PI_VALIABLE.cmd_retry_timer++;
		if(PI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			PI_VALIABLE.cmd_retry_timer = 0;
			if(PI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_BLE_NAME_SETUP_START);
				PI_VALIABLE.cmd_retry_count++;
			}
			else
			{
                SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, " -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n - 오류: 블루투스 연결을 확인해주십시오.\r\n");
                SET_RESULT.ble_name = TR_FAIL;
				PI_Variable_Init();	
			}
		}
	}	
}

void PI_End(int panel)
{
	if(PI_VALIABLE.one_time_process == RESET)
	{
		PI_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_PRODUCT_MODE_END);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_END))
	{
        Set_Test_End_Time();
        Set_User_Dev_Info();
		SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n*** 자동 설정 모드 종료 ***\r\n");
		PI_Variable_Init();
	}
	else
	{
		PI_VALIABLE.cmd_retry_timer++;
		if(PI_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			PI_VALIABLE.cmd_retry_timer = 0;
			if(PI_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_END);
				PI_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n6. 자동 설정 모드 종료 -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n - 오류: 블루투스 연결을 확인해주십시오.\r\n");
				PI_Variable_Init();	
			}
		}
	}	
}

void PI_Variable_Init()
{
	PI_VALIABLE.process = END;
	PI_VALIABLE.cmd_retry_count = 0;
	PI_VALIABLE.cmd_retry_timer = 0;
	PI_VALIABLE.function_number = 0;
	PI_VALIABLE.one_time_process = RESET;
	stick_state_check = 0;
	calibration_proc_state = 0;
	rx_msg_check = MSG_NONE;
	rx_msg_function = CMD_NONE;
	qa_stick_check = 0;
	qa_proc_state = 0;
    memset(&DATA_BANK, 0, sizeof(DATA_BANK));
    auto_setting_mode_state = END_AUTO_SETTING_MODE;
    SetCtrlVal(tabPanel_PI,TABPANEL_BT_AUTO_SETTINGS, 0);
}

void PI_Next_Process(char func_num)
{
  PI_VALIABLE.function_number = func_num;
  PI_VALIABLE.cmd_retry_timer = 0;
  PI_VALIABLE.cmd_retry_count = 0;
  PI_VALIABLE.one_time_process = RESET;
  rx_msg_check = MSG_NONE;
  rx_msg_function = CMD_NONE;
}

void LED_Calibration_Result_Display(int panel)
{
  char i = 0;
  char str_buf[100] = {0};
  
  SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n\r\nLine     L1       L2       L3       L4       L5     L6[G]  L7[R]\r\n");
  
  SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "DAC   ");
  for(i = 1; i <= 7; i++)
  {
	  sprintf(str_buf, "%5d  ", DATA_BANK.CALIBRATION_DATA[i]);
	  SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, str_buf);
	  memset(str_buf, 0, sizeof(str_buf));
  }

  SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\nPTR   ");
  
  for(i = 1; i <= 7; i++)
  {
	sprintf(str_buf, "%.1f ", (float)(DATA_BANK.INITIAL_STICK_SENSING_DATA[i] / 10.0));
	SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, str_buf);
	memset(str_buf, 0, sizeof(str_buf));
  }
  SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n\r\n");
}
