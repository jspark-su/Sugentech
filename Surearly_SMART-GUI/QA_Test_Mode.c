#include "main.h"

PI_VALIABLE_TypeDef QA_STICK_SENSING_MODE_VARIABLE;
PI_VALIABLE_TypeDef QA_CALIB_VARIABLE;



void QA_Stick_Sensing_Mode_Process()
{
	if(QA_STICK_SENSING_MODE_VARIABLE.process == START)
	{
	    switch(QA_STICK_SENSING_MODE_VARIABLE.function_number)
	    {
	      case QA_STICK_SENSING_MODE_START :
        	QA_Stick_Sensing_Mode_Start(tabPanel_QA_Test);
	        break;
		  case QA_STICK_SENSING_START :
        	QA_Stick_Sensing_Start(tabPanel_QA_Test);
	        break;
          case QA_STICK_SENSING_RESULT :
        	QA_Stick_Sensing_Result(tabPanel_QA_Test);
	        break;
	    }
	}
}

void QA_Stick_Sensing_Mode_Start(int panel)
{
    if(QA_STICK_SENSING_MODE_VARIABLE.one_time_process == RESET)
	{
		QA_STICK_SENSING_MODE_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_START))
	{
		SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n*** Stick 스캔 모드 시작 ***\n");
		QA_Stick_Sensing_Mode_Next_Process(QA_STICK_SENSING_START);
	}
	else
	{
		QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer++;
		if(QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer = 0;
			if(QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
				QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n오류: 기기 전원, 블루투스 상태를 확인하십시오.\n");
				QA_Stick_Sensing_Mode_Stop();	
			}
		}
	}
}

void QA_Stick_Sensing_Start(int panel)
{
    if(QA_STICK_SENSING_MODE_VARIABLE.one_time_process == RESET)
	{
		QA_STICK_SENSING_MODE_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_QA_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_QA_START))
	{
		SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "1. Stick 스캔 진행 -------------------- [OK]\n");
		QA_Stick_Sensing_Mode_Next_Process(QA_STICK_SENSING_RESULT);
	}
	else
	{
		QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer++;
		if(QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer = 0;
			if(QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_QA_START);
				QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "1. Stick 스캔 진행 -------------------- [Fail]\n");
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n - 오류: 블루투스 연결을 확인해주십시오.\n");
				QA_Stick_Sensing_Mode_Stop();	
			}
		}
	}    
}

void QA_Stick_Sensing_Result(int panel)
{
    if(QA_STICK_SENSING_MODE_VARIABLE.one_time_process == RESET)
	{
		QA_STICK_SENSING_MODE_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_QA_RESULT);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_QA_RESULT))
	{
		if(qa_proc_state == STATE_COMPLETE)
		{
			QA_Stick_Sensing_Result_Display(tabPanel_QA_Test);
			QA_Stick_Sensing_Mode_Stop();
		}
	}
	else
	{
		QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer++;
		if(QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer = 0;
			if(QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_QA_RESULT);
				QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n - 오류: 블루투스 연결을 확인해주십시오.\n");
				QA_Stick_Sensing_Mode_Stop();	
			}
		}
	}    
}

void QA_Stick_Sensing_Result_Display(int panel)
{
    char i = 0;
    char str_buf[100] = {0};
    
    SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\nLine          L1         L2        L3         L4        L5        L6[G]     L7[R]\n");
    SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "Ratio         ");
    for(i = 1; i <= 7; i++)
    {
	    sprintf(str_buf, "%.2f%%  ", (float)(DATA_BANK.CHANNEL_RATIO_DATA[i]));
	    SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, str_buf);
	    memset(str_buf, 0, sizeof(str_buf));
    }
    SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n");
    
    DATA_BANK.RSLT_T1_RAWDATA = (DATA_BANK.CHANNEL_RATIO_DATA[2] / DATA_BANK.CHANNEL_RATIO_DATA[1]) * 1000.0;
    DATA_BANK.RSLT_T2_RAWDATA = (DATA_BANK.CHANNEL_RATIO_DATA[2] / DATA_BANK.CHANNEL_RATIO_DATA[3]) * 1000.0;
    DATA_BANK.RSLT_T3_RAWDATA = (DATA_BANK.CHANNEL_RATIO_DATA[4] / DATA_BANK.CHANNEL_RATIO_DATA[5]) * 1000.0;
    DATA_BANK.RSLT_COLOR_RAWDATA = (DATA_BANK.CHANNEL_RATIO_DATA[6] / DATA_BANK.CHANNEL_RATIO_DATA[7]) * 1000.0;
    
    SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\nLine          T1         T2        Control     Color\n");
    sprintf(str_buf, "Rawdata    %.1f   %.1f   %.1f      %.1f", DATA_BANK.RSLT_T1_RAWDATA, DATA_BANK.RSLT_T2_RAWDATA, DATA_BANK.RSLT_T3_RAWDATA, DATA_BANK.RSLT_COLOR_RAWDATA);
    SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, str_buf);
	memset(str_buf, 0, sizeof(str_buf));
    SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n");
}

void QA_Stick_Sensing_Mode_Stop()
{
	QA_STICK_SENSING_MODE_VARIABLE.process = END;
    QA_STICK_SENSING_MODE_VARIABLE.one_time_process = RESET;
    QA_STICK_SENSING_MODE_VARIABLE.function_number = 0;
	QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count = 0;
	QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer = 0;
	stick_state_check = 0;
	rx_msg_check = MSG_NONE;
	rx_msg_function = CMD_NONE;
	qa_stick_check = 0;
	qa_proc_state = 0;
}

void QA_Stick_Sensing_Mode_Next_Process(char func_num)
{
  QA_STICK_SENSING_MODE_VARIABLE.function_number = func_num;
  QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_timer = 0;
  QA_STICK_SENSING_MODE_VARIABLE.cmd_retry_count = 0;
  QA_STICK_SENSING_MODE_VARIABLE.one_time_process = RESET;
  rx_msg_check = MSG_NONE;
  rx_msg_function = CMD_NONE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void QA_Calibration_Process()
{
	if(QA_CALIB_VARIABLE.process == START)
	{
	    switch(QA_CALIB_VARIABLE.function_number)
	    {
	      case QA_CALIBRATION_MODE_START :
        	QA_Calibration_Mode_Start(tabPanel_QA_Test);
	        break;
		  case QA_CALIBRATION_STICK_CHECK :
			QA_Calibration_Stick_Check(tabPanel_QA_Test);
			break;
		  case QA_CALIBRATION_START :
        	QA_Calibration_Start(tabPanel_QA_Test);
	        break;
          case QA_CALIBRATION_STATE :
        	QA_Calibration_State(tabPanel_QA_Test);
	        break;  
	      case QA_CALIBRATION_RESULT :
        	QA_Calibration_Result(tabPanel_QA_Test);
	        break;
	    }
	}
}

void QA_Calibration_Mode_Start(int panel)
{
    if(QA_CALIB_VARIABLE.one_time_process == RESET)
	{
		QA_CALIB_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_START))
	{
		SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n*** LED 캘리브레이션 모드 시작 ***\n");
		QA_Calib_Next_Process(QA_CALIBRATION_STICK_CHECK);
        //QA_Calib_Next_Process(QA_CALIBRATION_START);
	}
	else
	{
		QA_CALIB_VARIABLE.cmd_retry_timer++;
		if(QA_CALIB_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_CALIB_VARIABLE.cmd_retry_timer = 0;
			if(QA_CALIB_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
				QA_CALIB_VARIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n오류: 기기 전원, 블루투스 상태를 확인하십시오.\n");
				QA_Calib_Stop();
			}
		}
	}
}

void QA_Calibration_Stick_Check(int panel)
{
    if(QA_CALIB_VARIABLE.one_time_process == RESET)
	{
		QA_CALIB_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_STICK_CHECK))
	{
		if((stick_state_check == STICK_INSERTED) && (calibration_proc_state == STATE_NONE))
		{
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "1. 캘리브레이션 스틱 삽입 확인  --------------- [OK]\r\n");
			QA_Calib_Next_Process(QA_CALIBRATION_START);
		}
		else //STICK_EJECTED
		{
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "1. 캘리브레이션 스틱 삽입 확인  --------------- [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, " - 오류: 캘리브레이션 스틱 분리\r\n");
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, " - 캘리브레이션 스틱 삽입 후 설정을 재시작하십시오.\n\n");
			QA_Calib_Stop();
		}
	}
	else
	{
		QA_CALIB_VARIABLE.cmd_retry_timer++;
		if(QA_CALIB_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_CALIB_VARIABLE.cmd_retry_timer = 0;
			if(QA_CALIB_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
				QA_CALIB_VARIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "1. 캘리브레이션 스틱 삽입 확인 -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n오류: 기기 전원, 블루투스 상태를 확인하십시오.\n");
				QA_Calib_Stop();
			}
		}
	}	
	
}

void QA_Calibration_Start(int panel)
{
	if(QA_CALIB_VARIABLE.one_time_process == RESET)
	{
		QA_CALIB_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_START);
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && ((rx_msg_function == CMD_CALIBRATION_START) || (rx_msg_function == CMD_CALIBRATION_STATE_CHECK)))
	{
		SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "2. LED 캘리브레이션 진행 -------------------- [OK]\n");
		QA_Calib_Next_Process(QA_CALIBRATION_STATE);
	}
	else
	{
		QA_CALIB_VARIABLE.cmd_retry_timer++;
		if(QA_CALIB_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_CALIB_VARIABLE.cmd_retry_timer = 0;
			if(QA_CALIB_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_START);
				QA_CALIB_VARIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "2. LED 캘리브레이션 진행 -------------------- [Fail]\n");
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, " - 오류: 블루투스 연결을 확인해주십시오.\n");
				QA_Calib_Stop();	
			}
		}
	}	
}

void QA_Calibration_State(int panel)
{
	if(QA_CALIB_VARIABLE.one_time_process == RESET)
	{
		QA_CALIB_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_STATE_CHECK))
	{
		if((stick_state_check == STICK_INSERTED) && (calibration_proc_state == STATE_COMPLETE)) //STATE_COMPLETE
		{
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "3. LED 캘리브레이션 설정 -------------------- [Complete]\n");
            SET_RESULT.led_calibration = TR_PASS;
			QA_Calib_Next_Process(QA_CALIBRATION_RESULT);
		}
		else if((stick_state_check == STICK_INSERTED) && (calibration_proc_state == STATE_FAIL)) //STATE_FAIL
		{
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "3. LED 캘리브레이션 설정 -------------------- [Fail]\n");
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, " - 오류: 기기의 광학부 상태를 점검해주십시오.\n");
			SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, " - 기기 상태 확인 후 설정을 재시작하십시오.\n");
            SET_RESULT.led_calibration = TR_FAIL;
			QA_Calib_Next_Process(QA_CALIBRATION_RESULT);
		}
	}
	else
	{
		QA_CALIB_VARIABLE.cmd_retry_timer++;
		if(QA_CALIB_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_CALIB_VARIABLE.cmd_retry_timer = 0;
			if(QA_CALIB_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_STATE_CHECK);
				QA_CALIB_VARIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "3. LED 캘리브레이션 설정 -------------------- [Fail]\n");
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, " - 오류: 블루투스 연결을 확인해주십시오.\r\n");
                SET_RESULT.led_calibration = TR_FAIL;
				QA_Calib_Stop();	
			}
		}
	}	
}

void QA_Calibration_Result(int panel)
{
	if(QA_CALIB_VARIABLE.one_time_process == RESET)
	{
		QA_CALIB_VARIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_CALIBRATION_RESULT);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_RESULT))
	{
		//SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "3. Calibration result -------------------- [OK]\n");
		QA_Calibration_Result_Display(panel);
		QA_Calib_Stop();
	}
	else
	{
		QA_CALIB_VARIABLE.cmd_retry_timer++;
		if(QA_CALIB_VARIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_CALIB_VARIABLE.cmd_retry_timer = 0;
			if(QA_CALIB_VARIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_CALIBRATION_RESULT);
				QA_CALIB_VARIABLE.cmd_retry_count++;
			}
			else
			{
				//SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "3. Calibration result -------------------- [Fail]\n");
				SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, " - 오류: 블루투스 연결을 확인해주십시오.\n");
				QA_Calib_Stop();	
			}
		}
	}	
}

void QA_Calibration_Result_Display(int panel)
{
  char i = 0;
  char str_buf[100] = {0};
  
  SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\nLine     L1       L2      L3      L4      L5    L6[G]  L7[R]\n");
  
  SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "DAC   ");
  for(i = 1; i <= 7; i++)
  {
	  sprintf(str_buf, "%5d  ", DATA_BANK.CALIBRATION_DATA[i]);
	  SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, str_buf);
	  memset(str_buf, 0, sizeof(str_buf));
  }

  SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\nPTR   ");
  
  for(i = 1; i <= 7; i++)
  {
	sprintf(str_buf, "%.1f ", (float)(DATA_BANK.INITIAL_STICK_SENSING_DATA[i] / 10.0));
	SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, str_buf);
	memset(str_buf, 0, sizeof(str_buf));
  }
  SetCtrlVal(panel, TABPANEL_9_MONITOR_QA_TEST, "\n");
}

void QA_Calib_Stop()
{
	QA_CALIB_VARIABLE.process = END;
    QA_CALIB_VARIABLE.one_time_process = RESET;
    QA_CALIB_VARIABLE.function_number = 0;
	QA_CALIB_VARIABLE.cmd_retry_count = 0;
	QA_CALIB_VARIABLE.cmd_retry_timer = 0;
	stick_state_check = 0;
	rx_msg_check = MSG_NONE;
	rx_msg_function = CMD_NONE;
	qa_stick_check = 0;
	qa_proc_state = 0;
}

void QA_Calib_Next_Process(char func_num)
{
  QA_CALIB_VARIABLE.function_number = func_num;
  QA_CALIB_VARIABLE.cmd_retry_timer = 0;
  QA_CALIB_VARIABLE.cmd_retry_count = 0;
  QA_CALIB_VARIABLE.one_time_process = RESET;
  rx_msg_check = MSG_NONE;
  rx_msg_function = CMD_NONE;
}
