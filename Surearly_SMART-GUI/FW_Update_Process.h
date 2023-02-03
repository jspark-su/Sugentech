//==============================================================================
//
// Title:		FW_Update_Process.h
// Purpose:		A short description of the interface.
//
// Created on:	2022-01-28 at ¿ÀÀü 11:15:15 by º´¿ì À±.
// Copyright:	Sugentech. All Rights Reserved.
//
//==============================================================================

#ifndef __FW_Update_Process_H__
#define __FW_Update_Process_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types
        
#define FW_UPDATE_PASSWORD  (char*)"sugentech_20111209"
#define DATA_PACKET_UNIT    (long)128

#define FW_UPDATE_END               0
#define FW_UPDATE_START             1        

#define FILE_OPEN_NONE              0
#define FILE_OPEN_SUCCESS           1
#define FILE_OPEN_FAIL              2

        
#define FILE_DATA_READ_SUCCESS      0
#define FILE_DATA_READ_FAIL         1
#define FILE_READ_IDEX_EXCEED       2

        
#define SEND_PACKET_TOTAL_LEN           138
#define RESPONSE_PACKET_TOTAL_LEN       10
        
#define PACKET_HEADER_LEN               3
#define PACKET_TAIL_LEN                 3

#define SEND_PACKET_OPCODE_INDEX        3
#define SEND_PACKET_INDEX_START_INDEX   4
#define SEND_PACKET_DATA_START_INDEX    5
#define SEND_PACKET_CHECKSUM_IDEX       134        
        
#define RESP_PACKET_HEADER_START_INDEX	0
#define RESP_PACKET_TAIL_START_INDEX	7
#define RESP_PACKET_DATA_INDEX          5
		
#define RESP_DATA_ACK					0x06
#define RESP_DATA_NACK					0x15

#define PACKET_CHECKSUM_OK				0
#define PACKET_CHECKSUM_ERROR			1        
        
enum
{
 FW_UPD_IAP_MODE = 1,
 FW_UPD_START_MODE,
 FW_UPD_FILE_DATA_TRANSMIT,
 FW_UPD_END_MODE
};        
//==============================================================================
// External variables
extern char fw_update_mode_process;

typedef struct
{
    char one_time_process;
    char process;
	char process_state;
    char progress_stop;
    char function_number;
    char cmd_check;
    unsigned int cmd_retry_count;
    unsigned int cmd_retry_timer;
}FW_UPD_VALIABLE_TypeDef;

FW_UPD_VALIABLE_TypeDef FW_UPD_VALIABLE;
//==============================================================================
// Global functions
void FW_Update_Function_Process(void);
char FW_File_Open_Handle(int panel, char *filename);
char Received_Packet_Verify(unsigned char* received_packet_buffer);
void FW_Update_Mode_Variable_Init(void);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __FW_Update_Process_H__ */
