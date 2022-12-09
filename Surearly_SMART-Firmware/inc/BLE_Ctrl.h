#ifndef __BLE_CTRL_H
 #define __BLE_CTRL_H

//BLE device select
#define __BLE_BOT_NLE521
//#define __BLE_RF_BM_4044B4
//

#define BLE_BUADRATE            (uint32_t)38400

#define BLE_CLK_USART           CLK_Peripheral_USART1
#define BLE_USART               USART1
#define BLE_UART_PORT           GPIOA
#define BLE_RX_PIN              GPIO_Pin_3
#define BLE_TX_PIN              GPIO_Pin_2

#define BLE_CTS_PORT            GPIOE
#define BLE_CTS_PIN             GPIO_Pin_4

#define BLE_RTS_PORT            GPIOE
#define BLE_RTS_PIN             GPIO_Pin_5

#define BLE_C_STATUS_PORT       GPIOC
#define BLE_C_STATUS_PIN        GPIO_Pin_1

#define BLE_SLEEP_WAKE_PORT     GPIOD
#define BLE_SLEEP_WAKE_PIN      GPIO_Pin_2

#define BLE_AT_BYP_PORT         GPIOD
#define BLE_AT_BYP_PIN          GPIO_Pin_4

#define DISCO_FACT_BLE_PORT     GPIOB  
#define DISCO_FACT_BLE_PIN      GPIO_Pin_4


#define BLE_DISCONNECT          0
#define BLE_CONNECTED           1

#define BLE_BUFFER_SIZE         240

#define BLE_NAME_LEN            21 //"SurearlySmart-000000"
#define BLE_MAC_ADDR_LEN        13 //MAC address = "5CF286428515"

#define BLE_NAME_CMD_BUF_SIZE   31 //"AT+MANUF=SurearlySmart-000000\r"
#define BLE_MAC_ADDR_BUF_SIZE   18 //MAC address = "5C:F2:86:42:85:15"

#define BLE_TX_TACT_TIME        3000
#define BLE_RX_TACT_TIME        200



#define BLE_NAME_SETUP_COMPLETE         0
#define BLE_NAME_SETUP_FAIL_AT_INFO     1
#define BLE_NAME_SETUP_FAIL_AT_MANUF    2
#define BLE_NAME_SETUP_FAIL_VERIFY      3

#define BLE_BAUDRATE_SETUP_PASS         0
#define BLE_BAUDRATE_SETUP_COMPLETE     1
#define BLE_BAUDRATE_SETUP_FAIL         2

#define BLE_SETUP_NONE                  (uint8_t)0x00
#define BLE_SETUP_COMPLETE              (uint8_t)0x01

#define BLE_UART_BLOCK_DISABLE          0
#define BLE_UART_BLOCK_ENABLE           1


typedef struct
{
  char connect_status;
  char ble_icon_update;
  char ble_icon_disc_en;
  char ble_icon_con_en;
}BLUETOOTH_FLAG;

typedef struct
{
  char at_cmd_resp_chk;
  char api_resp_chk;
  char data[BLE_BUFFER_SIZE];
  char tact_time_flag;
  int tact_time;
}BLUETOOTH_RX;

typedef struct
{
  char retry; // 응답 재시도 횟수 count
  char tact_time_flag; //AT Command response 대기 
  int tact_time; // AT Command response count variable
  char data[BLE_BUFFER_SIZE];
  int index; // data[] buffer 참초 index 
  char flag; //전송할 API message 여부 확인
  int data_cnt;
}BLUETOOTH_TX;

void BLE_Init();
void BLE_Deinit();
void BLE_Start();
void BLE_Wakeup();
void BLE_Sleep();
char BLE_Name_Setting(char* ble_name_str);
void BLE_Connection_Check();
void BLE_Disconnect();
void BLE_Rx_Enqueue();
void BLE_Queue_Process();
void BLE_Tx_Byte(char ch);
void BLE_Tx_String(char* str);
void BLE_Buffer_Init();
char BLE_Name_Auto_Setting();

#endif /* __BLE_CTRL_H*/