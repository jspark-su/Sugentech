#include "Main_UI.h"

//==============================================================================
//
// Title:		FW_Update_UI.c
// Purpose:		A short description of the implementation.
//
// Created on:	2022-01-28 at ���� 11:14:29 by ���� ��.
// Copyright:	Sugentech. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <userint.h>
#include "FW_Update_UI.h"
#include "main.h"   
//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables
char forced_fw_upd_checkbox_state = 0;
//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions
char fw_file_open_state = 0;


int CVICALLBACK BT_FW_FILE_OPEN (int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            int ret;
            char filename[500]={0};
            ret = FileSelectPopup ("", "*.bin", "*.bin", "Load Data File", VAL_LOAD_BUTTON, 0, 0, 1, 0, filename);
			if(ret > 0){
				fw_file_open_state = FW_File_Open_Handle(panel, filename);
                if(fw_file_open_state == FILE_OPEN_SUCCESS)
                {
                    SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > �߿��� ���� ���� �Ϸ�.\n");
                    SetCtrlVal(panel,TABPANEL_3_INDI_FW_FILE_NAME, filename);
                }
                else
                {
                    SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > �߿��� ���� ���� ����.\n");
                }
            }
            else
            {}
            break;
    }
    return 0;
}

int CVICALLBACK BT_FW_FILE_SEND (int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
    			if(FW_UPD_VALIABLE.process != START)
    			{
    	            if(fw_file_open_state == FILE_OPEN_SUCCESS)
    	            {
    	                FW_UPD_VALIABLE.process = START;
    	                FW_UPD_VALIABLE.function_number = FW_UPD_START_MODE;
    	                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n *** SMART �߿��� ������Ʈ ���� ***\n");
    	            }
    	            else
    	            {
    	                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > ������Ʈ ���� �� �߿��� ���� ���⸦ �������ּ���.\n");
    	                SetCtrlVal(panel, TABPANEL_3_BT_FW_FILE_SEND, 0);
    	            }
    			}
    			else
    			{
    				SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > �߿��� ������Ʈ�� �����˴ϴ�. ��ø� ��ٷ��ּ���.\n");
                    FW_Update_Mode_Variable_Init();
                    FW_UPD_VALIABLE.process = START;
    	            FW_UPD_VALIABLE.function_number = FW_UPD_END_MODE;
    			}
            }
            else
            {
                SetCtrlVal(panel, TABPANEL_3_BT_FW_FILE_SEND, 0);
                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > ����: ��� ������� ���� �� �����Ͻʽÿ�.\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_ENTER_IAP_MODE (int panel, int control, int event,
                                   void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            if(BLE_FLAG.connect_status == BLE_CONNECTED)
            {
                if(!forced_fw_upd_checkbox_state) //��� �� main application�� ���� �� üũ �ڽ� ���� �� ����� �Ǽ��� "IAP mode ����" ��ư ���� ���� 
                {
                    FW_UPD_VALIABLE.process = START;
                    FW_UPD_VALIABLE.function_number = FW_UPD_IAP_MODE;
                }
                else{
                    SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > ����: �̹� IAP mode ���� ���� �Դϴ�.\n");
                }
            }
            else{
                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > ����: ��� ������� ���� �� �����Ͻʽÿ�.\n");
            }
            break;
    }
    return 0;
}

int CVICALLBACK BT_CLEAR_MON_FW (int panel, int control, int event,
                                 void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            DeleteTextBoxLines (panel, TABPANEL_3_LOG_MONITOR_FW, 0, -1);
            break;
    }
    return 0;
}


int CVICALLBACK CHKBOX_FORCE_FW_UPD (int panel, int control, int event,
                                     void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            
            GetCtrlVal(panel, TABPANEL_3_CHKBOX_FORCE_FW_UPD, &forced_fw_upd_checkbox_state);
            
            if(forced_fw_upd_checkbox_state){ //Main application �߿��� ���� IAP�� �̿��Ͽ� �߿��� ������Ʈ ���� �� (IAP ��� ���� ���� ����)
                fw_update_mode_process = FW_UPDATE_START;
            }
            else{
                fw_update_mode_process = FW_UPDATE_END;
            }
            break;
    }
    return 0;
}
