#include "main.h"

#ifdef __BLE_BOT_NLE521

CQueue BLE_BOT_QUEUE;

void BLE_Bot_Uart_Init()
{
  SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);
  
  CLK_PeripheralClockConfig(BLE_CLK_USART, ENABLE);

  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_RX_PIN, DISABLE); //DISABLE 상태 고정, ENABLE 시 MCU USART Rx로 data 수신 불가
  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_TX_PIN, ENABLE);
 
  USART_Init(BLE_USART, BLE_BUADRATE, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
  
  //enableInterrupts();
  
  USART_ITConfig(BLE_USART, USART_IT_RXNE, ENABLE);

  USART_Cmd(BLE_USART, ENABLE);
  
  QueueInit(&BLE_BOT_QUEUE);  

  ble_uart_block = BLE_UART_BLOCK_DISABLE;
}

void BLE_Bot_Uart_Deinit()
{
  //CLK_PeripheralClockConfig(BLE_CLK_USART, DISABLE);
  //GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_RX_PIN, DISABLE);
  //GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_TX_PIN, DISABLE);

  USART_ITConfig(BLE_USART, USART_IT_RXNE, DISABLE);
  USART_DeInit(BLE_USART);
  USART_Cmd(BLE_USART, DISABLE);

  //GPIO_Init(BLE_UART_PORT, BLE_RX_PIN, GPIO_Mode_Out_OD_Low_Slow);
  //GPIO_WriteBit(BLE_UART_PORT, BLE_RX_PIN, RESET);
}

void BLE_Bot_Wakeup() //BLE wakeup (Falling edge)
{
  //BLE Wakeup
  //GPIO_WriteBit(BLE_RTS_PORT, BLE_RTS_PIN, SET); //BLE UART 'On', Low Level(Falling Edge)
  //GPIO_WriteBit(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, SET); //BLE Wakeup, Low Level(Falling Edge)
  //delay_ms(20);
  //GPIO_WriteBit(BLE_RTS_PORT, BLE_RTS_PIN, RESET);
  GPIO_WriteBit(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, RESET);
  //delay_ms(20);
  delay_ms(50); //BLE power on settling time
}

void BLE_Bot_Sleep() //BLE sleep mode (Rising edge)
{
  //GPIO_WriteBit(BLE_RTS_PORT, BLE_RTS_PIN, RESET); //BLE UART 'Off', High Level(Rising Edge)
  //GPIO_WriteBit(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, RESET); //BLE Sleep, High Level(Rising Edge)
  //delay_ms(20);
  //GPIO_WriteBit(BLE_RTS_PORT, BLE_RTS_PIN, SET);
  GPIO_WriteBit(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, SET);
  delay_ms(20);

  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_RX_PIN, DISABLE);
  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_TX_PIN, DISABLE);

//BLE_TX_PIN (MCU 기준, BLE module 기준: BLE_RX) Open-drain 'Low' 설정 (floating 상태 방지)
  GPIO_Init(BLE_UART_PORT, BLE_TX_PIN, GPIO_Mode_Out_OD_Low_Slow);
  GPIO_WriteBit(BLE_UART_PORT, BLE_TX_PIN, RESET);
//

//BLE_RX_PIN (MCU 기준, BLE module 기준: BLE_TX) Input floating 설정 
  GPIO_Init(BLE_UART_PORT, BLE_RX_PIN, GPIO_Mode_In_FL_No_IT); //BLE module에서 sleep sequence 과정에서 해당 pin 설정 완료 후 open-raing 설정 진행
  delay_ms(100); //BLE Host 장치 연결 상태 기준 BLE module의 BLE_RX_PIN (MCU 기준) 설정 완료까지 약 95 msec 소요로 대기 시간 100 msec 추가
//
  
//BLE_TX_PIN (MCU 기준, BLE module 기준: BLE_RX) Open-drain 'Low' 설정 (floating 상태 방지)  
  GPIO_Init(BLE_UART_PORT, BLE_RX_PIN, GPIO_Mode_Out_OD_Low_Slow); 
  GPIO_WriteBit(BLE_UART_PORT, BLE_RX_PIN, RESET);
//  
}

// @Param
//ENABLE: 장치와 연결되어 있다면 연결 종료, ENABLE 상태 4초 이상 유지시
//        BLE 공장 초기화 되므로, 4초 이내 DISABLE 해야함.
//DISABLE: 장치와 연결 허용

void BLE_Bot_Disconnect()
{
  uint16_t time_out_cnt = 0;
  if(GPIO_ReadInputDataBit(BLE_C_STATUS_PORT, BLE_C_STATUS_PIN)) //App과 연결 상태라면 연결 해제
  {
    GPIO_WriteBit(DISCO_FACT_BLE_PORT, DISCO_FACT_BLE_PIN, RESET);
    delay_ms(1);
    GPIO_WriteBit(DISCO_FACT_BLE_PORT, DISCO_FACT_BLE_PIN, SET);

    while((GPIO_ReadInputDataBit(BLE_C_STATUS_PORT, BLE_C_STATUS_PIN)) && (time_out_cnt < 200) )
    {
       //BLE_CONNECT_STAT pin 'Low' 상태로 연결 해제 정상 완료 확인
       delay_ms(10);
       time_out_cnt += 1;
    }
    GPIO_WriteBit(DISCO_FACT_BLE_PORT, DISCO_FACT_BLE_PIN, RESET);
  }
}

char BLE_Bot_Name_Setting(char* ble_name_str)
{
  char ble_name_setup_cmd[BLE_NAME_CMD_BUF_SIZE] = {0};

  if(strlen(ble_name_str) >= BLE_NAME_LEN){ //BLE name length error
    return BLE_NAME_SETUP_LENGHTH_ERROR;
  }
  else
  {
    sprintf(ble_name_setup_cmd,"AT+MANUF=%s\r", ble_name_str);
    char result = Send_AT_Command(ble_name_setup_cmd);
    if(result == AT_CMD_RESP_ADVERTISING){return BLE_NAME_SETUP_OK;}
    else if(result == AT_CMD_RESP_ERROR){return BLE_NAME_SETUP_TIMEOUT;}
    else{return BLE_NAME_SETUP_TIMEOUT;}
  }
}

void BLE_Bot_Rx_Enqueue()
{
  //Received data is ready to be read.
  if(USART_GetFlagStatus(BLE_USART, USART_FLAG_RXNE) == SET)
  {
    uint8_t uart_rx_data = BLE_USART->DR;
    if(ble_uart_block == BLE_UART_BLOCK_DISABLE){
      Enqueue(&BLE_BOT_QUEUE, uart_rx_data);
    }
    else{}
    USART_ClearFlag(BLE_USART, USART_FLAG_RXNE);
  }
}

char Send_AT_Command(char* cmd)
{
  int i = 0;
  char resp_result = 0;
  uint16_t TACK_TIME = 0;
  
  BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;
  BLE_Tx_String(cmd);//Send AT command 
  BLE_TX.tact_time_flag = 1; //BLE Tx tact time count 시작
  BLE_TX.tact_time = 0; //Tact time count 초기화
  
  if(!strcmp(cmd, "AT+INTPULLDOWN=OFF\r") || !strcmp(cmd, "AT+INTPULLDOWN=ON\r")) // AT Command별 재전송 시간/횟수 설정
  { // 2200 ms, 총 2회 Command 전송
    TACK_TIME = BLE_PULLDOWN_OFF_TIME;
    BLE_TX.retry = 2;
  }
  else if(!strcmp(cmd, "AT+INTPULLDOWN?\r"))
  {// 200 ms, 총 3회 Command 전송
    TACK_TIME = BLE_TX_TACT_TIME;
    BLE_TX.retry = 3;
  }
  else
  { // 200 ms, 총 5회 Command 전송
    TACK_TIME = BLE_TX_TACT_TIME;
    BLE_TX.retry = 5;
  }

  while(1)
  {
    if((!strcmp(cmd, "AT\r")) || (strstr(cmd, "AT+UART=") != NULL))
    {
      if(BLE_RX.at_cmd_resp_chk == AT_CMD_RESP_OK){break;}
      else{BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;}
    }
    else if(!strcmp(cmd, "AT+INFO?\r"))
    {
      if(BLE_RX.at_cmd_resp_chk == AT_CMD_RESP_STRING_OK)
      {
        char at_info_resp_buf[39] = {0}; //"AT+INFO?"의 응답 내용 ("5C:F2:86:A0:00:01,SurearlySMART-000000") 임시 저장 
        char tmp_ble_mac_id[BLE_MAC_ADDR_BUF_SIZE] = {0};
        char *p_at_info_resp = strstr(BLE_RX.data, "5C:F2");

        if(p_at_info_resp != NULL)
        {
          memset(ble_name, 0, sizeof(ble_name));
          memset(ble_mac_id, 0, sizeof(ble_mac_id));
          u_strncpy(at_info_resp_buf, p_at_info_resp, sizeof(at_info_resp_buf));

          for(i = 0; i < (BLE_MAC_ADDR_BUF_SIZE - 1); i++){ //Get BLE MAC address: "5C:F2:86:42:85:15"
            tmp_ble_mac_id[i] = at_info_resp_buf[i];
          }
          for(i = BLE_MAC_ADDR_BUF_SIZE; i < (BLE_MAC_ADDR_BUF_SIZE + BLE_NAME_LEN -1); i++) //Get BLE name string
          {
            if(at_info_resp_buf[i] == 0x2C){ //수신된  name 길이가 20자 미만일 경우 ',' 전까지 name 저장
              break;
            }
            ble_name[i - BLE_MAC_ADDR_BUF_SIZE] = at_info_resp_buf[i];
          }

          for(i = 0; i < 6; i++) //Convert "5C:F2:86:42:85:15" to "5CF286428515"
          {
            if(i == 0)
            {
              ble_mac_id[0] = tmp_ble_mac_id[0];
              ble_mac_id[1] = tmp_ble_mac_id[1];
            }
            else
            {
              ble_mac_id[2 * i] = tmp_ble_mac_id[3 * i];
              ble_mac_id[(2 * i) + 1] = tmp_ble_mac_id[(3 * i) + 1];
            }
          }
          break;
        }
        else{BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;}//"AT+INFO?"의 수신 data 오류
      }
      else{BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;}//"AT+INFO?" 명령의 응답 수신 실패 또는 "+OK" 응답 수신
    }
    else if(!strcmp(cmd, "AT+INTPULLDOWN?\r"))
    {
      if(BLE_RX.at_cmd_resp_chk == AT_CMD_RESP_OFF) break;
      else if(BLE_RX.at_cmd_resp_chk == AT_CMD_RESP_ON) break;
      else BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;
    }
    else if((!strcmp(cmd, "AT+INTPULLDOWN=OFF\r")) || (strstr(cmd, "AT+MANUF=") != NULL) || (!strcmp(cmd, "AT+INTPULLDOWN=ON\r")))
    {
      if(BLE_RX.at_cmd_resp_chk == AT_CMD_RESP_ADVERTISING){break;}
      else{BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;}
    }
    else{}

    if(BLE_TX.tact_time >= TACK_TIME)
    {
      BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;
      BLE_Tx_String(cmd);//send
      BLE_TX.tact_time = 0;
      BLE_TX.retry--;
    }
    else{}

    if(BLE_TX.retry < 1)// 재시도 횟수 초과 시 time out error 
    {
      BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_ERROR; 
      break;
    }
    else{}
  }
  BLE_TX.tact_time_flag = 0; //BLE Tx tact time count 중지
  BLE_TX.tact_time = 0; //Tact time count 초기화
  BLE_TX.retry = 0;
  
  memset(BLE_RX.data, 0, sizeof(BLE_RX.data));

  resp_result = BLE_RX.at_cmd_resp_chk;
  BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;
  return resp_result;
}

void BLE_Bot_Queue_Process()
{
  while((!QIsEmpty(&BLE_BOT_QUEUE)) && (!BLE_RX.at_cmd_resp_chk) && (!BLE_RX.api_resp_chk))
  {
    BLE_RX.data[ble_rx_index] = Dequeue(&BLE_BOT_QUEUE);
    
    if(BLE_FLAG.connect_status == BLE_DISCONNECT)//AT COMMAND MODE
    {
      if(BLE_RX.data[ble_rx_index] == '\r')
      {
        ble_rx_index = 0;
        if(BLE_RX.data[0] == '+') //AT command의 수신 된 응답이 "+OK", "+ERROR", etc. 일 때
        {
          if(strstr(BLE_RX.data, "+OK\r") != NULL){
            BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_OK;
          }
          else if(strstr(BLE_RX.data, "+ERROR\r") != NULL){
            BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_ERROR;
          }
          else if(strstr(BLE_RX.data, "+ADVERTISING\r") != NULL){
            BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_ADVERTISING;
          }
          else{}
          memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
        }
        else
        {
          if(strstr(BLE_RX.data, "ON\r") != NULL){
            BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_ON;
          }
          else if(strstr(BLE_RX.data, "OFF\r") != NULL){
            BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_OFF;
          }
          else{
            BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_STRING_OK; //AT command의 수신 된 응답이 string 일 때, ex) "AT+INFO?" 명령 전송 후 응답 수신
          }
        }
      }
      else{
        ble_rx_index++;
      }
    }
    else if(BLE_FLAG.connect_status == BLE_CONNECTED)// BYPASS MODE
    {
      switch(device_mode_select)
      {
        case DEV_APP_CONNECTION_MODE:
          case DEV_PRODUCTION_MODE:
          if(BLE_RX.data[0] == '|') //App 연동 데이터 CMD 처리
          {
            if(BLE_RX.data[ble_rx_index] == '#')
            {
              // Stack overflow 방지
              USART_Cmd(BLE_USART, DISABLE);
              
              ble_rx_index = 0;
              BLE_RX.api_resp_chk = API_RESP_OK;
              BLE_RX.tact_time_flag = 0;
              BLE_RX.tact_time = 0;
            }
            else{
              ble_rx_index++;
            }
          }
          else{
            ble_rx_index = 0;
          }
          //'|'시작 문자가 수신 되었다면 RX tact time count 시작
          if(ble_rx_index){
            BLE_RX.tact_time_flag = 1;
          }
          //
          break;
        case DEV_ADMIN_MODE:
          if(BLE_RX.data[ble_rx_index] == '\n') //Test mode에서 명령어 처리
          {
            ble_rx_index = 0;
            BLE_RX.api_resp_chk = API_RESP_OK;
            BLE_RX.tact_time_flag = 0;
            BLE_RX.tact_time = 0;
          }
          else{
            ble_rx_index++;
          }
          break;
      }
    }

    //BLE_RX_TACT_TIME 시간 내 명령이 정상 수신되지 않았다면 Tact time, buffer 초기화
    if((BLE_RX.tact_time >= BLE_RX_TACT_TIME)||(ble_rx_index >= (BLE_BUFFER_SIZE - 1))){
      break;
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

void BLE_Bot_Queue_Init()
{
  QueueInit(&BLE_BOT_QUEUE);
}
#endif /*__BLE_BOT_NLE521*/