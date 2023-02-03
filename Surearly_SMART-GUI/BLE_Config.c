#include "main.h" 


BLUETOOTH_FLAG BLE_FLAG = {0,};
BLUETOOTH_RX BLE_RX = {0,};

int ble_rx_index = 0;
char ble_mac_id[BLE_MAC_BUF_SIZE] = {0};
char ble_name[BLE_NAME_BUF_SIZE] = {0}; //기기로부터 읽어온 BLE name string
char ble_name_check_string[BLE_NAME_BUF_SIZE] = {0};
char ble_connection_retry = 0;


static int String_To_Decimal(char *str, unsigned char len);


void BLE_Scan_Start()
{
	if((BLE_FLAG.ble_scan_state == BLE_SCAN_END) && (BLE_FLAG.connect_status == BLE_DISCONNECTED))
	{
		Serial_Write_String("AT+STARTSCAN\r"); //BLE master dongle에 scan 시작 명령 전송
		BLE_FLAG.ble_scan_state = BLE_SCAN_START; //5초간 scan 시작
	}
}

void BLE_Scan_Process(int panel)
{ //BLE_FLAG.ble_scan_state == BLE_SCAN_START일 경우 1 msec timer에서 BLE_RX.scan_time 값 증가 -> 기기 검색 완료 또는 5초 시간 초과 시 scan 중지
	if((BLE_FLAG.ble_scan_state == BLE_SCAN_START) && (BLE_RX.scan_time >= BLE_SCANNING_TIME))
	{
		Serial_Write_String("AT+STOPSCAN\r");
		SetCtrlVal(panel,SUB_PANEL_sBT_BLE_SCAN, 0);
		BLE_FLAG.ble_scan_state = BLE_SCAN_END;
		BLE_RX.scan_time = 0;
	}
}

void BLE_Connection_Process()
{
	int flagProcessExit = RESET;
	char ble_connection_cmd[24] = {0};
	char get_ble_mac_id[BLE_MAC_BUF_SIZE] = {0};

	BLE_FLAG.response_wait_time++;

	if(BLE_FLAG.connect_status == BLE_CONNECTED){ // BLE Connection succesed 
		flagProcessExit = SET;
	}
	else
	{
		if(BLE_FLAG.response_wait_time > 15)	// 15 sec
		{
			SetCtrlVal(subPanel, SUB_PANEL_MONITOR_CON_SET, " > Surearly SMART Pro 연결 실패, 다시 시도해주세요.\n");
			flagProcessExit = SET;
		}
		
		if(ble_connection_retry == 0)
		{
			GetCtrlVal(subPanel, SUB_PANEL_sINPUT_BLE_MAC_ADDR, get_ble_mac_id);
			sprintf(ble_connection_cmd, "AT+CONMAC=%s\r", get_ble_mac_id);
			Serial_Write_String(ble_connection_cmd);
			BLE_FLAG.at_response_msg = AT_NONE;
			ble_connection_retry++;
		}
		else
		{
			if(ble_connection_retry < BLE_CONNECT_RETRY_NUM)
			{
				switch(BLE_FLAG.at_response_msg){
				case AT_PASSKEY:
					Serial_Write_String("AT+ACCEPT\r");
					BLE_FLAG.response_wait_time = 0;
					BLE_FLAG.at_response_msg = AT_NONE;
					break;
				case AT_DISCONNECTED:
				case AT_ERROR:
					if(BLE_FLAG.response_wait_time >= 2)	// resend
					{
						GetCtrlVal(subPanel, SUB_PANEL_sINPUT_BLE_MAC_ADDR, get_ble_mac_id);
						sprintf(ble_connection_cmd, "AT+CONMAC=%s\r", get_ble_mac_id);
						Serial_Write_String(ble_connection_cmd);
						ble_connection_retry++;
						BLE_FLAG.response_wait_time = 0;
						BLE_FLAG.at_response_msg = AT_NONE;
					}
					break;
				case AT_FAIL:
					SetCtrlVal(subPanel, SUB_PANEL_MONITOR_CON_SET, " > Surearly SMART Pro 연결 실패, 기기 상태를 확인해주세요.\n");
					flagProcessExit = SET;
					break;
				default:
					break;
				}
			}
			else // BLE Connection failed
			{
				SetCtrlVal(subPanel, SUB_PANEL_MONITOR_CON_SET, " > Surearly SMART Pro 연결 실패, 다시 시도해주세요.\n");
				flagProcessExit = SET;
			}
		}
	}
	
	if(flagProcessExit == SET){
	    BLE_FLAG.connect_tact_time_flag = BLE_CONNECT_TACT_TIME_END;
	    BLE_FLAG.connect_tact_time = 0;
	    ble_connection_retry = 0;
		BLE_FLAG.response_wait_time = 0;
		BLE_FLAG.at_response_msg = AT_NONE;
	}
}

void BLE_Rx_Dequeue(int panel)
{
    if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED)
	{
        int serial_rx_queue_len = GetInQLen(SERIAL_CONFIG.port_number);
        
        if((serial_rx_queue_len > 0) && (serial_rx_queue_len <= 5000))
        {
            if((!BLE_RX.ok) && (!BLE_RX.complete))
            {
                int buf_idx = 0;
                while(buf_idx < serial_rx_queue_len)
                {
                    BLE_RX.data[ble_rx_index] = (char)ComRdByte(SERIAL_CONFIG.port_number);
                    
                    if(BLE_RX.mode_sel == BLE_CMD_MODE) //BLE AT command handle
                    {
                        if(BLE_RX.data[0] == BLE_CMD_MODE_START_BYTE)
                        {
                    		if(BLE_RX.data[ble_rx_index] == BLE_CMD_MODE_TERM_BYTE) //AT command 수신 완료
                    		{
                                ble_rx_index = 0;
                    			BLE_RX.ok = 1;
                    			BLE_RX.tact_time_flag = 0;
                    			BLE_RX.tact_time = 0;
                                break;
                    		}
                    		else
                    			ble_rx_index++;
                        }
                        else
                        {
                            ble_rx_index = 0;
                            memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
                        }
                    }
                    else if(BLE_RX.mode_sel == API_CMD_MODE) //API command handle
                    {
                        if(BLE_RX.data[0] == API_CMD_MODE_START_BYTE)
                        {
                            if(BLE_RX.data[ble_rx_index] == API_CMD_MODE_TERM_BYTE)
                            {
                                ble_rx_index = 0;
                                BLE_RX.complete = 1; //Call "API_Rx_Command_Process" function
                                BLE_RX.tact_time_flag = 0;
                                BLE_RX.tact_time = 0;
                                break;
                            }
                            else{
                                ble_rx_index++;
                            }
                        }
                        else if(BLE_RX.data[0] == BLE_CMD_MODE_START_BYTE)
                        {
                            if(BLE_RX.data[ble_rx_index] == BLE_CMD_MODE_TERM_BYTE)
                            {
                                ble_rx_index = 0;
                                BLE_RX.ok = 1; //Call "BLE_AT_Command_Process" function
                                BLE_RX.tact_time_flag = 0;
                                BLE_RX.tact_time = 0;
                                break;
                            }
                            else{
                                ble_rx_index++;
                            }
                        }
                        else
                        {
                            ble_rx_index = 0;
                            memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
                        }             
                    }
                    else if(BLE_RX.mode_sel == FW_UPD_CMD_MODE)
                    {
                        if((BLE_RX.data[RESP_PACKET_HEADER_START_INDEX] == 0x3A) && (BLE_RX.data[RESP_PACKET_HEADER_START_INDEX+1] == 0x02) && (BLE_RX.data[RESP_PACKET_HEADER_START_INDEX+2] == 0x7B)) //Check Packet Header
                        {
                            //Header 수신 완료
                            if((BLE_RX.data[RESP_PACKET_TAIL_START_INDEX] == 0x7D) && (BLE_RX.data[RESP_PACKET_TAIL_START_INDEX+1] == 0x03) && (BLE_RX.data[RESP_PACKET_TAIL_START_INDEX+2] == 0x0D)) //Check Packet Tail
                            {
                              BLE_RX.tact_time_flag = END;
                              BLE_RX.tact_time = 0;
                              rx_msg_check = 0;
                              //Header + Tail 수신 완료
                              if(Received_Packet_Verify((unsigned char*)BLE_RX.data) == PACKET_CHECKSUM_OK)
                              {
								  //Checksum 정상
								if(BLE_RX.data[RESP_PACKET_DATA_INDEX] == RESP_DATA_ACK){
									rx_msg_check = PACKET_DATA_ACK;
								}
								else{
									rx_msg_check = PACKET_DATA_NACK;
								}
                              }
                              else{//Checksum 오류
                                rx_msg_check = PACKET_DATA_NACK;
                              }
                              ble_rx_index = 0;
                              memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
                            }
                            else{
                              ble_rx_index++;
                            }
                        }
                        else
                        {
                            if(ble_rx_index < PACKET_HEADER_LEN){
                              ble_rx_index++;
                            }
                            else{//Header 수신 실패
                              BLE_RX.tact_time_flag = END;
                              BLE_RX.tact_time = 0;
                              ble_rx_index = 0;
                              memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
                            }
                        }
                    }
                    else
                    {
                        ble_rx_index = 0;
                        memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
                    }

                    if(ble_rx_index){BLE_RX.tact_time_flag = START;} //문자가 수신 되었다면 RX tact time count 시작
                    
                    if((BLE_RX.tact_time >= BLE_RX_TACT_TIME)||(ble_rx_index >= (BLE_BUFFER_SIZE - 1))){ //Tact time, 수신 버퍼 초과 시 dequeue 중지
                        break;
                    }
                    
                    buf_idx++;
                }
            }
        }
        else if(serial_rx_queue_len > 5000) //Serial port 연결 중 BLE master module 강제 'Off'할 경우 노이즈 발생으로 serial rx buffer에 data 다량으로 수신되는 문제
        {
            if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED)
            {
                SetCtrlVal(subPanel, SUB_PANEL_MONITOR_CON_SET, "\n > 오류: BLE master module이 강제 종료되었습니다.\n");
                SERIAL_CONFIG.serial_connection_check = SERIAL_PORT_CLOSED;
                BLE_FLAG.connect_status = BLE_DISCONNECTED;
                BLE_RX.mode_sel = BLE_CMD_MODE;
                FlushInQ(SERIAL_CONFIG.port_number);
                CloseCom(SERIAL_CONFIG.port_number);
                SetCtrlVal(subPanel, SUB_PANEL_sBT_SERIAL_PORT_OPEN, 0);
                Connection_Settings_Init(subPanel);
                if(auto_setting_mode_state == START_AUTO_SETTING_MODE) //"공정 검사 - 기기 자동 설정 시작" 실행 중이라면 강제 중지
                {
                    SetCtrlVal(panel,TABPANEL_BT_AUTO_SETTINGS, 0);
                    auto_setting_mode_state = END_AUTO_SETTING_MODE;
                    PI_Variable_Init();
                }
            }
        }
        
        //BLE_RX_TACT_TIME 시간 내 명령이 정상 수신되지 않았다면 Tact time, buffer 초기화
        if((BLE_RX.tact_time >= BLE_RX_TACT_TIME)||(ble_rx_index >= (BLE_BUFFER_SIZE - 1)))
        {
          BLE_RX.tact_time_flag = 0;
          BLE_RX.tact_time = 0;
          ble_rx_index = 0;
          memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
        }
        //
    }
}



int ble_rssi = 60000;

void BLE_AT_Command_Process(int panel)
{
    if(BLE_RX.ok)
    {
        if((strstr(BLE_RX.data, "SurearlySMART-") != NULL) || (strstr(BLE_RX.data, "SuSm") != NULL)|| (strstr(BLE_RX.data, "Sugentech") != NULL))
        {
            //Get rssi info.
            char get_rssi_sign = BLE_RX.data[14];
            char get_rssi_data_buf[BLE_RSSI_BUF_SIZE] = {0};
            
            if(get_rssi_sign == '-') //수신된 RSSI 값의 부호 확인
            {
                memcpy(get_rssi_data_buf, &BLE_RX.data[15], BLE_RSSI_BUF_SIZE - 1);
                ble_rssi = String_To_Decimal(get_rssi_data_buf, BLE_RSSI_BUF_SIZE - 1);
                /*
                //Get distance
                char ble_rssi_str_buf[50] = {0};
                double environmental_factor = 2.0;
                double cal = (double)((-61.0 + (double)ble_rssi) / (10.0 * environmental_factor));
                double ble_dev_distance = pow(10.0, cal);
                sprintf(ble_rssi_str_buf, " > SMART Pro 기기와의 거리: %0.1f m\n", ble_dev_distance);
                SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, ble_rssi_str_buf);
                //
                */
                if(ble_rssi <= BLE_MAXIMUM_RSSI)
                {
                    //Get MAC address
                	memcpy(ble_mac_id, &BLE_RX.data[1], BLE_MAC_BUF_SIZE - 1);
                    SetCtrlVal(panel, SUB_PANEL_sINPUT_BLE_MAC_ADDR, ble_mac_id);
                    
                    //Get BLE name string
                    for(char i = 0; i < BLE_NAME_LEN; i++)
                    {
                        char ble_name_char = BLE_RX.data[18 + i];
                        if(ble_name_char == '\r'){break;}
                        else{
                            ble_name[i] = ble_name_char;
                        }
                    }
                    
                    //검색 완료 BLE 장치 정보 출력
                    SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\n");
                    SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, BLE_RX.data);
                	SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\n");
                    
                    //BLE 장치 스캔 중지
                    BLE_FLAG.ble_scan_state = BLE_SCAN_END;
                    BLE_RX.scan_time = 0;
                    Serial_Write_String("AT+STOPSCAN\r");
                	SetCtrlVal(panel,SUB_PANEL_sBT_BLE_SCAN, 0);
                    SetCtrlVal(subPanel, SUB_PANEL_MONITOR_CON_SET, " > 기기 검색 완료.\n");
                    Sleep(1000);
                    //Serial 수신 버퍼 비움
                    FlushInQ(SERIAL_CONFIG.port_number);
                }
                else{}
            }
            else{}
        }
        else if(strstr(BLE_RX.data, "+CONNECTED") != NULL)
        {
        	BLE_FLAG.connect_status = BLE_CONNECTED;
            SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n > Surearly SMART 연결 완료 !\r\n");
            BLE_RX.mode_sel = API_CMD_MODE;
            Sleep(1000);
            
            SetCtrlVal(tabPanel_FGI, TABPANEL_2_INPUT_BLE_MAC_ADDR, ble_mac_id); //완제품 검사 메인 화면의 블루투스 MAC 주소 항목 update
            /* Check BLE name */
            //기기의 BLE name 설정이 정상인지 확인
            
            memset(ble_name_check_string, 0, BLE_NAME_BUF_SIZE);
            sprintf(ble_name_check_string, "SurearlySMART-%s", &ble_mac_id[6]);
            
            BLE_FLAG.ble_name_setup_complete_check = BLE_NAME_SETUP_CHECK_NONE;
            
            //기기로부터 읽어온 BLE name (ble_name)이 정상적으로 설정되어 있는지 확인 진행
            if(!strcmp(ble_name_check_string, ble_name)) //Name 설정 정상
            {
                SetCtrlAttribute (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, ATTR_ON_COLOR, VAL_GREEN);
                SetCtrlVal (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, 1);
                BLE_FLAG.ble_name_setup_complete_check = BLE_NAME_SETUP_CHECK_SUCCESS;
            }
            else //Name 설정 오류
            {
                SetCtrlAttribute (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, ATTR_ON_COLOR, VAL_RED);
                SetCtrlVal (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, 1);
                
                SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\n > 오류: 기기의 블루투스 이름 설정에 문제가 있습니다.\n");
                SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\n > 블루투스 이름 설정을 수동으로 진행해주세요.\n");
                BLE_FLAG.ble_name_setup_complete_check = BLE_NAME_SETUP_CHECK_FAIL;
            }
            /* ********************* */
            
            if(fw_update_mode_process == FW_UPDATE_END){ //공정, 완제품, QA test 모드 일때
                production_mode_initializing = START;
            }
            else{}//기기가 Firmware update 모드의 IAP 진입 상태라면, 기기 정보 조회하지 않음 (API 명령 송수신 가능 모드 아님)
        }
        else if(strstr(BLE_RX.data, "+DISCONNECTED") != NULL)
        {
            BLE_FLAG.connect_status = BLE_DISCONNECTED;
			BLE_FLAG.at_response_msg = AT_DISCONNECTED;
			
			if(BLE_FLAG.connect_tact_time_flag != BLE_CONNECT_TACT_TIME_START){
	            SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n > Surearly SMART 연결 해제.\r\n");
	            BLE_RX.mode_sel = BLE_CMD_MODE;
	            
	            SetCtrlVal (subPanel, SUB_PANEL_sINPUT_BLE_MAC_ADDR, 0); //연결 및 설정 화면의 MAC 주소 항목 초기화
	            SetCtrlVal (subPanel, SUB_PANEL_sINDI_FW_VER, 0); //연결 및 설정 화면의 F/W 버전 항목 초기화
	            SetCtrlVal (subPanel, SUB_PANEL_sINDI_MCU_UID, 0); //연결 및 설정 화면의 MCU UID 항목 초기화
	            SetCtrlVal (subPanel, SUB_PANEL_sINDI_BLE_NAME, 0); //연결 및 설정 화면의 BLE 이름 항목 초기화
	            SetCtrlVal (subPanel, SUB_PANEL_DEVICE_CONNECT_LED, 0); //Device의 BLE name 설정 확인 결과 표시 LED 초기화
	            
	            SetCtrlVal(tabPanel_FGI, TABPANEL_2_INPUT_BLE_MAC_ADDR, 0); //완제품 검사 메인 화면의 블루투스 MAC 주소 항목 초기화
	            SetCtrlVal(tabPanel_FGI, TABPANEL_2_INPUT_DEV_SERIAL_NUM, DEV_SERIAL_INIT_STR); //완제품 검사 메인 화면의 기기 시리얼 번호 항목 초기화
			}
        }
        else if(!strcmp(BLE_RX.data, "+OK\r"))
        {	
			BLE_FLAG.at_response_msg = AT_OK;
        }
		else if(strstr(BLE_RX.data, "+PASSKEY") != NULL)
		{
			BLE_FLAG.at_response_msg = AT_PASSKEY;
		}
        else if(!strcmp(BLE_RX.data, "+ERROR\r"))
        {
			BLE_FLAG.at_response_msg = AT_ERROR;
        }
        else if(!strcmp(BLE_RX.data, "+FAIL\r"))
        {
			BLE_FLAG.at_response_msg = AT_FAIL;
			SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\n오류: 기기 전원, 블루투스 상태를 확인하십시오.\n");
			
			Connection_Settings_Init(subPanel);
        }
        
        memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
        BLE_RX.ok = 0;
    }
}

int String_To_Decimal(char *str, unsigned char len) //String -> Decimal 변환
{
  int dec = 0;
  int i = 0;

  for(i = 0; i < len; i++)
  {
    if((0x30 <= str[i])&&(str[i] <= 0x39)){
      dec += (str[i]-0x30) * (int)RoundRealToNearestInteger(pow(10.0, (double)(len-i-1)));
    }
  }

  return dec;
}