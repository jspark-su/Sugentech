#include "main.h"

BLUETOOTH_FLAG BLE_FLAG = {0,};
BLUETOOTH_RX BLE_RX = {0,};
BLUETOOTH_TX BLE_TX = {0,};

uint16_t ble_rx_index = 0;
char ble_mac_id[BLE_MAC_ADDR_LEN] = {0}; //BLE module로부터 읽어온 MAC address
char ble_name[BLE_NAME_LEN] = {0}; //BLE module로부터 읽어온 name 정보
uint8_t ble_uart_block = 0;

void BLE_Init()
{
  #ifdef __BLE_BOT_NLE521
   BLE_Bot_Uart_Init();
  #endif /*__BLE_BOT_NLE521*/
  #ifdef __BLE_RF_BM_4044B4
   ble_rf_uart_init();
  #endif /*__BLE_RF_BM_4044B4*/
}

void BLE_Deinit()
{
  #ifdef __BLE_BOT_NLE521
   BLE_Bot_Uart_Deinit();
  #endif /*__BLE_BOT_NLE521*/
  #ifdef __BLE_RF_BM_4044B4
   ble_rf_uart_deinit();
  #endif /*__BLE_RF_BM_4044B4*/
}

void BLE_Start()
{
  #ifdef __BLE_BOT_NLE521
  BLE_Bot_Wakeup();

  //BLE 연결시 +CONNECTED[MAC Address] 메시지 제거 위해
  memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
  ble_rx_index = 0;
  //
  
  uint8_t ble_name_setup_complete_check = EEPROM_Byte_Read(BLE_NAME_SETUP_COMPLETE_ADDR);
  
  if(ble_name_setup_complete_check == BLE_SETUP_COMPLETE) //이전 BLE name 설정 진행 완료 (추가 진행 필요 없음)
  {
    //"AT+INFO?" 명령을 이용하여 BLE name, MAC address load
    char at_info_retry = 0;
    while(at_info_retry < 5) //"AT+INFO?" 명령 5회 재시도 (응답 수신 불가)
    {
      if(Send_AT_Command("AT+INFO?\r") == AT_CMD_RESP_STRING_OK){break;}
      else{at_info_retry++;}
    }
    if(at_info_retry >= 5) //Error "AT+INFO?\r" (LCD icon: 'EJECT' + 'BLE' + 'SQUARE_1' + 'BOOK')
    {
      LCD_Display(BLE, ICON_ON); LCD_Display(BOOK, ICON_ON); LCD_Display(EJECT, ICON_ON); LCD_Display(SQUARE_1, ICON_ON);
      delay_ms(5000);
      LCD_Display(BLE, ICON_OFF); LCD_Display(BOOK, ICON_OFF); LCD_Display(EJECT, ICON_OFF); LCD_Display(SQUARE_1, ICON_OFF);
    }
    else{} //응답 수신 성공
  }
  else //BLE name 설정 진행 필요
  {
    char ble_name_auto_setup_rslt = BLE_Name_Auto_Setting();
    if(ble_name_auto_setup_rslt == BLE_NAME_SETUP_COMPLETE){ //BLE name 설정 성공
      EEPROM_Byte_Write(BLE_NAME_SETUP_COMPLETE_ADDR, BLE_SETUP_COMPLETE);
    }
    else //BLE name 설정 실패
    {
      //Error "AT+INFO?\r" (LCD icon: 'EJECT' + 'BLE' + 'SQUARE_2' + 'BOOK')
      //Error "AT+MANU=xxxx\r" (LCD icon: 'EJECT' + 'BLE' + 'SQUARE_3' + 'BOOK')
      EEPROM_Byte_Write(BLE_NAME_SETUP_COMPLETE_ADDR, BLE_SETUP_NONE);
      LCD_Display(BLE, ICON_ON); LCD_Display(BOOK, ICON_ON); LCD_Display(EJECT, ICON_ON);
      if(ble_name_auto_setup_rslt == BLE_NAME_SETUP_FAIL_AT_INFO){LCD_Display(SQUARE_2, ICON_ON);}
      else if(ble_name_auto_setup_rslt == BLE_NAME_SETUP_FAIL_AT_MANUF){LCD_Display(SQUARE_3, ICON_ON);}
      delay_ms(5000);
      LCD_Display(BLE, ICON_OFF); LCD_Display(BOOK, ICON_OFF); LCD_Display(EJECT, ICON_OFF);
      if(ble_name_auto_setup_rslt == BLE_NAME_SETUP_FAIL_AT_INFO){LCD_Display(SQUARE_2, ICON_OFF);}
      else if(ble_name_auto_setup_rslt == BLE_NAME_SETUP_FAIL_AT_MANUF){LCD_Display(SQUARE_3, ICON_OFF);}
    }
  }
  
  
   //Send_AT_Command("AT+INFO?\r");
   //Send_AT_Command("AT+MANUF=Surearly\r");
   //Send_AT_Command("AT+TXPWR=7\r");
   //Send_AT_Command("AT+UART=38400\r");
   //Send_AT_Command("AT+VER?\r");
   //Send_AT_Command("AT+FLOWCONTROL?\r");
   //Send_AT_Command("AT+INTPULLDOWN?\r");
  #endif /*__BLE_BOT_NLE521*/
    
  #ifdef __BLE_RF_BM_4044B4
    ble_rf_start();
    //send_ttm_command("TTM:CIT-20ms");
    //send_ttm_command("TTM:ADP-(200)");
    
    send_ttm_command("TTM:TPL-(2)");    
    send_ttm_command("TTM:MAC-?");
    send_ttm_command("TTM:NAM-?");
    
  #endif /*__BLE_RF_BM_4044B4*/
}

void BLE_Wakeup()
{
  BLE_Bot_Wakeup();
}

void BLE_Sleep()
{
  #ifdef __BLE_BOT_NLE521
    BLE_Bot_Sleep();
  #endif /*__BLE_BOT_NLE521*/
    
  #ifdef __BLE_RF_BM_4044B4
    ble_rf_sleep();
  #endif /*__BLE_RF_BM_4044B4*/
}

char BLE_Name_Setting(char* ble_name_str)
{
  #ifdef __BLE_BOT_NLE521
    return BLE_Bot_Name_Setting(ble_name_str);
  #endif /*__BLE_BOT_NLE521*/
    
  #ifdef __BLE_RF_BM_4044B4
  #endif /*__BLE_RF_BM_4044B4*/
}

void BLE_Connection_Check()
{
  //BLE Connection status
  if(GPIO_ReadInputDataBit(BLE_C_STATUS_PORT, BLE_C_STATUS_PIN))
  {
    #ifdef __BLE_BOT_NLE521
      BLE_FLAG.connect_status = BLE_CONNECTED;
      if(BLE_FLAG.ble_icon_con_en == RESET)
      {
        BLE_FLAG.ble_icon_con_en = SET;
        BLE_FLAG.ble_icon_disc_en = RESET;
        BLE_FLAG.ble_icon_update = SET;
      }    
    #endif /*__BLE_BOT_NLE521*/
      
    #ifdef __BLE_RF_BM_4044B4
      BLE_FLAG.connect_status = BLE_DISCONNECT;
      if(BLE_FLAG.ble_icon_disc_en == RESET)
      {
        BLE_FLAG.ble_icon_disc_en = SET;
        BLE_FLAG.ble_icon_con_en = RESET;
        BLE_FLAG.ble_icon_update = SET;
      }
    #endif /*__BLE_RF_BM_4044B4*/
  }
  else
  {
    #ifdef __BLE_BOT_NLE521
      BLE_FLAG.connect_status = BLE_DISCONNECT;
      if(BLE_FLAG.ble_icon_disc_en == RESET)
      {
        BLE_FLAG.ble_icon_disc_en = SET;
        BLE_FLAG.ble_icon_con_en = RESET;
        BLE_FLAG.ble_icon_update = SET;
      }
    #endif /*__BLE_BOT_NLE521*/
      
    #ifdef __BLE_RF_BM_4044B4
      BLE_FLAG.connect_status = BLE_CONNECTED;
      if(BLE_FLAG.ble_icon_con_en == RESET)
      {
        BLE_FLAG.ble_icon_con_en = SET;
        BLE_FLAG.ble_icon_disc_en = RESET;
        BLE_FLAG.ble_icon_update = SET;
      }    
    #endif /*__BLE_RF_BM_4044B4*/
  }
  //
}

// @Param
//ENABLE: 장치와 연결되어 있다면 연결 종료, ENABLE 상태 4초 이상 유지시
//        BLE 공장 초기화 되므로, 4초 이내 DISABLE 해야함.
//DISABLE: 장치와 연결 허용
void BLE_Disconnect()
{
  #ifdef __BLE_BOT_NLE521
    BLE_Bot_Disconnect();      
  #endif /*__BLE_BOT_NLE521*/
}

void BLE_Rx_Enqueue()
{
  disableInterrupts();
  #ifdef __BLE_BOT_NLE521
    BLE_Bot_Rx_Enqueue();
  #endif /*__BLE_BOT_NLE521*/
    
  #ifdef __BLE_RF_BM_4044B4
    ble_rf_rx_ch();
  #endif /*__BLE_RF_BM_4044B4*/
  enableInterrupts();
}

void BLE_Queue_Process()
{
  #ifdef __BLE_BOT_NLE521
    BLE_Bot_Queue_Process();
  #endif /*__BLE_BOT_NLE521*/
    
  #ifdef __BLE_RF_BM_4044B4
    ble_rf_queue_process();
  #endif /*__BLE_RF_BM_4044B4*/
}

void BLE_Tx_Byte(char ch) 
{
  while(!(BLE_USART->SR & 0x80)){}  // Transmit data register empty
  BLE_USART->DR = ch;
}

void BLE_Tx_String(char* str)
{
  while(*str){ 
    BLE_Tx_Byte(*str++);
  }
}

void BLE_Buffer_Init()
{
  disableInterrupts();
  ble_uart_block = BLE_UART_BLOCK_ENABLE; //UART RX IT 발생 시 BLE_BOT_QUEUE로 data enqueue 방지
//BLE UART 수신 버퍼 초기화
  memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
  #ifdef __BLE_BOT_NLE521
    BLE_Bot_Queue_Init();
  #endif /*__BLE_BOT_NLE521*/
    
  #ifdef __BLE_RF_BM_4044B4
    ble_rf_queue_init();
  #endif /*__BLE_RF_BM_4044B4*/
//  
//BLE UART 송신 버퍼 초기화
  BLE_TX.index = 0;
  BLE_TX.data_cnt = 0;
  BLE_TX.flag = 0;
  memset(BLE_TX.data, 0, sizeof(BLE_TX.data));
//
  enableInterrupts();
}

char BLE_Name_Auto_Setting()
{
  char at_info_retry = 0;
  char ble_name_setup_retry = 0;
  char ble_mac_addr_head[5] = {0};
  char ble_mac_addr_tail[7] = {0};
  char comp_ble_name_str[BLE_NAME_LEN] = {0};
  char ble_name_auto_setup_rslt = 0;

  while(at_info_retry < 5) //"AT+INFO?" 명령 5회 재시도
  {
    if(Send_AT_Command("AT+INFO?\r") == AT_CMD_RESP_STRING_OK)
    {
      u_strncpy(ble_mac_addr_head, ble_mac_id, 5); //수신된 MAC address의 앞 4자리 저장
      u_strncpy(ble_mac_addr_tail, &ble_mac_id[6], 7); //수신된 MAC address의 뒤 6자리 저장

      if(!strcmp(ble_mac_addr_head, "5CF2")) //MAC address, name 정보 수신 완료
      {
        sprintf(comp_ble_name_str,"SurearlySMART-%s", ble_mac_addr_tail);
        if(!strcmp(ble_name, comp_ble_name_str)){ble_name_auto_setup_rslt = BLE_NAME_SETUP_COMPLETE;} //수신된 BLE name과 MAC address와 조합된 name과 같을 시 name 설정 Pass
        else //수신된 BLE name이 MAC address와 조합된 name과 다를 시 name 설정 진행
        {
          while(ble_name_setup_retry < 5) //BLE name 설정 5회 재시도
          {
            char ble_name_setup_resp = BLE_Name_Setting(comp_ble_name_str);
            if(ble_name_setup_resp == BLE_NAME_SETUP_OK){break;}  //BLE name 설정 명령 송수신 완료
            else{ble_name_setup_retry++;} //BLE name 설정 재시도 (Name 설정 진행 오류)
          }

          if(ble_name_setup_retry >= 5){ble_name_auto_setup_rslt = BLE_NAME_SETUP_FAIL_AT_MANUF;}//BLE name (AT+MANUF="ble name") 설정 명령 실패, 오류 
          else
          {
            //설정 정상 적용 검증 시작
            at_info_retry = 0;
            delay_ms(300); //BLE name 설정 후 advertising 재시작 시간 대기
            while(at_info_retry < 5) //"AT+INFO?" 명령으로 name 정상 설정 완료 여부 확인
            {
              if(Send_AT_Command("AT+INFO?\r") == AT_CMD_RESP_STRING_OK)
              {
                if(!strcmp(ble_name, comp_ble_name_str)){break;} //API로 수신된 BLE name이 BLE module에 정상 적용 완료
                else{at_info_retry++;} //API로 수신된 BLE name과 BLE module에 적용된 name과 다름, 재시도
              }
              else{at_info_retry++;} //"AT+INFO?" 명령 재시도 (응답 수신 불가)
            }
            if(at_info_retry >= 5){ble_name_auto_setup_rslt = BLE_NAME_SETUP_FAIL_VERIFY;} //BLE name 설정 검증 실패
            else{ble_name_auto_setup_rslt = BLE_NAME_SETUP_COMPLETE;} //BLE name 설정 검증 완료
          }
        }
        break;
      }
      else{at_info_retry++;} //"AT+INFO?" 명령 재시도 (수신 data 오류)
    }
    else{at_info_retry++;} //"AT+INFO?" 명령 재시도 (응답 수신 불가)
  }

  if(at_info_retry >= 5){ble_name_auto_setup_rslt = BLE_NAME_SETUP_FAIL_AT_INFO;} //"AT+INFO?" 명령 응답 수신 실패, 오류

  return ble_name_auto_setup_rslt;
}