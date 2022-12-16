#include "main.h"

RX_MSG_CHECK_Typedef rx_msg_check = MSG_NONE;
RX_MSG_FUNCTION_Typedef rx_msg_function = CMD_NONE;

void API_Rx_Command_Process()
{
  if(BLE_RX.complete)
  {
    JSON_Value *rootValue;
    JSON_Object *rootObject;

    rootValue = json_parse_file(&BLE_RX.data[17]);      // JSON ?????? ?¬à? ???
	if(rootValue == NULL){
	    json_value_free(rootValue);
	    memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
	    BLE_RX.complete = 0;
		return;
	}

	rootObject = json_value_get_object(rootValue);    // JSON_Value???? JSON_Object?? ????
	if(rootObject == NULL){
	    json_value_free(rootValue);
	    json_object_clear(rootObject);
	    memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
	    BLE_RX.complete = 0;
		return;
	}
	
    int rx_strlen = str_to_dec(&BLE_RX.data[1], 4);
	
	const char* json_cmd = json_object_get_string(rootObject, "cmd");
	if(json_cmd == NULL){
	    json_value_free(rootValue);
	    json_object_clear(rootObject);
	    memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
	    BLE_RX.complete = 0;
		return;
	}
	int error_code = (int)json_object_get_number(rootObject, "code");
	
    
    char* begin = strchr(BLE_RX.data, '{');
    char* end = strrchr(BLE_RX.data, '#');
    
    int real_strlen = (int)(end - begin);
 
    if(rx_strlen == real_strlen)
    {
	  if((error_code == 404) || (error_code == 411) || (error_code == 416)) //code ??? (Smart Device ???? ????? Error) 
	  {
	  	rx_msg_check = ERROR_RETRY_MESSAGE;
	  }
	  else //Smart Device ???? ????? ????
	  {
	      if(strlen(json_cmd) <= API_CMD_LEN)
	      {
	        if(!strcmp(json_cmd,"PRODUCT_MODE"))
	        {
				rx_msg_check = MSG_NONE;
          		const char* state_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "state");
				if(!strcmp(state_str,"start"))
				{
					const char* dev_id_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "dev_id");
					const char* fw_ver_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "fw_ver");
					
					memset(dev_uid, 0, sizeof(dev_uid));
					memset(fw_ver, 0, sizeof(fw_ver));
					
					memcpy(dev_uid, dev_id_str, sizeof(dev_uid));
					memcpy(fw_ver, fw_ver_str, sizeof(fw_ver));
					
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_PRODUCT_MODE_START;
				}
				else if(!strcmp(state_str,"end"))
				{
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_PRODUCT_MODE_END;
				}
				else
				{
					//Error Handle
					//Error Incorrect Value
					rx_msg_check = ERROR_INCORRECT_VALUE;  
				}
	        }
	        else if(!strcmp(json_cmd,"CALIB_STAT"))
	        {
				rx_msg_check = MSG_NONE;
				const char* stick_state_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "stick_state");
				const char* proc_state_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "proc_state");
				
				if(!strcmp(stick_state_str, "insert"))
				{
					stick_state_check = STICK_INSERTED;
					rx_msg_check = NORMAL_MESSAGE;
				}
				else if(!strcmp(stick_state_str, "eject"))
				{
					stick_state_check = STICK_EJECTED;
					rx_msg_check = NORMAL_MESSAGE;
				}
				else
				{
					//Error Handle
					//Error Incorrect Value
					rx_msg_check = ERROR_INCORRECT_VALUE;  
				}
				
				if(!strcmp(proc_state_str, "none"))
				{
					calibration_proc_state = STATE_NONE;
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_CALIBRATION_STICK_CHECK;
				}
				else if(!strcmp(proc_state_str, "doing"))
				{
					calibration_proc_state = STATE_DOING;
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_CALIBRATION_START;
				}
				else if(!strcmp(proc_state_str, "complete"))
				{
					calibration_proc_state = STATE_COMPLETE;
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_CALIBRATION_STATE_CHECK;
				}
				else if(!strcmp(proc_state_str, "fail"))
				{
					calibration_proc_state = STATE_FAIL;
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_CALIBRATION_STATE_CHECK;
				}
				else
				{
					//Error Handle
					//Error Incorrect Value
					rx_msg_check = ERROR_INCORRECT_VALUE;  
				}
	        }
	        else if(!strcmp(json_cmd,"CALIB_RSLT"))
	        {
				rx_msg_check = MSG_NONE;
				int i = 0;
				
				JSON_Array *dac_data_array = json_object_get_array(json_object_dotget_object(rootObject, "data"), "dac_data");
	            JSON_Array *ptr_data_array = json_object_get_array(json_object_dotget_object(rootObject, "data"), "ptr_data");
				
				int dac_data_array_cnt = json_array_get_count(dac_data_array);
	            int ptr_data_array_cnt = json_array_get_count(ptr_data_array);
				
				for (i = 0; i < dac_data_array_cnt; i++)
	            {
				 	DATA_BANK.CALIBRATION_DATA[i + 1] = (int)json_array_get_number(dac_data_array, i);	
				}
				
				for (i = 0; i < ptr_data_array_cnt; i++)
	            {
					DATA_BANK.INITIAL_STICK_SENSING_DATA[i + 1] = (int)json_array_get_number(ptr_data_array, i);	
				}
                
                memcpy(TEST_REPORT_VAR.led_calib_dac,  DATA_BANK.CALIBRATION_DATA, sizeof(TEST_REPORT_VAR.led_calib_dac));
                memcpy(TEST_REPORT_VAR.led_calib_ptr,  DATA_BANK.INITIAL_STICK_SENSING_DATA, sizeof(TEST_REPORT_VAR.led_calib_ptr));
				rx_msg_check = NORMAL_MESSAGE;
				rx_msg_function = CMD_CALIBRATION_RESULT;
	        }
			else if(!strcmp(json_cmd,"BLE_NAME"))
	        {
				rx_msg_check = MSG_NONE;
          		const char* setup_state_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "setup_state");
                if(!strcmp(setup_state_str, "pass"))
                {
                    rx_msg_check = NORMAL_MESSAGE;
                    rx_msg_function = CMD_BLE_NAME_SETUP_PASS;
                }
                else if(!strcmp(setup_state_str, "start"))
                {
                    const char* receive_ble_name = json_object_get_string(json_object_dotget_object(rootObject, "data"), "name");
                    if(!strcmp(ble_name_check_string, receive_ble_name)){rx_msg_check = NORMAL_MESSAGE;}
                    else{rx_msg_check = ERROR_INCORRECT_VALUE;}
					rx_msg_function = CMD_BLE_NAME_SETUP_START;
                }
                else{rx_msg_check = ERROR_INCORRECT_VALUE;}
	        }
			else if(!strcmp(json_cmd,"RAW_DATA"))
	        {
				int i = 0;
				rx_msg_check = MSG_NONE;
				const char* state_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "state");
				
				if(!strcmp(state_str, "none"))
				{
					qa_proc_state = STATE_NONE;
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_QA_START;
				}
				else if(!strcmp(state_str, "doing"))
				{
					qa_proc_state = STATE_DOING;
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_QA_DOING;
				}
				else if(!strcmp(state_str, "complete"))
				{
					qa_proc_state = STATE_COMPLETE;
					rx_msg_check = NORMAL_MESSAGE;
					rx_msg_function = CMD_QA_RESULT;
				}
				else
				{
					//Error Handle
					//Error Incorrect Value
					rx_msg_check = ERROR_INCORRECT_VALUE;  
				}

                if(qa_proc_state == STATE_COMPLETE)
                {
              		JSON_Array *rawdata_array = json_object_get_array(json_object_dotget_object(rootObject, "data"), "ch_rawdata");
    				int rawdata_array_cnt = json_array_get_count(rawdata_array);
				
    				for (i = 0; i < rawdata_array_cnt; i++){
                        /*
    					DATA_BANK.STD_STICK_SENSING_RAW_DATA[i + 1] = (int)json_array_get_number(rawdata_array, i); //QA_Setting_Mode ¿ë
                        DATA_BANK.STICK_SENSING_RAW_DATA[i + 1] = (int)json_array_get_number(rawdata_array, i); //QA_Test_Mode ¿ë
                        */
                        DATA_BANK.STD_STICK_RATIO_DATA[i + 1] = (float)json_array_get_number(rawdata_array, i);
    	            }
                    memcpy(DATA_BANK.CHANNEL_RATIO_DATA, DATA_BANK.STD_STICK_RATIO_DATA, sizeof(DATA_BANK.CHANNEL_RATIO_DATA));
                }
	        }
            else if(!strcmp(json_cmd,"FW_UPD"))
	        {
				rx_msg_check = MSG_NONE;
				const char* password_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "password");
                if(!strcmp(password_str, FW_UPDATE_PASSWORD))
                {
                    rx_msg_check = NORMAL_MESSAGE;
                    rx_msg_function = CMD_FW_UPD_ENTER_IAP_MODE;
                }
                else{rx_msg_check = ERROR_INCORRECT_VALUE;}
	        }
			else if(!strcmp(json_cmd,"READ_PTR"))
			{
				//SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, "Response\r\n");
				//SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, BLE_RX.data);
				//SetCtrlVal(tabPanel_RP, TABPANEL_4_LOG_MONITOR_RAW, "\r\n");
				
				// rx_msg_check = MSG_NONE;
				
				int device_status = json_object_get_number(json_object_dotget_object(rootObject, "data"), "status");

				if(PRODUCT_MODE_PTR.status == READ_PTR_NONE)
				{
					if(device_status == READ_PTR_READ_COMPLETE)
					{
						JSON_Array *ArrDacData = json_object_get_array(json_object_dotget_object(rootObject, "data"), "dac_data");
			            JSON_Array *ArrPtrData = json_object_get_array(json_object_dotget_object(rootObject, "data"), "ptr_data");
						int cDac = json_array_get_count(ArrDacData);
						//int cPTR = json_array_get_count(ArrPtrData);
						
						for (int iArray = 0; iArray < cDac; iArray++)
			            {
						 	PRODUCT_MODE_PTR.DacData[iArray] = (int)json_array_get_number(ArrDacData, iArray);
							PRODUCT_MODE_PTR.PtrData[iArray] = (int)json_array_get_number(ArrPtrData, iArray);
						}
						
						PRODUCT_MODE_PTR.status = READ_PTR_READ_COMPLETE;
					}
				}
				else if(PRODUCT_MODE_PTR.status == READ_PTR_DISPLAY_COMPLETE){
					if(device_status == READ_PTR_NONE){
						PRODUCT_MODE_PTR.status = READ_PTR_NONE;
					}
				}
				
				rx_msg_function = CMD_READ_PTR;
				
				if(device_status == 3){
					rx_msg_function = CMD_READ_PTR_END;
				}
				rx_msg_check = NORMAL_MESSAGE;
			}
	        else
	        {
	          //Error Handle
	          //Unidentified Command (404)
			  rx_msg_check = ERROR_UNIDENTIFIED_CMD;
	        }
	      }
		  else
	  	  {
		  	//Error Handle
          	//Unidentified Command (404)
			rx_msg_check = ERROR_UNIDENTIFIED_CMD;
      	  }
	  }
    }
    else
    {
        //Error Handle
        //Length Mismatch (411)
		rx_msg_check = ERROR_LENGTH_MISMATCH;
    }
    json_object_clear(rootObject);
    json_value_free(rootValue);    // JSON_Value?? ???? ???? ??? ????
    memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
    BLE_RX.complete = 0;
  }
}

char tx_cmd_msg[500] = {0};

void API_Tx_Command_Process(char cmd_name)
{
	char *serialized_string = NULL;
	
	char cmd_name_str[13] ={0};
	int idx = 0;
	int i = 0;
    
    memset(tx_cmd_msg, 0, sizeof(tx_cmd_msg));
	
	JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
	
	
	switch(cmd_name)
    {
      case CMD_PRODUCT_MODE_START :
		memcpy(cmd_name_str, "PRODUCT_MODE", sizeof(cmd_name_str));  
        json_object_set_string(root_object, "cmd", "PRODUCT_MODE");
		json_object_dotset_string(root_object, "data.state", "start");
        break;
      case CMD_CALIBRATION_STICK_CHECK :
	  	case CMD_CALIBRATION_STATE_CHECK :
		  case CMD_QA_STICK_CHECK :
			memcpy(cmd_name_str, "CALIB_STAT", sizeof(cmd_name_str));
			json_object_set_string(root_object, "cmd", "CALIB_STAT");
			json_object_dotset_string(root_object, "data.test_state", "state");
        break;
      case CMD_CALIBRATION_START :
		memcpy(cmd_name_str, "CALIB_STAT", sizeof(cmd_name_str));  
        json_object_set_string(root_object, "cmd", "CALIB_STAT");
		json_object_dotset_string(root_object, "data.test_state", "start");
        break;
	  case CMD_CALIBRATION_RESULT :
		memcpy(cmd_name_str, "CALIB_RSLT", sizeof(cmd_name_str));  
        json_object_set_string(root_object, "cmd", "CALIB_RSLT");
        break;
      case CMD_BLE_NAME_SETUP_START :
		memcpy(cmd_name_str, "BLE_NAME", sizeof(cmd_name_str));
        json_object_set_string(root_object, "cmd", "BLE_NAME");
        json_object_dotset_string(root_object, "data.setup_state", "start");
		json_object_dotset_string(root_object, "data.name", ble_name_check_string);
        break;
	  case CMD_QA_START :
		memcpy(cmd_name_str, "RAW_DATA", sizeof(cmd_name_str));  
        json_object_set_string(root_object, "cmd", "RAW_DATA");
		json_object_dotset_string(root_object, "data.test_state", "start");
        break;
	  case CMD_QA_RESULT :
		memcpy(cmd_name_str, "RAW_DATA", sizeof(cmd_name_str));  
        json_object_set_string(root_object, "cmd", "RAW_DATA");
		json_object_dotset_string(root_object, "data.test_state", "state");
        break;		
      case CMD_PRODUCT_MODE_END :
		memcpy(cmd_name_str, "PRODUCT_MODE", sizeof(cmd_name_str));  
        json_object_set_string(root_object, "cmd", "PRODUCT_MODE");
		json_object_dotset_string(root_object, "data.state", "end");
        break;
      case CMD_FW_UPD_ENTER_IAP_MODE :
		memcpy(cmd_name_str, "FW_UPD", sizeof(cmd_name_str));  
        json_object_set_string(root_object, "cmd", "FW_UPD");
		json_object_dotset_string(root_object, "data.password", FW_UPDATE_PASSWORD);
        break;
	  case CMD_READ_PTR :
		memcpy(cmd_name_str, "READ_PTR", sizeof(cmd_name_str));
		json_object_set_string(root_object, "cmd", "READ_PTR");
		json_object_dotset_number(root_object, "data.optics_channel", PRODUCT_MODE_PTR.OpticsChannel);
		json_object_dotset_number(root_object, "data.dac_interval", PRODUCT_MODE_PTR.vDacInterval);
		json_object_dotset_number(root_object, "data.status", PRODUCT_MODE_PTR.status);
		break;
    }
	
	serialized_string = json_serialize_to_string(root_value);
	memcpy(&tx_cmd_msg[17], serialized_string, strlen(serialized_string));
	
	int cmd_strlen = strlen(cmd_name_str);
	int data_strlen = strlen(&tx_cmd_msg[17]);

  	for(idx = 0; idx <= 5; idx++)
  	{
    	if(idx == 0){
      	tx_cmd_msg[idx] = START_STR;
    	}
    	else if(idx <= 4){
      	tx_cmd_msg[idx] = ((data_strlen % (int)pow(10.0, (double)(5-idx))) / (int)pow(10.0, (double)(4-idx))) + 0x30;
    	}
    	else if(idx == 5)
    	{
      		for(i = idx; i < idx+cmd_strlen; i++){
          		tx_cmd_msg[i] = cmd_name_str[i-idx];
      		}
      		for(i = idx+cmd_strlen; i < idx+MSG_NAME; i++){
          		tx_cmd_msg[i] = '@';        
      		}
    	}      
  	}
	tx_cmd_msg[data_strlen + 17] = END_STR;
	
	json_free_serialized_string(serialized_string);
    json_value_free(root_value);
	
	Serial_Write_String(tx_cmd_msg);
}

int str_to_dec(char *str, int len) //String -> Decimal ???
{
  int dec = 0;
  int i = 0;

  for(i = 0; i < len; i++)
  {
    if((0x30 <= str[i])&&(str[i] <= 0x39)){
      dec += (str[i]-0x30) * (int)pow(10.0, (double)(len-i-1));
    }
  }

  return dec;
}

void u_strncpy(char *dest_buf, char *buf, int size)
{
  strncpy(dest_buf, buf, size);
  dest_buf[size - 1] = '\0';
}



void json_test()
{
	char test_msg[50] = {0};
	char tx_msg[200] = {0};
	
	int react_time[3] = {100,101,102};
	JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
	char *serialized_string = NULL;
	
	json_object_set_string(root_object, "cmd", "PRODUCT_MODE");
	json_object_dotset_string(root_object, "data.state", "start");
	
	sprintf(test_msg,"[%d,%d,%d]", react_time[0], react_time[1], react_time[2]);
	
	json_object_dotset_value(root_object, "data.react_time", json_parse_string(test_msg));
	serialized_string = json_serialize_to_string(root_value);
    
	memcpy(tx_msg, serialized_string, strlen(serialized_string));
	
    json_free_serialized_string(serialized_string);
    json_value_free(root_value);
	
}
