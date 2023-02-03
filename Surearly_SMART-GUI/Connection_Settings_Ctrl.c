#include "main.h" 

char production_mode_initializing = 0;
char one_time_process = 0;
int cmd_retry_timer = 0;
int cmd_retry_count = 0;

char dev_uid[25] = {0};
char fw_ver[20] = {0};

void Connect_Settings_Popup()
{
    subPanel = LoadPanel (1, "Connection_Settings.uir", SUB_PANEL);
	InstallPopup (subPanel);
}


void Production_Mode_Init(int panel)
{
    if(production_mode_initializing == START)
    {
    	if(one_time_process == RESET)
    	{
    		one_time_process = SET;
    		API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
    	}

    	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_START))
    	{
    		SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n > 기기 정보 불러오기 -------------------- [OK]\r\n");
    		SetCtrlVal(panel, SUB_PANEL_sINDI_FW_VER, fw_ver);
    		SetCtrlVal(panel, SUB_PANEL_sINDI_MCU_UID, dev_uid);
            SetCtrlVal(panel, SUB_PANEL_sINDI_BLE_NAME, ble_name);
            production_mode_initializing = END;
            rx_msg_check = MSG_NONE;
            rx_msg_function = CMD_NONE;
    	}
    	else
    	{
    		cmd_retry_timer++;
    		if(cmd_retry_timer >= CMD_RETRY_TIME)
    		{
    			cmd_retry_timer = 0;
    			if(cmd_retry_count < CMD_RETRY_COUNT_NUM)
    			{
    				API_Tx_Command_Process(CMD_PRODUCT_MODE_START);
    				cmd_retry_count++;
    			}
    			else
    			{
                    SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n > 기기 정보 불러오기 -------------------- [Fail]\r\n");
    				SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n오류: 기기 전원, 블루투스 상태를 확인하십시오.\r\n");
                    production_mode_initializing = END;
                    rx_msg_check = MSG_NONE;
                    rx_msg_function = CMD_NONE;
    			}
    		}
    	}
    }
    else
    {
    	one_time_process = 0;
        cmd_retry_timer = 0;
        cmd_retry_count = 0;
    }
}

void Connection_Settings_Init(int panel)
{
    memset(fw_ver, 0, sizeof(fw_ver));
    memset(dev_uid, 0, sizeof(dev_uid));
    memset(ble_name, 0, sizeof(ble_name));
    memset(ble_mac_id, 0, sizeof(ble_mac_id));
    SetCtrlVal (panel, SUB_PANEL_sINPUT_BLE_MAC_ADDR, 0); //연결 및 설정 화면의 MAC 주소 항목 초기화
    SetCtrlVal (panel, SUB_PANEL_sINDI_FW_VER, 0); //연결 및 설정 화면의 F/W 버전 항목 초기화
    SetCtrlVal (panel, SUB_PANEL_sINDI_MCU_UID, 0); //연결 및 설정 화면의 MCU UID 항목 초기화
    SetCtrlVal (panel, SUB_PANEL_sINDI_BLE_NAME, 0); //연결 및 설정 화면의 BLE 이름 항목 초기화
    SetCtrlVal (panel, SUB_PANEL_DEVICE_CONNECT_LED, 0); //Device의 BLE name 설정 확인 결과 표시 LED 초기화
}
