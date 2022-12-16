/* ==============================================================================

 Title:			Read_PTR_Mode.c
 Purpose:		Raw data Read Process
 Created on:	2022-07-06 by Jisoo Park
 Copyright:		Sugentech. All Rights Reserved.

================================================================================= */


/* Includes ------------------------------------------------------------------*/
// #include "Read_PTR_Mode.h"
#include "main.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define panel	tabPanel_RP
/* Private constants ---------------------------------------------------------*/
enum
{
	RP_END = 0,
 	RP_START = 1,
 	//RP_CALIBRATION_STICK_CHECK = 2,
 	RP_READ_PTR_START = 2,
 	//RP_CALIBRATION_STATE,
 	//RP_CALIBRATION_RESULT,
 	//RP_BLE_NAME_SETUP_START,
};
/* Private variables ---------------------------------------------------------*/
PI_VALIABLE_TypeDef RP_VARIABLE;
PRODUCT_MODE_READ_PTR_Typedef PRODUCT_MODE_PTR;
/* Private function prototypes -----------------------------------------------*/
void RP_Start(void);
void RP_Calibration_Stick_Check(void);
void RP_Read_PTR_Data(void);
void RP_End(void);

void RP_Next_Process(char func_num);
void RP_Variable_Init(void);


/* ============================================================================
                      ##### Raw Data Mode functions #####
 =============================================================================*/
void RP_Process(void)
{
	if(RP_VARIABLE.process == RP_START)
	{
		switch(RP_VARIABLE.function_number)
		{
			case RP_START :
				RP_Start();
				break;
			case RP_READ_PTR_START :
				RP_Read_PTR_Data();
				break;
			case RP_END :
				RP_End();
				break;
		}
	}
}

void RP_Start(void)
{
	int vOpticsChannel;
	int vDacInterval;
	char str_buf[50] = {0};
	
	GetCtrlVal(tabPanel_RP, TABPANEL_4_RING_SEL_CH, &vOpticsChannel);
	GetCtrlVal(tabPanel_RP, TABPANEL_4_SLIDE_DAC_INTERVAL, &vDacInterval);
	sprintf(str_buf, " DAC Interval: %d | Optics Channel: L%d\r\n", vDacInterval, vOpticsChannel+1);
	
	SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, "\r\n *** Raw Data - LED (DAC) vs. PTR ***\r\n");
	SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, str_buf);
	
	PRODUCT_MODE_PTR.OpticsChannel = vOpticsChannel;
	PRODUCT_MODE_PTR.vDacInterval = vDacInterval;
	PRODUCT_MODE_PTR.status = READ_PTR_NONE;
		
	RP_Next_Process(RP_READ_PTR_START);
}


void RP_Read_PTR_Data(void)
{
	if(RP_VARIABLE.one_time_process == RESET)
	{
		RP_VARIABLE.one_time_process = SET;
		rx_msg_check = MSG_WAITING;
		API_Tx_Command_Process(CMD_READ_PTR);
		SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, "\r\n\r\n[DAC] [ADC]\r\n");
		RP_VARIABLE.cmd_retry_timer = 0;
		RP_VARIABLE.cmd_retry_count = 0;
	}
	
	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_READ_PTR))
	{
		if(PRODUCT_MODE_PTR.status == READ_PTR_READ_COMPLETE)
		{
			RP_VARIABLE.cmd_retry_timer = 0;
			RP_VARIABLE.cmd_retry_count = 0;
										  
			char str_buf[20] = {0};
			
			for(int iArrayDisplay = 0; iArrayDisplay < 10; iArrayDisplay++)
			{
				if(iArrayDisplay > 0 && PRODUCT_MODE_PTR.DacData[iArrayDisplay] == 0){
					RP_Next_Process(RP_END);
					rx_msg_check = NORMAL_MESSAGE;
					return;
				}
				sprintf(str_buf, "%d    %d\r\n", PRODUCT_MODE_PTR.DacData[iArrayDisplay], PRODUCT_MODE_PTR.PtrData[iArrayDisplay]);
				SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, str_buf);
				memset(str_buf, 0, sizeof(str_buf));
			}
			PRODUCT_MODE_PTR.status = READ_PTR_DISPLAY_COMPLETE;
		}
		rx_msg_check = MSG_WAITING;
		API_Tx_Command_Process(CMD_READ_PTR);
	}
	else if(rx_msg_check == ERROR_RETRY_MESSAGE){
		rx_msg_check = MSG_WAITING;
		API_Tx_Command_Process(CMD_READ_PTR);
	}
	else
	{
		RP_VARIABLE.cmd_retry_timer++;
		if(RP_VARIABLE.cmd_retry_timer >= 20)	// 10 = 1s
		{
			RP_VARIABLE.cmd_retry_timer = 0;
			if(RP_VARIABLE.cmd_retry_count < 3) //CMD_RETRY_COUNT_NUM)
			{
				RP_VARIABLE.cmd_retry_count++;
			}
			else {

			}
		}
	}
}

void RP_End(void)
{
	if(RP_VARIABLE.one_time_process == RESET)
	{
		if(rx_msg_check != MSG_WAITING){	
			RP_VARIABLE.one_time_process = SET;
			PRODUCT_MODE_PTR.status = READ_PTR_END;
			
			rx_msg_check = MSG_WAITING;
			API_Tx_Command_Process(CMD_READ_PTR);
		}
	}
	else 
	{
		if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_READ_PTR_END))
		{
			RP_Variable_Init();
			SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, "\r\n ------------- Read PTR Á¾·á ------------- \r\n");
			SetCtrlVal(tabPanel_RP,TABPANEL_4_BT_READ_PTR, 0);
		}
		else
		{
			RP_VARIABLE.cmd_retry_timer++;
		}
	}
}


void RP_Next_Process(char func_num)
{
  RP_VARIABLE.function_number = func_num;
  RP_VARIABLE.cmd_retry_timer = 0;
  RP_VARIABLE.cmd_retry_count = 0;
  RP_VARIABLE.one_time_process = RESET;
  rx_msg_check = MSG_NONE;
  rx_msg_function = CMD_NONE;
}

void RP_Variable_Init(void)
{
	RP_VARIABLE.process = END;
	RP_VARIABLE.cmd_retry_count = 0;
	RP_VARIABLE.cmd_retry_timer = 0;
	RP_VARIABLE.function_number = 0;
	RP_VARIABLE.one_time_process = RESET;
	
	
	PRODUCT_MODE_PTR.OpticsChannel = 0;
    PRODUCT_MODE_PTR.vDacInterval = 0;
    PRODUCT_MODE_PTR.status = READ_PTR_NONE;
	memset(&PRODUCT_MODE_PTR.DacData, 0, sizeof(PRODUCT_MODE_PTR.DacData));
	memset(&PRODUCT_MODE_PTR.DacData, 0, sizeof(PRODUCT_MODE_PTR.DacData));

	rx_msg_check = MSG_NONE;
	rx_msg_function = CMD_NONE;

    auto_setting_mode_state = END_AUTO_SETTING_MODE;
    SetCtrlVal(tabPanel_RP,TABPANEL_4_BT_READ_PTR, 0);
}
