#ifndef __BLE_RF_BM_4044B4_H
 #define __BLE_RF_BM_4044B4_H


//TTM COMMAND LIST
#define TTM_MAC_R               1


void ble_rf_uart_init();
void ble_rf_uart_deinit();
void ble_rf_start();
void ble_rf_sleep();
void ble_rf_wakeup();
void ble_rf_rx_ch();
void send_ttm_command(char* cmd);
void ble_rf_queue_process();
void ble_rf_queue_init();

//#endif /*__BLE_RF_BM_4044B4*/
#endif /*__BLE_RF_BM_4044B4_H*/