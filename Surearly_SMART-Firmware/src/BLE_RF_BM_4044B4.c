#include "main.h"

#ifdef __BLE_RF_BM_4044B4

CQueue BLE_RF_QUEUE;

extern uint8_t ble_rx_index;
//extern char ble_rx_buf[BLE_BUFFER_SIZE];

void ble_rf_uart_init()
{
  CLK_PeripheralClockConfig(BLE_CLK_USART, ENABLE);

  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_RX_PIN, ENABLE);
  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_TX_PIN, ENABLE);
  
  USART_Init(BLE_USART, BLE_BUADRATE, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
  
  USART_ITConfig(BLE_USART, USART_IT_RXNE, ENABLE);

  USART_Cmd(BLE_USART, ENABLE);
  
  QueueInit(&BLE_RF_QUEUE);
}

void ble_rf_uart_deinit()
{
  CLK_PeripheralClockConfig(BLE_CLK_USART, DISABLE);
  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_RX_PIN, DISABLE);
  GPIO_ExternalPullUpConfig(BLE_UART_PORT, BLE_TX_PIN, DISABLE);
  USART_DeInit(BLE_USART);
  USART_ITConfig(BLE_USART, USART_IT_RXNE, DISABLE);
  USART_Cmd(BLE_USART, DISABLE);
}

void ble_rf_start()
{
  GPIO_WriteBit(BLE_UART_PORT, BLE_RTS_PIN, RESET);
  //GPIO_WriteBit(BLE_UART_PORT, BLE_RTS_PIN, SET);
  GPIO_WriteBit(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, RESET); //Module starting to broadcast

  delay_ms(100);
  //initial message("Module is working!") remove
  QueueInit(&BLE_RF_QUEUE);
  memset(ble_rx_buf, 0, sizeof(ble_rx_buf));
  //
}

void ble_rf_sleep() //BLE sleep mode
{
  GPIO_WriteBit(BLE_UART_PORT, BLE_RTS_PIN, SET);
  //Entering sleep mode
  GPIO_WriteBit(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, SET);
}

void ble_rf_wakeup() //BLE wakeup mode
{
  GPIO_WriteBit(BLE_UART_PORT, BLE_RTS_PIN, RESET);
  GPIO_WriteBit(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, RESET);
}

void ble_rf_rx_ch()
{
  unsigned char rx_data = 0;

  if(BLE_USART->SR&0x20)
  {
    rx_data = BLE_USART->DR;
    // Eco back
    if(BLE_FLAG.connect_status == BLE_CONNECTED)
      BLE_Tx_Byte(rx_data);
    //
    Enqueue(&BLE_RF_QUEUE, rx_data);
  }
}

void send_ttm_command(char* cmd)
{
  uint8_t i = 0;
  //GPIO_WriteBit(BLE_UART_PORT, BLE_RTS_PIN, RESET); //RTS LOW
  //delay_ms(2);
  BLE_Tx_String(cmd);//send
  //delay_ms(5);
  
  BLE_TX.tact_time_flag = 1;
  BLE_TX.tact_time = 0;
  BLE_TX.retry = 5;
  while(BLE_RX.at_cmd_resp_chk != AT_CMD_RESP_OK)
  {
    if(BLE_TX.tact_time == 200)//200 msec 마다 재전송 (총 5회)
    {
      BLE_Tx_String(cmd);//send
      BLE_TX.tact_time = 0;
      BLE_TX.retry--;
    }
    if(BLE_TX.retry < 1)
    {
      //Error process ble()
      BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_ERROR; //Error process ble()내에 BLE_RX.error = 0; 추가 필요 
      break;
    }
  }
  BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_NONE;
  BLE_TX.tact_time_flag = 0;
  BLE_TX.tact_time = 0;
  BLE_TX.retry = 0;
  
  //GPIO_WriteBit(BLE_UART_PORT, BLE_RTS_PIN, SET); //RTS HIGH
  //delay_ms(20);

  
  //Read message process
  if(!strcmp(cmd, "TTM:MAC-?"))
  {
    for(i = 0; i < 12; i++)
    {
      ble_mac_id[i] = BLE_RX.data[i+10];
    }
  }
  else if(!strcmp(cmd, "TTM:NAM-?"))
  {
    for(i = 8; i <38; i++)
    {
      if(BLE_RX.data[i] == 0x00)
        break;
      ble_name[i - 8] = BLE_RX.data[i];
    }
  }
  memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
}


void ble_rf_queue_process()
{
  if((!(BLE_USART->SR&0x20))&&(!QIsEmpty(&BLE_RF_QUEUE)))
  //if(!QIsEmpty(&BLE_RF_QUEUE))
  {
    ble_rx_buf[ble_rx_index] = Dequeue(&BLE_RF_QUEUE);
    
    if(BLE_FLAG.connect_status == BLE_DISCONNECT)//AT COMMAND MODE
    {
      if(ble_rx_buf[0] == 'T')
      {
        if(ble_rx_buf[ble_rx_index] == '\n')
        {
          if((ble_rx_buf[0] == 'T')&&(ble_rx_buf[1] == 'T')&&(ble_rx_buf[2] == 'M'))
          {
            if(strcmp(ble_rx_buf, "TTM:OK\r\n") == 0)
            {
              BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_OK;
            }
            else if(strcmp(ble_rx_buf, "TTM:ERP\r\n") == 0)
            {
              BLE_RX.at_cmd_resp_chk = AT_CMD_RESP_ERROR;
            }
            else
            {
              memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
              memcpy(BLE_RX.data, ble_rx_buf, sizeof(ble_rx_buf));
              BLE_RX.at_cmd_resp_chk = 1;
              //BLE_RX.api_resp_chk = API_RESP_OK;
            }
          }
          else
          {
            memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
          }
          memset(ble_rx_buf, 0, sizeof(ble_rx_buf));
          ble_rx_index = 0;
        }
        else
          ble_rx_index++;
        
        if(ble_rx_index == BLE_BUFFER_SIZE)
        {
          memset(ble_rx_buf, 0, sizeof(ble_rx_buf));
          ble_rx_index = 0;
        }
      }
      else
      {
        memset(ble_rx_buf, 0, sizeof(ble_rx_buf));
        ble_rx_index = 0;
      }
    }
    else if(BLE_FLAG.connect_status == BLE_CONNECTED)// BYPASS MODE
    {
      if(ble_rx_buf[ble_rx_index] == '\n')
      {
        ble_rx_index = 0;
        memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
        memcpy(BLE_RX.data, ble_rx_buf, sizeof(ble_rx_buf));
        BLE_RX.api_resp_chk = API_RESP_OK;
        memset(ble_rx_buf, 0, sizeof(ble_rx_buf));
      }
      else
        ble_rx_index++;
    }
  }
}

void ble_rf_queue_init()
{
  QueueInit(&BLE_RF_QUEUE);
}

#endif /*__BLE_RF_BM_4044B4*/
