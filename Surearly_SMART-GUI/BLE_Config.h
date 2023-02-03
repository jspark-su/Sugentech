#ifndef __BLE_CONFIG_H
 #define __BLE_CONFIG_H

#define BLE_DISCONNECTED        0
#define BLE_CONNECTED           1

#define AT_NONE					0
#define AT_OK					1
#define AT_PASSKEY				2
#define AT_DISCONNECTED			3
#define AT_ERROR				4
#define AT_FAIL					5

#define BLE_BUFFER_SIZE         10000

#define BLE_MAC_BUF_SIZE        13
#define BLE_NAME_LEN            21
#define BLE_RSSI_BUF_SIZE       3

#define BLE_MAXIMUM_RSSI        90 //-90 dBm

#define BLE_RX_TACT_TIME        300 //msec

#define BLE_SCAN_END			0
#define BLE_SCAN_START			1

#define BLE_CONNECT_TACT_TIME_END   	0
#define BLE_CONNECT_TACT_TIME_START		1


#define BLE_SCANNING_TIME		5000 //sec

#define BLE_CONNECT_SUCCESS     0
#define BLE_CONNECT_FAIL        1

#define BLE_CONNECT_RETRY_TIME  1000 //sec
#define BLE_CONNECT_RETRY_NUM   5

#define BLE_NAME_SETUP_CHECK_NONE       0
#define BLE_NAME_SETUP_CHECK_SUCCESS    1
#define BLE_NAME_SETUP_CHECK_FAIL       2


//Rx command mode
#define BLE_CMD_MODE        0
#define API_CMD_MODE        1
#define STRIG_OUTPUT_MODE   2
#define FW_UPD_CMD_MODE     3

//Rx start, termination byte
#define BLE_CMD_MODE_START_BYTE   (char)'+'
#define BLE_CMD_MODE_TERM_BYTE   (char)'\r'

#define API_CMD_MODE_START_BYTE   (char)'|'
#define API_CMD_MODE_TERM_BYTE   (char)'#'

#define STRING_MODE_TERM_BYTE   (char)'\n'


typedef struct
{
  char connect_status;
  char connect_tact_time_flag;
  int connect_tact_time;
  char ble_scan_state;
  char ble_name_setup_complete_check;
  char at_response_msg;
  int response_wait_time; // unit: sec
}BLUETOOTH_FLAG;

typedef struct
{
  char ok;
  char complete;
  char data[BLE_BUFFER_SIZE];
  char tact_time_flag;
  int tact_time;
  int scan_time;
  char mode_sel;
}BLUETOOTH_RX;


void BLE_Scan_Start();
void BLE_Scan_Process(int panel);
void BLE_Connection_Process();
void BLE_Rx_Dequeue(int panel);
void BLE_AT_Command_Process(int panel);

void json_test();
#endif /* __BLE_CONFIG_H*/
