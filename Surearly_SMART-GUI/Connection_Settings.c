#include <cvirte.h>		
#include <userint.h>
#include "Connection_Settings.h"
#include "main.h"

int subPanel;

int CVICALLBACK SUB_PANEL_FUNC (int panel, int event, void *callbackData,
                                int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_GOT_FOCUS:
            SetCtrlVal(panel, SUB_PANEL_sSERIAL_BAUDRATE_SEL, SERIAL_CONFIG.baud_rate); // Serial port�� �̹� Open �� ���¶�� "���� �� ����" ���� ��  baud rate ���� �׸��� ���� �� baud rate ������ update
            
            break;
        case EVENT_LOST_FOCUS:

            break;
        case EVENT_CLOSE:
            RemovePopup (1);
            break;
    }
    return 0;
}

int CVICALLBACK sBT_BLE_CONNECT (int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED)
            {
                if(BLE_FLAG.connect_status == BLE_DISCONNECTED)
                {
                    SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\n > ������� ���� �õ� ��..\n");
                    BLE_FLAG.connect_tact_time_flag = BLE_CONNECT_TACT_TIME_START; //1msec timer���� 1�� ���� BLE_Connection_Process() ����
    			}
            }
            else
            {
                SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, " > ����: �ø��� ��Ʈ ���⸦ ���� ������ �ּ���.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK sBT_BLE_SCAN (int panel, int control, int event,
                              void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED)
            {
                if(BLE_FLAG.connect_status == BLE_DISCONNECTED){
    				BLE_Scan_Start();
    			}
    			else{
    				SetCtrlVal(panel,SUB_PANEL_sBT_BLE_SCAN, 0);	
    			}
            }
            else
            {
                SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, " > ����: �ø��� ��Ʈ ���⸦ ���� ������ �ּ���.\r\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK sBT_CLOSE (int panel, int control, int event,
                        void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            RemovePopup (1);
            break;
    }
    return 0;
}

int CVICALLBACK sBT_SERIAL_PORT_OPEN (int panel, int control, int event,
                                      void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_CLOSED){
				Serial_Port_Open(subPanel);	
			}
			else if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED){
				Serial_Port_Close(subPanel);
			}
            break;
    }
    return 0;
}

int CVICALLBACK sBT_CLEAR_MON (int panel, int control, int event,
                               void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            DeleteTextBoxLines (subPanel, SUB_PANEL_MONITOR_CON_SET, 0, -1);
            break;
    }
    return 0;
}


