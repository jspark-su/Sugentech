//==============================================================================
//
// Title:		FW_Update_Process.c
// Purpose:		A short description of the implementation.
//
// Created on:	2022-01-28 at 오전 11:15:15 by 병우 윤.
// Copyright:	Sugentech. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

//
#include <windows.h>
#include <userint.h>
#include "FW_Update_Process.h"
#include "main.h"

//==============================================================================
// Constants

//==============================================================================
// Types
#define SEND_PACKET_DATA_LEN    (unsigned long)128

#define HEADER_STR              0x7B023A //0x3A, 0x02, 0x7B

#define OPCODE_START            (unsigned char)0x01
#define OPCODE_DATA_STREAM      (unsigned char)0x02
#define OPCODE_END              (unsigned char)0x03

#define DATA_ACK                (unsigned char)0x06
#define DATA_NACK               (unsigned char)0x15

#define TAIL_STR                0x0D037D //0x7D, 0x03, 0x0D

//Timer 10 msec
#define FW_UPD_START_CMD_RETRY_TIME         200 //200 * 10 msec = 2000 msec
#define FW_UPD_DATA_CMD_RETRY_TIME          100 //100 * 10 msec = 1000 msec
#define FW_UPD_END_CMD_RETRY_TIME           100 //100 * 10 msec = 1000 msec

#define FW_UPD_CMD_RETRY_COUNT_NUM          7

//==============================================================================
// Static global variables


typedef struct
{
 unsigned char header[3];
 unsigned char opcode;
 unsigned char index[2];
 unsigned char data_buffer[SEND_PACKET_DATA_LEN];
 unsigned char checksum;
 unsigned char tail[3];
}SEND_PACKET_STRUCT_Typedef;

SEND_PACKET_STRUCT_Typedef  SEND_PACKET_STRUCT = {0,};

FILE *fp = {0,};
ssize_t file_size = 0;

unsigned short total_data_stream_num = 0;
unsigned short data_stream_remain_bytes = 0;

unsigned short data_stream_count = 0;
unsigned int progress_bar_value = 0;

//==============================================================================
// Static functions
void FW_Update_Enter_IAP_Mode(int panel);
void FW_Update_Start_Mode(int panel);
void FW_Update_Data_Transmit_Mode(int panel);
void FW_Update_End_Mode(int panel);
//void FW_Update_Mode_Variable_Init(void);
void FW_Update_Mode_Next_Function(char func_num);

unsigned char Calcular_Checksum(SEND_PACKET_STRUCT_Typedef* pSEND_PACKET_STRUCT);
void Send_Packet(SEND_PACKET_STRUCT_Typedef* pSEND_PACKET_STRUCT);
char Read_FW_File_Data_Stream(unsigned short* data_stream_count, unsigned char* data_stream_buffer);
//==============================================================================
// Global variables
char* pFILE_READ_BUFFER = NULL;


char fw_update_mode_process = 0;
//==============================================================================
// Global functions


void FW_Update_Function_Process(void)
{
	if(FW_UPD_VALIABLE.process == START)
	{
	    switch(FW_UPD_VALIABLE.function_number)
	    {
          case FW_UPD_IAP_MODE :
        	FW_Update_Enter_IAP_Mode(tabPanel_FW_Update);
	        break;      
          case FW_UPD_START_MODE :
        	FW_Update_Start_Mode(tabPanel_FW_Update);
	        break;      
		  case FW_UPD_FILE_DATA_TRANSMIT :
        	FW_Update_Data_Transmit_Mode(tabPanel_FW_Update);
	        break;
          case FW_UPD_END_MODE :
        	FW_Update_End_Mode(tabPanel_FW_Update);
	        break;
	    }
	}
}

void FW_Update_Mode_Variable_Init(void)
{
	FW_UPD_VALIABLE.process = END;
    FW_UPD_VALIABLE.one_time_process = RESET;
    FW_UPD_VALIABLE.function_number = 0;
	FW_UPD_VALIABLE.cmd_retry_count = 0;
	FW_UPD_VALIABLE.cmd_retry_timer = 0;
	rx_msg_check = MSG_NONE;
	rx_msg_function = CMD_NONE;
}

void FW_Update_Mode_Next_Function(char func_num)
{
  FW_UPD_VALIABLE.function_number = func_num;
  FW_UPD_VALIABLE.cmd_retry_timer = 0;
  FW_UPD_VALIABLE.cmd_retry_count = 0;
  FW_UPD_VALIABLE.one_time_process = RESET;
  rx_msg_check = MSG_NONE;
  rx_msg_function = CMD_NONE;
}

void FW_Update_Enter_IAP_Mode(int panel)
{
    if(FW_UPD_VALIABLE.one_time_process == RESET)
	{
		FW_UPD_VALIABLE.one_time_process = SET;
		API_Tx_Command_Process(CMD_FW_UPD_ENTER_IAP_MODE);
	}

	if((rx_msg_check == NORMAL_MESSAGE) && (rx_msg_function == CMD_FW_UPD_ENTER_IAP_MODE))
	{
		SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > IAP 모드 진입 성공.\n");
        Sleep(1000);
        SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 기기의 LCD Square-1 ~ 4, Drop icon이 켜지면,\n");
        SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "   업데이트를 계속 진행하십시오.\n");
        Sleep(1000);
        SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n ** 알림 **\n");
        SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "  - 3분 내 펌웨어 업데이트를 시작하십시오.\n");
        SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "  - 시간 초과 시 기존 main application이 실행됩니다.\n");
        fw_update_mode_process = FW_UPDATE_START;
        FW_Update_Mode_Variable_Init();
	}
	else
	{
		FW_UPD_VALIABLE.cmd_retry_timer++;
		if(FW_UPD_VALIABLE.cmd_retry_timer >= FW_UPD_START_CMD_RETRY_TIME)
		{
			FW_UPD_VALIABLE.cmd_retry_timer = 0;
			if(FW_UPD_VALIABLE.cmd_retry_count < FW_UPD_CMD_RETRY_COUNT_NUM)
			{
				API_Tx_Command_Process(CMD_FW_UPD_ENTER_IAP_MODE);
				FW_UPD_VALIABLE.cmd_retry_count++;
			}
			else
			{
                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > IAP 모드 진입 실패.\n");
				SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 오류: 기기 전원, 블루투스 상태를 확인하십시오.\n");
				FW_Update_Mode_Variable_Init();
			}
		}
	}
}

void FW_Update_Start_Mode(int panel)
{
    if(FW_UPD_VALIABLE.one_time_process == RESET)
	{
        SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 펌웨어 업데이트 준비중...\n");
        Sleep(500);
		FW_UPD_VALIABLE.one_time_process = SET;
        
        BLE_RX.mode_sel = FW_UPD_CMD_MODE;
        data_stream_count = 0;
        
        for(char idx = 2; idx >= 0; idx--)
        {
            SEND_PACKET_STRUCT.header[idx] = (HEADER_STR >> 8*idx) & 0x0000FF;
            SEND_PACKET_STRUCT.tail[idx] = (TAIL_STR >> 8*idx) & 0x0000FF;
        }
        SEND_PACKET_STRUCT.opcode = OPCODE_START;
        SEND_PACKET_STRUCT.index[0] = (total_data_stream_num >> 8) & 0x00FF; //FW file open 완료 시 설정됨. (FW_File_Open_Handle())
        SEND_PACKET_STRUCT.index[1] = total_data_stream_num & 0x00FF;
        memset(SEND_PACKET_STRUCT.data_buffer, 0, sizeof(SEND_PACKET_STRUCT.data_buffer));
        SEND_PACKET_STRUCT.checksum = Calcular_Checksum(&SEND_PACKET_STRUCT);
        Send_Packet(&SEND_PACKET_STRUCT);
	}
	
	FW_UPD_VALIABLE.cmd_retry_timer++;

	if(rx_msg_check == PACKET_DATA_ACK)
	{
		SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 펌웨어 업데이트 진행 준비 완료.\n");
        Sleep(500);
        SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 펌웨어 파일 전송 시작.\n");
        FW_Update_Mode_Next_Function(FW_UPD_FILE_DATA_TRANSMIT);
	}
	else if(rx_msg_check == PACKET_DATA_NACK)
	{
		Send_Packet(&SEND_PACKET_STRUCT);
		FW_UPD_VALIABLE.cmd_retry_count++;
	}
	else
	{
		if(FW_UPD_VALIABLE.cmd_retry_timer >= FW_UPD_START_CMD_RETRY_TIME)
		{
			FW_UPD_VALIABLE.cmd_retry_timer = 0;
			if(FW_UPD_VALIABLE.cmd_retry_count < FW_UPD_CMD_RETRY_COUNT_NUM)
			{
				Send_Packet(&SEND_PACKET_STRUCT);
				FW_UPD_VALIABLE.cmd_retry_count++;
			}
			else
			{
                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 펌웨어 업데이트 시작에 실패 했습니다.\n");
				SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 오류: 기기 전원, 블루투스 상태를 확인하십시오.\n");
                fclose(fp);
				FW_Update_Mode_Variable_Init();	
                SetCtrlVal(panel, TABPANEL_3_BT_FW_FILE_SEND, 0);
			}
		}
	}
}

void FW_Update_Data_Transmit_Mode(int panel)
{
    char error_string[300] = {0};
    
    if(FW_UPD_VALIABLE.one_time_process == RESET)
	{
		FW_UPD_VALIABLE.one_time_process = SET;
        
        SEND_PACKET_STRUCT.opcode = OPCODE_DATA_STREAM;
        SEND_PACKET_STRUCT.index[0] = (data_stream_count >> 8) & 0x00FF;
        SEND_PACKET_STRUCT.index[1] = data_stream_count & 0x00FF;
        //Data stream setup
        if(Read_FW_File_Data_Stream(&data_stream_count, SEND_PACKET_STRUCT.data_buffer) == FILE_DATA_READ_SUCCESS)
        { //Firmware file data read success
            SEND_PACKET_STRUCT.checksum = Calcular_Checksum(&SEND_PACKET_STRUCT);
            Send_Packet(&SEND_PACKET_STRUCT);
        }
        else
        {//Firmware file data read fail
            memset(error_string, 0, sizeof(error_string));
            sprintf(error_string, "\n > 오류: 펌웨어 파일로부터 데이터 읽기에 실패 했습니다. File index: %d\n", data_stream_count);
            SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 기기 재부팅 후, 업데이트를 재시도 하십시오.\n");
            SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, error_string);
            fclose(fp);
            FW_Update_Mode_Variable_Init();
            SetCtrlVal(panel, TABPANEL_3_BT_FW_FILE_SEND, 0);
        }
	}
	
	FW_UPD_VALIABLE.cmd_retry_timer++;
	
	if(rx_msg_check == PACKET_DATA_ACK)
	{
        data_stream_count++;
        
        char percent_value_str[4] = {0};
        progress_bar_value = (unsigned int)(((float)data_stream_count / (float)total_data_stream_num) * 100.0);
        sprintf(percent_value_str, "%d", progress_bar_value);
        SetCtrlVal(panel, TABPANEL_3_INDI_FW_DOWNLOADING, progress_bar_value);
        SetCtrlVal(panel, TABPANEL_3_INDI_FW_DOWN_PERCENT, percent_value_str);
        
        if(data_stream_count < total_data_stream_num)
        {
            FW_Update_Mode_Next_Function(FW_UPD_FILE_DATA_TRANSMIT);
        }
        else
        {
            SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 펌웨어 파일 전송 완료.\n");
            Sleep(500);
            FW_Update_Mode_Next_Function(FW_UPD_END_MODE);
        }
	}
	else if(rx_msg_check == PACKET_DATA_NACK)
	{
		Send_Packet(&SEND_PACKET_STRUCT);
		FW_UPD_VALIABLE.cmd_retry_count++;
	}
	else
	{
		if(FW_UPD_VALIABLE.cmd_retry_timer >= FW_UPD_DATA_CMD_RETRY_TIME)
		{
			FW_UPD_VALIABLE.cmd_retry_timer = 0;
			if(FW_UPD_VALIABLE.cmd_retry_count < FW_UPD_CMD_RETRY_COUNT_NUM)
			{
				Send_Packet(&SEND_PACKET_STRUCT);
				FW_UPD_VALIABLE.cmd_retry_count++;
			}
			else
			{
				memset(error_string, 0, sizeof(error_string));
                sprintf(error_string, "\n > 오류: 펌웨어 파일 전송 실패. File index: %d\n", data_stream_count);
                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, error_string);
                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 기기 재부팅 후, 업데이트를 재시도 하십시오.\n");
                fclose(fp);
                FW_Update_Mode_Variable_Init();
                SetCtrlVal(panel, TABPANEL_3_BT_FW_FILE_SEND, 0);
			}
		}
	}
}

void FW_Update_End_Mode(int panel)
{
    if(FW_UPD_VALIABLE.one_time_process == RESET)
	{
		FW_UPD_VALIABLE.one_time_process = SET;
        //total_data_stream_num = 0;
        //data_stream_count = 0;
        fclose(fp);
        SEND_PACKET_STRUCT.opcode = OPCODE_END;
        SEND_PACKET_STRUCT.index[0] = 0; //FW file open 완료 시 설정됨. (FW_File_Open_Handle())
        SEND_PACKET_STRUCT.index[1] = 0;
        memset(SEND_PACKET_STRUCT.data_buffer, 0, sizeof(SEND_PACKET_STRUCT.data_buffer));
        SEND_PACKET_STRUCT.checksum = Calcular_Checksum(&SEND_PACKET_STRUCT);
        Send_Packet(&SEND_PACKET_STRUCT);
	}
	
	FW_UPD_VALIABLE.cmd_retry_timer++;
	
	if(rx_msg_check == PACKET_DATA_ACK)
	{
        if(data_stream_count == total_data_stream_num)
        {
            //Firmware update 완료
            SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 기기의 펌웨어 업데이트가 완료되었습니다.\n");
            Sleep(1000);
            SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > Main application 프로그램이 시작됩니다.\n");
        }
        else
        {
            //Firmware update 강제 종료됨.
            SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 펌웨어 업데이트 중지 완료.\n");
            SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 업데이트를 재시도 하십시오.\n");
            SetCtrlVal(panel, TABPANEL_3_INDI_FW_DOWNLOADING, 0);
            SetCtrlVal(panel, TABPANEL_3_INDI_FW_DOWN_PERCENT, 0);
        }
        data_stream_count = 0;
        SetCtrlVal(panel, TABPANEL_3_BT_FW_FILE_SEND, 0);
        SetCtrlVal(panel, TABPANEL_3_CHKBOX_FORCE_FW_UPD, 0);
        SetCtrlVal(panel, TABPANEL_3_INDI_FW_FILE_NAME, 0);
        fw_file_open_state = 0;
        
        BLE_RX.mode_sel = BLE_CMD_MODE;
        fw_update_mode_process = FW_UPDATE_END;
        FW_Update_Mode_Variable_Init();
	}
	else if(rx_msg_check == PACKET_DATA_NACK)
	{
		Send_Packet(&SEND_PACKET_STRUCT);
		FW_UPD_VALIABLE.cmd_retry_count++;
	}
	else
	{
		if(FW_UPD_VALIABLE.cmd_retry_timer >= FW_UPD_END_CMD_RETRY_TIME)
		{
			FW_UPD_VALIABLE.cmd_retry_timer = 0;
			if(FW_UPD_VALIABLE.cmd_retry_count < FW_UPD_CMD_RETRY_COUNT_NUM)
			{
				Send_Packet(&SEND_PACKET_STRUCT);
				FW_UPD_VALIABLE.cmd_retry_count++;
			}
			else
			{
                SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 오류: 펌웨어 파일 전송에 성공하였지만, 업데이트 기능 종료에 실패했습니다.\n");
				SetCtrlVal(panel, TABPANEL_3_LOG_MONITOR_FW, "\n > 기기 재부팅 후, 업데이트를 재시도 하십시오.\n");
                fclose(fp);
				FW_Update_Mode_Variable_Init();
                SetCtrlVal(panel, TABPANEL_3_BT_FW_FILE_SEND, 0);
			}
		}
	}
}

char FW_File_Open_Handle(int panel, char *filename)
{
    if((fp = fopen(filename, "rb")) != NULL)
	{
        GetFileSize(filename, &file_size);
        total_data_stream_num = (unsigned short)(file_size / SEND_PACKET_DATA_LEN);
        data_stream_remain_bytes = (unsigned short)((unsigned long)file_size % SEND_PACKET_DATA_LEN);
        if(data_stream_remain_bytes){
            total_data_stream_num = total_data_stream_num + 1;
        }
        else{}
        return FILE_OPEN_SUCCESS;
    }
    else{ //File open fail
        return FILE_OPEN_FAIL;
    }
}

char Read_FW_File_Data_Stream(unsigned short* data_stream_count, unsigned char* data_stream_buffer)
{
    char file_read_buf[SEND_PACKET_DATA_LEN] = {0};
    
    if(*data_stream_count < total_data_stream_num) //Packet size 단위 전송
    {
        fseek(fp, SEND_PACKET_DATA_LEN * (*data_stream_count), SEEK_SET);
        if((data_stream_remain_bytes) && (*data_stream_count == total_data_stream_num - 1)) //남은 bytes read
        {
            if(fread(file_read_buf, 1, (size_t)data_stream_remain_bytes, fp)) //File read success
            {
                memcpy(data_stream_buffer, file_read_buf, SEND_PACKET_DATA_LEN);
                return FILE_DATA_READ_SUCCESS;
            }
            else{ //File read fail
                return FILE_DATA_READ_FAIL;
            }
        }
        else //남은 bytes가 없을때
        {
            if(fread(file_read_buf, 1, (size_t)SEND_PACKET_DATA_LEN, fp)) //File read success
            {
                memcpy(data_stream_buffer, file_read_buf, SEND_PACKET_DATA_LEN);
                return FILE_DATA_READ_SUCCESS;
            }
            else{ //File read fail
                return FILE_DATA_READ_FAIL;
            }
        }
    }
    else{ //fseek index 범위 초과
        return FILE_READ_IDEX_EXCEED;
    }
}

void FW_File_Transmit()
{
    if(BLE_FLAG.connect_status == BLE_CONNECTED)
    {
        if(total_data_stream_num)
        {
            for(int idx = 0; idx < total_data_stream_num; idx++){
                ComWrt(SERIAL_CONFIG.port_number, (const char*)pFILE_READ_BUFFER[idx * DATA_PACKET_UNIT], DATA_PACKET_UNIT);
            }
        }
        else{
            
        }
        free(pFILE_READ_BUFFER);
    }
    else{
        
    }
}

char Received_Packet_Verify(unsigned char* received_packet_buffer) //Arithmetic sum algorithm
{
    unsigned char cal_checksum = 0;
    for(int idx = 0; idx < RESPONSE_PACKET_TOTAL_LEN - PACKET_TAIL_LEN; idx++)
    {
        cal_checksum += received_packet_buffer[idx];
    }
    
    if(!cal_checksum){ //Checksum 'Ok'
        return PACKET_CHECKSUM_OK;
    }
    else{ //Checksum 'Error'
        return PACKET_CHECKSUM_ERROR;
    }
}

unsigned char Calcular_Checksum(SEND_PACKET_STRUCT_Typedef* pSEND_PACKET_STRUCT)
{
    unsigned char cal_checksum = 0;
    unsigned char* p_sendpacket = pSEND_PACKET_STRUCT->header;
    unsigned char check_data = 0;
    
    for(int idx = 0; idx < SEND_PACKET_CHECKSUM_IDEX; idx++)
    {
        check_data = *p_sendpacket;
        cal_checksum += *p_sendpacket;
        p_sendpacket++;
    }
    cal_checksum = ~cal_checksum + 0x01;
    return cal_checksum;
}

void Send_Packet(SEND_PACKET_STRUCT_Typedef* pSEND_PACKET_STRUCT)
{
    ComWrt(SERIAL_CONFIG.port_number, (const char*)pSEND_PACKET_STRUCT->header, sizeof(SEND_PACKET_STRUCT_Typedef));
}