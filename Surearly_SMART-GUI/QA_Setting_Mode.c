#include "main.h"
#if 0
PI_VALIABLE_TypeDef QA_VALIABLE;


void QA_Read_Standard_Stick_Process()
{
	if(QA_VALIABLE.process == START)
	{
	    switch(QA_VALIABLE.function_number)
	    {
	      case QA_DATA_SETTING_START :
        	QA_Data_Setting_Start(tabPanel_QA);
	        break;
		  case QA_STICK_INSERT_CHECK :
        	QA_Stick_Insert_Check(tabPanel_QA); 
	        break;	
		  case QA_READ_STD_STICK_START :
        	QA_Read_Standard_Stick_Start(tabPanel_QA);
	        break;
          case QA_READ_STD_STICK_RESULT :
        	QA_Read_Standard_Stick_Result(tabPanel_QA);
	        break;  
	      case QA_DATA_SETTING_END :
        	QA_Data_Setting_End(tabPanel_QA);
	        break;
	    }
	}
}

void QA_Data_Setting_Start(int panel)
{
    if(QA_VALIABLE.one_time_process == RESET)
	{
		QA_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_START))
	{
		SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n*** Start Standard Stick Data Setting ***\r\n\r\n");
		QA_Next_Process(QA_STICK_INSERT_CHECK);		
	}
	else
	{
		QA_VALIABLE.cmd_retry_timer++;
		if(QA_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_VALIABLE.cmd_retry_timer = 0;
			if(QA_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
				QA_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\nError: Requires checking device power on and BLE module status\r\n");
				QA_Stop();	
			}
		}
	}    
}

void QA_Stick_Insert_Check(int panel)
{
    if(QA_VALIABLE.one_time_process == RESET)
	{
		QA_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_QA_STICK_CHECK);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_CALIBRATION_STICK_CHECK)) 
	{
		if(stick_state_check == STICK_INSERTED)
		{
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n1. Standard stick insertion check -------------------- [OK]\r\n");
			QA_Next_Process(QA_READ_STD_STICK_START);
		}
		else
		{
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n1. Standard stick insertion check -------------------- [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, " - Error: Stick detachment\r\n");
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, " - Please insert the standard stick and start again.\n\n");
			QA_Stop();
		}
	}
	else
	{
		QA_VALIABLE.cmd_retry_timer++;
		if(QA_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_VALIABLE.cmd_retry_timer = 0;
			if(QA_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_QA_STICK_CHECK);
				QA_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n1. Standard stick check -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n - Error: Please check the BLE connection.\r\n");
				QA_Stop();
			}
		}
	}    
}

void QA_Read_Standard_Stick_Start(int panel)
{
    if(QA_VALIABLE.one_time_process == RESET)
	{
		QA_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_QA_START);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_QA_START))
	{
		if(stick_state_check == STICK_INSERTED)
		{
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n2. Read standard stick data -------------------- [OK]\r\n");
			QA_Next_Process(QA_READ_STD_STICK_RESULT);
		}
		else //STICK_EJECTED
		{
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n2. Read standard stick data -------------------- [Fail]\r\n");
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, " - Error: Stick detachment\r\n");
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, " - Please insert the standard stick and start again.\n\n");
			QA_Stop();
		}
	}
	else
	{
		QA_VALIABLE.cmd_retry_timer++;
		if(QA_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_VALIABLE.cmd_retry_timer = 0;
			if(QA_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_QA_START);
				QA_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n2. Read standard stick data -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n - Error: Please check the BLE connection.\r\n");
				QA_Stop();	
			}
		}
	}    
}

void QA_Read_Standard_Stick_Result(int panel)
{
    if(QA_VALIABLE.one_time_process == RESET)
	{
		QA_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_QA_RESULT);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_QA_RESULT))
	{
		if(qa_proc_state == STATE_COMPLETE)
		{
			SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n3. Result standard stick data -------------------- [Complete]\r\n");
			QA_Stick_Sensing_Rawdata_Display(tabPanel_QA);
            QA_Set_Standard_Stick_Data(tabPanel_QA);
			QA_Next_Process(QA_DATA_SETTING_END);
		}
		else //STATE_DOING
        {
            rx_msg_check = MSG_NONE;
            API_Tx_Command_Process(CMD_QA_RESULT);
        }
	}
	else
	{
		QA_VALIABLE.cmd_retry_timer++;
		if(QA_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_VALIABLE.cmd_retry_timer = 0;
			if(QA_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_QA_RESULT);
				QA_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n3. Result standard stick data -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n - Error: Please check the BLE connection.\r\n");
				QA_Stop();	
			}
		}
	}    
}

void QA_Data_Setting_End(int panel)
{
    if(QA_VALIABLE.one_time_process == RESET)
	{
		QA_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_PRODUCT_MODE_END);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_END))
	{
		SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n\t\t*** End Standard Stick Data Setting ***\r\n");
		QA_Stop();
	}
	else
	{
		QA_VALIABLE.cmd_retry_timer++;
		if(QA_VALIABLE.cmd_retry_timer >= CMD_RETRY_TIME)
		{
			QA_VALIABLE.cmd_retry_timer = 0;
			if(QA_VALIABLE.cmd_retry_count < CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_END);
				QA_VALIABLE.cmd_retry_count++;
			}
			else
			{
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n4. End Standard Stick Data Setting -------------------- [Fail]\r\n");
				SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n - Error: Please check the BLE connection.\r\n");
				QA_Stop();	
			}
		}
	}    
}

void QA_Stop()
{
	QA_VALIABLE.process = END;
    QA_VALIABLE.one_time_process = RESET;
    QA_VALIABLE.function_number = 0;
	QA_VALIABLE.cmd_retry_count = 0;
	QA_VALIABLE.cmd_retry_timer = 0;
	stick_state_check = 0;
	rx_msg_check = MSG_NONE;
	rx_msg_function = CMD_NONE;
	qa_stick_check = 0;
	qa_proc_state = 0;
}

void QA_Next_Process(char func_num)
{
  QA_VALIABLE.function_number = func_num;
  QA_VALIABLE.cmd_retry_timer = 0;
  QA_VALIABLE.cmd_retry_count = 0;
  QA_VALIABLE.one_time_process = RESET;
  rx_msg_check = MSG_NONE;
  rx_msg_function = CMD_NONE;
}

void QA_Stick_Sensing_Rawdata_Display(int panel)
{
    char i = 0;
    char str_buf[100] = {0};

    SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "\r\n\r\nLine     L1       L2       L3       L4       L5     L6[G]  L7[R]\r\n");
    
    SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, "Stick   ");
  
    for(i = 1; i <= 7; i++)
    {
	    sprintf(str_buf, "%.1f  ", (float)(DATA_BANK.STD_STICK_SENSING_RAW_DATA[i] / 10.0));
	    SetCtrlVal(panel, TABPANEL_6_SERIAL_MONITOR_QA, str_buf);
	    memset(str_buf, 0, sizeof(str_buf));
    }
    SetCtrlVal(panel, TABPANEL_LOG_MONITOR_PI, "\r\n\r\n");
}

void QA_Set_Standard_Stick_Data(int panel) //Read Standard Stick Data?? ?¬à?? Standard stick sensing raw data?? Control panel "Standard stick data setting" ????
{
    SetCtrlVal (panel, TABPANEL_6_INPUT_STD_S_DATA_CH1, (float)DATA_BANK.STD_STICK_SENSING_RAW_DATA[1] / 10.0);
	SetCtrlVal (panel, TABPANEL_6_INPUT_STD_S_DATA_CH2, (float)DATA_BANK.STD_STICK_SENSING_RAW_DATA[2] / 10.0);
	SetCtrlVal (panel, TABPANEL_6_INPUT_STD_S_DATA_CH3, (float)DATA_BANK.STD_STICK_SENSING_RAW_DATA[3] / 10.0);
	SetCtrlVal (panel, TABPANEL_6_INPUT_STD_S_DATA_CH4, (float)DATA_BANK.STD_STICK_SENSING_RAW_DATA[4] / 10.0);
	SetCtrlVal (panel, TABPANEL_6_INPUT_STD_S_DATA_CH5, (float)DATA_BANK.STD_STICK_SENSING_RAW_DATA[5] / 10.0);
	SetCtrlVal (panel, TABPANEL_6_INPUT_STD_S_DATA_CH6, (float)DATA_BANK.STD_STICK_SENSING_RAW_DATA[6] / 10.0); 
	SetCtrlVal (panel, TABPANEL_6_INPUT_STD_S_DATA_CH7, (float)DATA_BANK.STD_STICK_SENSING_RAW_DATA[7] / 10.0);    
}
#endif
