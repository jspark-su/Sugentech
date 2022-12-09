#ifndef __BLE_BOT_NLE521_H
 #define __BLE_BOT_NLE521_H
 

//AT_COMMAND_LIST //R == Read, W == Write
#define AT                      1
#define ATZ                     2
#define AT_VER_R                3
#define AT_INFO_R               4
#define AT_MANUF_R              5
#define AT_MANUF_W              6
#define AT_UART_W               7
#define AT_TXPWR_R              8
//AT command response
#define AT_CMD_RESP_NONE        0
#define AT_CMD_RESP_OK          1
#define AT_CMD_RESP_READY       2
#define AT_CMD_RESP_ADVERTISING 3
#define AT_CMD_RESP_STRING_OK   4
#define AT_CMD_RESP_ERROR       5
#define AT_CMD_RESP_TIMEOUT     6
//API command response
#define API_RESP_NONE           0
#define API_RESP_OK             1
#define API_RESP_ERROR          2
//BLE name setup response
#define BLE_NAME_SETUP_NONE             0
#define BLE_NAME_SETUP_OK               1
#define BLE_NAME_SETUP_LENGHTH_ERROR    2
#define BLE_NAME_SETUP_TIMEOUT          3


void BLE_Bot_Uart_Init();
void BLE_Bot_Uart_Deinit();
void BLE_Bot_Wakeup();
void BLE_Bot_Sleep();
char BLE_Bot_Name_Setting();
void BLE_Bot_Disconnect();
void BLE_Bot_Rx_Enqueue();
char Send_AT_Command(char* cmd);
void BLE_Bot_Queue_Process();
void BLE_Bot_Queue_Init();
char Resetting_BaudRate();
char BLE_SetupBaudrate();

//#endif /*__BLE_BOT_NLE521*/
#endif /* __BLE_BOT_NLE521_H*/