#include "Connection_Settings.h"
//#include <userint.h>
#include "Main_UI.h"
#include "main.h"         

int mainPanel;

int tabPanel_PI;
int tabPanel_FGI;
int tabPanel_QA;
int tabPanel_QA_Test;
int tabPanel_FW_Update;
int tabPanel_RP;

DATA_BANK_TypeDef DATA_BANK;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "Main_UI.uir", MAIN_PANEL)) < 0)
		return -1;
	
	GetPanelHandleFromTabPage(mainPanel, MAIN_PANEL_TAB_SETTING_MODE, 0, &tabPanel_PI);
    GetPanelHandleFromTabPage(mainPanel, MAIN_PANEL_TAB_SETTING_MODE, 1, &tabPanel_FGI);
    GetPanelHandleFromTabPage(mainPanel, MAIN_PANEL_TAB_SETTING_MODE, 2, &tabPanel_QA_Test);
    GetPanelHandleFromTabPage(mainPanel, MAIN_PANEL_TAB_SETTING_MODE, 3, &tabPanel_FW_Update);
	GetPanelHandleFromTabPage(mainPanel, MAIN_PANEL_TAB_SETTING_MODE, 4, &tabPanel_RP);
    
    SetCtrlVal(tabPanel_FGI, TABPANEL_2_INPUT_DEV_SERIAL_NUM, DEV_SERIAL_INIT_STR); //완제품 검사 메인 화면의 기기 시리얼 번호 항목 초기화
    
	// TODO: Get the panel handle associated with a tab page with GetPanelHandleFromTabPage
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED)
	{
        CloseCom(SERIAL_CONFIG.port_number);
    }
    
	return 0;
}

int CVICALLBACK MAIN_PANEL_FUNC (int panel, int event, void *callbackData,
								 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			/*for(int i = 0; i < 3; i++)
			{
				API_Tx_Command_Process(CMD_PRODUCT_MODE_END);
				//delay_ms
				Delay(1);
				if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_PRODUCT_MODE_END))
				{
            		QuitUserInterface (0);
				}
			}*/
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK TAB_SETTING_MODE_FUNC (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	
	return 0;
}

int CVICALLBACK TAB_CUTOFF_SETTING_FUNC (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	return 0;
}

int CVICALLBACK BT_AUTO_SETTINGS (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                if(auto_setting_mode_state == END_AUTO_SETTING_MODE)
                {
                    auto_setting_mode_state = START_AUTO_SETTING_MODE;
    			    PI_VALIABLE.function_number = PI_START;
    			    PI_VALIABLE.process = START;
                    SetCtrlVal(panel,TABPANEL_BT_AUTO_SETTINGS, 1);
                }
                else if(auto_setting_mode_state == START_AUTO_SETTING_MODE)
                {
                    auto_setting_mode_state = END_AUTO_SETTING_MODE;
                    PI_Variable_Init();
                    SetCtrlVal(tabPanel_PI, TABPANEL_LOG_MONITOR_PI, "\r\n > 자동 설정 모드 강제 종료.\r\n");
                    SetCtrlVal(panel,TABPANEL_BT_AUTO_SETTINGS, 0);
                }
            }
            else{
                SetCtrlVal(tabPanel_PI, TABPANEL_LOG_MONITOR_PI, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
                SetCtrlVal(panel,TABPANEL_BT_AUTO_SETTINGS, 0);
            }
			break;
	}
	return 0;
}

int CVICALLBACK BT_RD_STD_STICK_DATA (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK BT_TEST_REPORT_LOAD (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			break;
	}
	return 0;
}

int CVICALLBACK BT_SAVE_REPORT_PI (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            int ret;
			int save_check = 0;
			char Buff[260]={0};
            char save_file_name[200]={0};
            sprintf(save_file_name,"Settings_Report_%s.csv", ble_mac_id);
            ret = FileSelectPopup ("", save_file_name, "*.csv", "Save Data File", VAL_SAVE_BUTTON, 0, 0, 1, 0, Buff);
			if(ret > 0)
			{
				save_check = PI_Save_Test_Report(tabPanel_PI, Buff);
				if(save_check > 0){
					SetCtrlVal(tabPanel_PI, TABPANEL_LOG_MONITOR_PI, "\r\n > Test report 저장 완료 !\r\n");	
				}
				else{
					SetCtrlVal(tabPanel_PI, TABPANEL_LOG_MONITOR_PI, "\r\n > Test report 저장 실패.\r\n");	
				}
			}
			break;
	}
	return 0;
}

int CVICALLBACK TIMER_1msec (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
            
			BLE_Scan_Process(subPanel);
            
			BLE_Rx_Dequeue(subPanel);
            
            if((BLE_RX.mode_sel == BLE_CMD_MODE) || (BLE_RX.mode_sel == API_CMD_MODE))
            {
			    BLE_AT_Command_Process(subPanel);
			    API_Rx_Command_Process();
            }
            else if(BLE_RX.mode_sel == FW_UPD_CMD_MODE){}
            
		  	if(BLE_RX.tact_time_flag){
		    	BLE_RX.tact_time++;  
		  	}
            else{BLE_RX.tact_time = 0;}
            
			if(BLE_FLAG.ble_scan_state == BLE_SCAN_START){
				BLE_RX.scan_time++;
			}
            else{BLE_RX.scan_time = 0;}
            
            if(BLE_FLAG.connect_tact_time_flag == BLE_CONNECT_TACT_TIME_START)
            {
                BLE_FLAG.connect_tact_time++;
                if(BLE_FLAG.connect_tact_time == 1000)
                {
                    BLE_FLAG.connect_tact_time = 0;
                    BLE_Connection_Process();   
                }
            }
            else{BLE_FLAG.connect_tact_time = 0;}
            
			break;
	}
	return 0;
}

int CVICALLBACK TIMER_10msec (int panel, int control, int event,
                              void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_TIMER_TICK:
            FW_Update_Function_Process();
            break;
    }
    return 0;
}

int CVICALLBACK TIMER_100msec (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			PI_Process();
            FGI_Read_Std_Stick_Process();
            QA_Stick_Sensing_Mode_Process();
			QA_Calibration_Process();
			RP_Process();
            Production_Mode_Init(subPanel);
			break;
	}
	return 0;
}

int CVICALLBACK BT_QA_CLIB_START (int panel, int control, int event,
                                  void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                QA_CALIB_VARIABLE.function_number = QA_CALIBRATION_MODE_START;
    			QA_CALIB_VARIABLE.process = START;
            }
            else{
                SetCtrlVal(tabPanel_QA_Test, TABPANEL_9_MONITOR_QA_TEST, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_READ_STICK_RAW (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                QA_STICK_SENSING_MODE_VARIABLE.function_number = QA_STICK_SENSING_MODE_START;
    			QA_STICK_SENSING_MODE_VARIABLE.process = START;
            }
            else{
                SetCtrlVal(tabPanel_QA_Test, TABPANEL_9_MONITOR_QA_TEST, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK MONITOR_QA_TEST_CLEAR (int panel, int control, int event,
                                       void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            DeleteTextBoxLines (tabPanel_QA_Test, TABPANEL_9_MONITOR_QA_TEST, 0, -1);
            break;
    }
    return 0;
}

int CVICALLBACK BT_SSM1_DATA_READ (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > SSM1 표준 스틱 측정중..\r\n");
                std_stick_number = STD_STICK_SSM1;
                FGI_VALIABLE.process = START;
                FGI_Next_Process(FGI_STD_STICK_READ_START);
            }
            else{
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_SSM2_DATA_READ (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > SSM2 표준 스틱 측정중..\r\n");
                std_stick_number = STD_STICK_SSM2;
                FGI_VALIABLE.process = START;
                FGI_Next_Process(FGI_STD_STICK_READ_START);
            }
            else{
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_SSM3_DATA_READ (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > SSM3 표준 스틱 측정중..\r\n");
                std_stick_number = STD_STICK_SSM3;
                FGI_VALIABLE.process = START;
                FGI_Next_Process(FGI_STD_STICK_READ_START);
            }
            else{
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_SSM4_DATA_READ (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > SSM4 표준 스틱 측정중..\r\n");
                std_stick_number = STD_STICK_SSM4;
                FGI_VALIABLE.process = START;
                FGI_Next_Process(FGI_STD_STICK_READ_START);
            }
            else{
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_SSM5_DATA_READ (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > SSM5 표준 스틱 측정중..\r\n");
                std_stick_number = STD_STICK_SSM5;
                FGI_VALIABLE.process = START;
                FGI_Next_Process(FGI_STD_STICK_READ_START);
            }
            else{
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_SSN1_DATA_READ (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > SSN1 표준 스틱 측정중..\r\n");
                std_stick_number = STD_STICK_SSN1;
                FGI_VALIABLE.process = START;
                FGI_Next_Process(FGI_STD_STICK_READ_START);
            }
            else{
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_CLEAR_MONITOR_FGI (int panel, int control, int event,
                                      void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            DeleteTextBoxLines (tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, 0, -1);
            break;
    }
    return 0;
}

int CVICALLBACK BT_CONNECT_SETTINGS (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            Connect_Settings_Popup();
            Serial_Port_Find(subPanel);
            if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_CLOSED){
				SetCtrlVal(subPanel, SUB_PANEL_sBT_SERIAL_PORT_OPEN, 1);
			}
			else if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED)
            {
				SetCtrlVal(subPanel, SUB_PANEL_sBT_SERIAL_PORT_OPEN, 0);
                
                if(BLE_FLAG.connect_status == BLE_CONNECTED) //?????? ??? ???? ???? ???????, ???? ????? ??? ?????? ???????
                {
                    SetCtrlVal(subPanel, SUB_PANEL_sINPUT_BLE_MAC_ADDR, ble_mac_id);
                    SetCtrlVal(subPanel, SUB_PANEL_MONITOR_CON_SET, "\r\n > Surearly SMART 연결 완료 !\r\n");
                    SetCtrlVal(subPanel, SUB_PANEL_sINDI_FW_VER, fw_ver);
		            SetCtrlVal(subPanel, SUB_PANEL_sINDI_MCU_UID, dev_uid);
                    SetCtrlVal(subPanel, SUB_PANEL_sINDI_BLE_NAME, ble_name);
                    if(!strcmp(ble_name_check_string, ble_name)) //Name 설정 정상
                    {
                        SetCtrlAttribute (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, ATTR_ON_COLOR, VAL_GREEN);
                        SetCtrlVal (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, 1);
                    }
                    else
                    {
                        SetCtrlAttribute (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, ATTR_ON_COLOR, VAL_RED);
                        SetCtrlVal (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, 1);
                    }
                }
                else
                {
                    SetCtrlVal (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, 0); //Device의 BLE name 설정 확인 결과 표시 LED 초기화
                }
			}
            break;
    }
    return 0;
}


int CVICALLBACK BT_CLEAR_MON (int panel, int control, int event,
                              void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            DeleteTextBoxLines (tabPanel_PI, TABPANEL_LOG_MONITOR_PI, 0, -1);
            break;
    }
    return 0;
}



int CVICALLBACK BT_RESET_ITEM_FGI (int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            FGI_Clear_Items(tabPanel_FGI);
            break;
    }
    return 0;
}

int CVICALLBACK BT_SAVE_REPORT_FGI (int panel, int control, int event,
                                    void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(!FGI_Checklist_Items_Inspection(tabPanel_FGI))
            {
                int ret;
    			int save_check = 0;
    			char Buff[260]={0};
                char save_file_name[200]= {0};
                char dev_serial_num[100] = {0};
                char ble_mac_addr[100] = {0};
                
                GetCtrlVal (tabPanel_FGI, TABPANEL_2_INPUT_BLE_MAC_ADDR, ble_mac_addr); //BLE MAC address load
                GetCtrlVal (tabPanel_FGI, TABPANEL_2_INPUT_DEV_SERIAL_NUM, dev_serial_num); //Device serial number load
                
                sprintf(save_file_name,"Test_Report-%s-%s.csv", dev_serial_num, ble_mac_addr); //File name: Test_Report-Serial_number-MAC_address
    			ret = FileSelectPopup ("", save_file_name, "*.csv", "Save Data File", VAL_SAVE_BUTTON, 0, 0, 1, 0, Buff);
    			if(ret > 0)
    			{
    				save_check = FGI_Save_Test_Report(tabPanel_FGI, Buff);
    				if(save_check > 0){
    					SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > Test report 저장 완료 !\r\n");	
    				}
    				else{
    					SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > Test report 저장 실패.\r\n");	
    				}
    			}
            }
            else{
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, "\r\n > 오류: 검사 항목 선택이 누락되었습니다.\r\n");
                SetCtrlVal(tabPanel_FGI, TABPANEL_2_LOG_MONITOR_FGI, " > 적합/부적합 checklist를 확인해주십시오.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK INS_APPRO_CHKBOX1 (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX1, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX1, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_INAPPRO_CHKBOX1 (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX1, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX1, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_APPRO_CHKBOX2 (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX2, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX2, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_INAPPRO_CHKBOX2 (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX2, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX2, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_APPRO_CHKBOX3 (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX3, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX3, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_INAPPRO_CHKBOX3 (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX3, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX3, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_APPRO_CHKBOX4 (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX4, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX4, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_INAPPRO_CHKBOX4 (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX4, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX4, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_APPRO_CHKBOX5 (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX5, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX5, 0);
            }
            else{}
            break;
    }
    return 0;
}

int CVICALLBACK INS_INAPPRO_CHKBOX5 (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int check_status = 0;
            GetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_INAPPRO_CHKBOX5, &check_status);

            if(check_status){
            	SetCtrlVal (tabPanel_FGI, TABPANEL_2_INS_APPRO_CHKBOX5, 0);
            }
            else{}
            break;
    }
    return 0;
}

/* ------------------------------------------------ Panel 4 Read PTR --------------------------------------------------------------*/

int CVICALLBACK BT_CLEAR_MON_RAW (int panel, int control, int event,
                              void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            DeleteTextBoxLines (panel, TABPANEL_4_LOG_MONITOR_RAW, 0, -1);
            break;
    }
    return 0;
}

/*
	VAL_MARK_ON  : [중단]
	VAL_MARK_OFF : [PTR 데이터 읽기]
*/
int  CVICALLBACK BT_READ_PTR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                if(auto_setting_mode_state == END_AUTO_SETTING_MODE)
                {
                    auto_setting_mode_state = START_AUTO_SETTING_MODE;
    			    RP_VARIABLE.function_number = START;
    			    RP_VARIABLE.process = START;
                    SetCtrlVal(panel,TABPANEL_4_BT_READ_PTR, VAL_MARK_ON);
                }
                else if(auto_setting_mode_state == START_AUTO_SETTING_MODE)
                {
                    auto_setting_mode_state = END_AUTO_SETTING_MODE;
                    //RP_Variable_Init();
                    //SetCtrlVal(panel, TABPANEL_4_LOG_MONITOR_RAW, "\r\n > -------- 종료 -------\r\n");
                    RP_VARIABLE.one_time_process = RESET;
					RP_VARIABLE.function_number = END;
					// SetCtrlVal(panel,TABPANEL_4_BT_READ_PTR, VAL_MARK_OFF);
                }
            }
            else{
                SetCtrlVal(panel, TABPANEL_4_LOG_MONITOR_RAW, "\r\n > 오류: 블루투스 연결 후 다시 시도해주세요.\r\n");
                SetCtrlVal(panel,TABPANEL_4_BT_READ_PTR, VAL_MARK_OFF);
            }
			break;
	}
	
	return 0;
}

int  CVICALLBACK BT_READ_CAL(int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	return 0;
}
