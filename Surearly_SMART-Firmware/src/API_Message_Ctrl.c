#include "main.h"


void API_Rx_Message_Parsing()
{
  if(BLE_RX.api_resp_chk == API_RESP_OK)
  {
    if(BLE_TX.flag == RESET)
    {
      JSON_Value *rootValue;
      JSON_Object *rootObject;

      /* 초기화 */
      rootValue = json_parse_file(&BLE_RX.data[17]);      // JSON 파일을 읽어서 파싱
      if(rootValue == NULL){
        return;
      }
      rootObject = json_value_get_object(rootValue);    // JSON_Value에서 JSON_Object를 얻음
      if(rootObject == NULL){
        return;
      }
      
      char* json_cmd = (char*)json_object_get_string(rootObject, "cmd");
      
      // Command 일치 확인
      char cmd_check_buf[API_CMD_LEN] = {0};
      char cmd_check_len = strlen(json_cmd);
      char cmd_check_error = 0;

      if(cmd_check_len < API_CMD_LEN)
      {
        u_strncpy(cmd_check_buf, &BLE_RX.data[5], API_CMD_LEN);
        if(strstr(cmd_check_buf, json_cmd) != NULL)
        {
          if(cmd_check_len == (API_CMD_LEN - 1)){}
          else
          {
            for(char idx = cmd_check_len; idx < (API_CMD_LEN - 1); idx++){
              if(cmd_check_buf[idx] != '@'){
                cmd_check_error = 1;
              }
            }
          }
        }
        else{
          cmd_check_error = 1;
        }
      }
      else{
        cmd_check_error = 1;
      }
      //

      //수신된 메시지 길이 확인
      uint16_t rx_strlen = (uint16_t)String_To_Decimal(&BLE_RX.data[1], 4);
      
      char* begin = strchr(BLE_RX.data, '{');
      char* end = strrchr(BLE_RX.data, '#');
      
      uint16_t real_strlen = (uint16_t)(end - begin);
  
      if(rx_strlen == real_strlen)
      {
        if(!cmd_check_error)
        {   
          if(!strcmp(json_cmd,"DEV_INFO"))
          {
            char* stick_color = (char*)json_object_get_string(json_object_dotget_object(rootObject, "data"), "stick_color");
            int react_time = (int)json_object_get_number(json_object_dotget_object(rootObject, "data"), "react_time");
            int stick_insert_wait_time = (int)json_object_get_number(json_object_dotget_object(rootObject, "data"), "insert_time");

            if((1 <= stick_insert_wait_time) && (stick_insert_wait_time <= 60))
            {
              if((1 <= react_time) && (react_time <= 500))
              {
                if(strlen(stick_color) == API_STICK_COLOR_LEN)
                {
                  if((*stick_color == 'R') || (*stick_color == 'G') || (*stick_color == 'Y')|| (*stick_color == 'W'))
                  {
                    if(API_DATA.proc_step == MAIN_STICK_INSERT_WAITING)
                    {
                      API_DATA.stick_color = *stick_color;
                      API_DATA.react_time = react_time;
                      API_DATA.stick_insert_wait_time = stick_insert_wait_time;
                    }
                    API_Tx_Message(rootObject, DEV_INFO_CMD, 200);
                  }
                  else{
                    API_Tx_Message(rootObject, ERROR_CMD, 416);
                  }
                }
                else{
                  API_Tx_Message(rootObject, ERROR_CMD, 416);
                }
              }
              else{
                API_Tx_Message(rootObject, ERROR_CMD, 416);
              }
            }
            else{
              API_Tx_Message(rootObject, ERROR_CMD, 416);
            }
          }
          else if(!strcmp(json_cmd,"STAT_CHK"))
          {
            if((MAIN_STICK_INSERT_WAITING <= API_DATA.proc_step)&&(API_DATA.proc_step <= MAIN_RESULT_DISPLAY)){
              API_Tx_Message(rootObject, STAT_CHK_CMD, 200);
            }
          }
          else if(!strcmp(json_cmd,"DEV_RST"))
          {
            if((MAIN_STICK_INSERT_WAITING <= API_DATA.proc_step)&&(API_DATA.proc_step <= MAIN_RESULT_DISPLAY)){
              API_Tx_Message(rootObject, DEV_RST_CMD, 200);
              User_Mode_Variable_Init();
              Next_User_Mode_Process(MAIN_STICK_EJECTION);
            }
          }
          else if(!strcmp(json_cmd,"RSLT_DATA"))
          {
            uint8_t rslt_level = (uint8_t)json_object_get_number(json_object_dotget_object(rootObject, "data"), "rslt_level");
            API_DATA.rslt_level = rslt_level;
            API_DATA.rslt_level_recv_chk = SET;
            API_Tx_Message(rootObject, RSLT_DATA_CMD, 200);
          }
          else if(!strcmp(json_cmd,"PRODUCT_MODE"))
          {
            char* state_str = (char*)json_object_get_string(json_object_dotget_object(rootObject, "data"), "state");

            if(!strcmp(state_str,"start"))
            {
              device_mode_select = DEV_PRODUCTION_MODE;
              Production_Mode_Next_Process(PRODUCT_MODE_START, START);
              u_strncpy(API_DATA.product_mode_state, state_str, sizeof(API_DATA.product_mode_state));
              API_Tx_Message(rootObject, PRODUCT_MODE_CMD, 200); //Production_Mode_Start()에서 응답 전송
            }
            else if(!strcmp(state_str,"end"))
            {
              Production_Mode_Next_Process(PRODUCT_MODE_END, START);
              u_strncpy(API_DATA.product_mode_state, state_str, sizeof(API_DATA.product_mode_state));
              API_Tx_Message(rootObject, PRODUCT_MODE_CMD, 200); //Production_Mode_End()에서 응답 전송
            }
            else{
              API_Tx_Message(rootObject, ERROR_CMD, 416);
            }              
          }
          else if(!strcmp(json_cmd,"CALIB_STAT"))
          {
            char* calib_stat_str = (char*)json_object_get_string(json_object_dotget_object(rootObject, "data"), "test_state");
            if(strlen(calib_stat_str) < API_CALIB_STATE_LEN)
            {
              if(!strcmp(calib_stat_str,"start"))
              {
                Production_Mode_Next_Process(PRODUCT_CALIB_START, START);
                API_Tx_Message(rootObject, CALIB_STAT_CMD, 200);
              }
              else if(!strcmp(calib_stat_str,"state"))
              {
                Production_Calibration_Stick_Check();
                API_Tx_Message(rootObject, CALIB_STAT_CMD, 200);
              }
              else{
                API_Tx_Message(rootObject, ERROR_CMD, 416);
              }
            }
            else{
              API_Tx_Message(rootObject, ERROR_CMD, 416);
            }
          }
          else if(!strcmp(json_cmd,"CALIB_RSLT"))//
          {
            API_Tx_Message(rootObject, CALIB_RSLT_CMD, 200);
          }
          else if(!strcmp(json_cmd,"BLE_NAME"))//
          {
            const char* ble_name_setup_state = json_object_get_string(json_object_dotget_object(rootObject, "data"), "setup_state");
            const char* ble_name_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "name");

            if((!strcmp(ble_name_setup_state,"start")) || (!strcmp(ble_name_setup_state,"pass")))
            {
              u_strncpy(API_DATA.ble_name_setup_state, (char*)ble_name_setup_state, sizeof(API_DATA.ble_name_setup_state));
              u_strncpy(API_DATA.ble_name_str, (char*)ble_name_str, sizeof(API_DATA.ble_name_str));
              API_Tx_Message(rootObject, BLE_NAME_CMD, 200);
            }
            else{
              API_Tx_Message(rootObject, ERROR_CMD, 416);
            }
          }
          else if(!strcmp(json_cmd,"RAW_DATA"))//
          {
            char* test_stat_str = (char*)json_object_get_string(json_object_dotget_object(rootObject, "data"), "test_state");
            if(strlen(test_stat_str) < API_RAW_DATA_TEST_STATE_LEN)
            {
              if(!strcmp(test_stat_str,"start"))
              {
                API_Key_Value_Setting(&API_DATA, RAW_DATA_STATE_KEY, NONE_VAL);
                Production_Mode_Next_Process(PRODUCT_READ_RAWDATA_START, START);
                API_Tx_Message(rootObject, RAW_DATA_CMD, 200);
              }
              else if(!strcmp(test_stat_str,"state"))
              {
                API_Tx_Message(rootObject, RAW_DATA_CMD, 200);
              }
              else{
                API_Tx_Message(rootObject, ERROR_CMD, 416);
              }
            }
            else{
              API_Tx_Message(rootObject, ERROR_CMD, 416);
            }
          }
          else if(!strcmp(json_cmd,"FW_UPD"))
          {
            const char* fw_upd_password_str = json_object_get_string(json_object_dotget_object(rootObject, "data"), "password");
            
            if(!strcmp(fw_upd_password_str, FW_UPD_PASSWORD))
            {
              API_Tx_Message(rootObject, FW_UPD_CMD, 200);
              fw_update_mode_execution = 1;
            }
            else{
              API_Tx_Message(rootObject, ERROR_CMD, 416);
            }
          }
          else if(!strcmp(json_cmd,"READ_PTR"))
          {
            if(PRODUCT_MODE_PTR.status == READ_PTR_END){
              PRODUCT_MODE_PTR.status = READ_PTR_NONE;
            }
            uint8_t optics_channel = (uint8_t)json_object_get_number(json_object_dotget_object(rootObject, "data"), "optics_channel");
            uint8_t dac_interval = (uint8_t)json_object_get_number(json_object_dotget_object(rootObject, "data"), "dac_interval");
            
            if(optics_channel <= 6)
            {
              if(dac_interval != 0)
              {
                if(PRODUCT_MODE_PTR.OneTimeProcess == 0)
                {
                    PRODUCT_MODE_PTR.OpticsChannel = optics_channel;
                    PRODUCT_MODE_PTR.vDacInterval = dac_interval;
                    PRODUCT_MODE_PTR.status = READ_PTR_NONE;
                    
                    Production_Mode_Next_Process(PRODUCT_READ_PTR_START, START);
                }
                else
                {
                  uint8_t manager_status = (uint8_t)json_object_get_number(json_object_dotget_object(rootObject, "data"), "status");
                  
                  if(manager_status == READ_PTR_DISPLAY_COMPLETE)
                  {
                    if(PRODUCT_MODE_PTR.status == READ_PTR_READ_COMPLETE){
                      PRODUCT_MODE_PTR.status = READ_PTR_NONE;
                    }
                  }
                  else if(manager_status == READ_PTR_END)
                  {
                    PRODUCT_MODE_PTR.status = READ_PTR_END;
                  }
                }
                
                if(PRODUCT_MODE_PTR.status == READ_PTR_END){
                  PRODUCT_MODE_PTR.OneTimeProcess = 0;
                  Production_Mode_Next_Process(PRODUCT_MODE_NONE, END);
                }
                else Production_Mode_Next_Process(PRODUCT_READ_PTR_START, START);
                
                API_Tx_Message(rootObject, READ_PTR_CMD, 200);
              }
              else API_Tx_Message(rootObject, ERROR_CMD, 416);
            }
            else API_Tx_Message(rootObject, ERROR_CMD, 416);
          }
          else
          {
            //Error Handle
            //Unidentified Command
            API_Tx_Message(rootObject, ERROR_CMD, 404);
          }
        }
        else{
          cmd_check_error = 0;
          API_Tx_Message(rootObject, ERROR_CMD, 404);
        }
      }
      else
      {
        if(!strcmp(BLE_RX.data,"|adminmode#"))
        {
          Run_Adminmode();
        }
        else
        {
          //Error Handle
          //Length Mismatch
          API_Tx_Message(rootObject, ERROR_CMD, 411);
        }
      }
      json_object_clear(rootObject);
      json_value_free(rootValue);    // JSON_Value에 할당된 동적 메모리 해제
    }
    else{}

    memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
    BLE_RX.api_resp_chk = API_RESP_NONE;
    
    // Stack overflow 방지
    USART_Cmd(BLE_USART, ENABLE);
  }
  else{}
}
void API_Tx_Message(const JSON_Object *object, char cmd, uint16_t code)
{
  char json_cmd[API_CMD_LEN] = {0};
  char cmd_strlen = 0;
  int i = 0;
  int idx = 0;

  if(BLE_TX.flag == RESET)
  {
    memset(BLE_TX.data, 0, sizeof(BLE_TX.data));

    if(code == 200) //Normal
    {
        if(cmd == DEV_INFO_CMD){
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"DEV_INFO\",\"code\":200,\"data\":{\"stick_color\":\"%c\",\"react_time\":%d,\"insert_time\":%d,\"dev_id\":\"%s\",\"fw_ver\":\"%s\"}}#\r\n", API_DATA.stick_color, API_DATA.react_time, API_DATA.stick_insert_wait_time, mcu_uid, FW_VER);
        }
        else if(cmd == STAT_CHK_CMD){
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"STAT_CHK\",\"code\":200,\"data\":{\"proc_step\":%d,\"test_state\":\"%s\",\"stick_color\":\"%c\",\"stick_state\":\"%s\",\"fin_time\":%d,\"react_time\":%d,\"react_chk\":\"%s\",\"rslt_rawdata\":[%0.1f,%0.1f,%0.1f]}}#\r\n",
          API_DATA.proc_step, API_DATA.test_state, API_DATA.stick_color_detect, API_DATA.stick_state, API_DATA.fin_time, API_DATA.cur_react_time, API_DATA.react_chk, API_DATA.rslt_rawdata[0], API_DATA.rslt_rawdata[1], API_DATA.rslt_rawdata[2]);
        }
        else if(cmd == DEV_RST_CMD){
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"DEV_RST\",\"code\":200}#\r\n");
        }
        else if(cmd == RSLT_DATA_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"RSLT_DATA\",\"code\":200,\"data\":{\"rslt_level\":%d}}#\r\n", API_DATA.rslt_level);
        }
        else if(cmd == PRODUCT_MODE_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"PRODUCT_MODE\",\"code\":200,\"data\":{\"state\":\"%s\",\"dev_id\":\"%s\",\"fw_ver\":\"%s\"}}#\r\n", API_DATA.product_mode_state, mcu_uid, FW_VER);
        }
        else if(cmd == CALIB_STAT_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"CALIB_STAT\",\"code\":200,\"data\":{\"stick_state\":\"%s\",\"proc_state\":\"%s\"}}#\r\n", API_DATA.calib_stick_stat, API_DATA.calib_proc_stat);
        }
        else if(cmd == CALIB_RSLT_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"CALIB_RSLT\",\"code\":200,\"data\":{\"dac_data\":[%d,%d,%d,%d,%d,%d,%d],\"ptr_data\":[%u,%u,%u,%u,%u,%u,%u]}}#\r\n", 
          MEM_BANK_1.CALIBRATION_DATA[1], MEM_BANK_1.CALIBRATION_DATA[2], MEM_BANK_1.CALIBRATION_DATA[3], MEM_BANK_1.CALIBRATION_DATA[4], MEM_BANK_1.CALIBRATION_DATA[5], MEM_BANK_1.CALIBRATION_DATA[6], MEM_BANK_1.CALIBRATION_DATA[7],
          MEM_BANK_1.INITIAL_BAND_SENSING_DATA[1], MEM_BANK_1.INITIAL_BAND_SENSING_DATA[2], MEM_BANK_1.INITIAL_BAND_SENSING_DATA[3], MEM_BANK_1.INITIAL_BAND_SENSING_DATA[4], MEM_BANK_1.INITIAL_BAND_SENSING_DATA[5], MEM_BANK_1.INITIAL_COLOR_SENSING_DATA[0], MEM_BANK_1.INITIAL_COLOR_SENSING_DATA[1]);
        }
        else if(cmd == BLE_NAME_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"BLE_NAME\",\"code\":200,\"data\":{\"setup_state\":\"%s\",\"name\":\"%s\"}}#\n", API_DATA.ble_name_setup_state, API_DATA.ble_name_str);
        }
        else if(cmd == RAW_DATA_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"RAW_DATA\",\"code\":200,\"data\":{\"state\":\"%s\",\"ch_rawdata\":[%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f]}}#\r\n", 
          API_DATA.raw_data_state, PRODUCT_MODE_VAL.ch_ratio_data[1], PRODUCT_MODE_VAL.ch_ratio_data[2], PRODUCT_MODE_VAL.ch_ratio_data[3], PRODUCT_MODE_VAL.ch_ratio_data[4], PRODUCT_MODE_VAL.ch_ratio_data[5], PRODUCT_MODE_VAL.ch_ratio_data[6], PRODUCT_MODE_VAL.ch_ratio_data[7]);
        }
        else if(cmd == FW_UPD_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"FW_UPD\",\"code\":200,\"data\":{\"password\":\"%s\"}}#\n", FW_UPD_PASSWORD);
        }
        else if(cmd == READ_PTR_CMD)
        {
          sprintf(&BLE_TX.data[17],"{\"cmd\":\"READ_PTR\",\"code\":200,\"data\":{\"status\":%d,\"dac_data\":[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d],\"ptr_data\":[%u,%u,%u,%u,%u,%u,%u,%u,%u,%u]}}#\r\n", 
           PRODUCT_MODE_PTR.status
            ,PRODUCT_MODE_PTR.DacData[0],PRODUCT_MODE_PTR.DacData[1],PRODUCT_MODE_PTR.DacData[2],PRODUCT_MODE_PTR.DacData[3],PRODUCT_MODE_PTR.DacData[4]
             ,PRODUCT_MODE_PTR.DacData[5],PRODUCT_MODE_PTR.DacData[6],PRODUCT_MODE_PTR.DacData[7],PRODUCT_MODE_PTR.DacData[8],PRODUCT_MODE_PTR.DacData[9]
              ,PRODUCT_MODE_PTR.PtrData[0],PRODUCT_MODE_PTR.PtrData[1],PRODUCT_MODE_PTR.PtrData[2],PRODUCT_MODE_PTR.PtrData[3],PRODUCT_MODE_PTR.PtrData[4]
               ,PRODUCT_MODE_PTR.PtrData[5],PRODUCT_MODE_PTR.PtrData[6],PRODUCT_MODE_PTR.PtrData[7],PRODUCT_MODE_PTR.PtrData[8],PRODUCT_MODE_PTR.PtrData[9]);
        }
        else{}

        cmd_strlen = strlen(json_object_get_string(object, "cmd"));
        if(cmd_strlen < API_CMD_LEN){
            u_strncpy(json_cmd, (char*)json_object_get_string(object, "cmd"), sizeof(json_cmd));
        }
        else{
            cmd_strlen = 0;
        }
    }
    else //Error: Length mismatch (411), Unidentified Command (404), Incorrect Value (416)
    {
        u_strncpy(json_cmd, "ERROR", sizeof(json_cmd));
        cmd_strlen = strlen(json_cmd);
        sprintf(&BLE_TX.data[17],"{\"cmd\":\"ERROR\",\"code\":%d}#\r\n",code);
    }

    char* begin = strchr(&BLE_TX.data[17], '{');
    char* end = strrchr(&BLE_TX.data[17], '#');  
    uint16_t data_strlen = (uint16_t)(end - begin);

    for(idx = 0; idx <= 5; idx++)
    {
        if(idx == 0){
          BLE_TX.data[idx] = START_STR;
        }
        else if(idx <= 4){
          BLE_TX.data[idx] = ((data_strlen % (uint16_t)round(pow(10.0, (double)(5-idx)))) / (uint16_t)round(pow(10.0, (double)(4-idx)))) + 0x30;
        }
        else if(idx == 5)
        {
          for(i = idx; i < idx+cmd_strlen; i++){
              BLE_TX.data[i] = json_cmd[i-idx];
          }
          for(i = idx+cmd_strlen; i < idx+MSG_NAME; i++){
              BLE_TX.data[i] = '@';        
          }
        }      
    }
    BLE_TX.index = 0;
    BLE_TX.data_cnt = 0;
    BLE_TX.flag = SET;
  }
  else{}
}

//API 중 String 처리에 해당 함수 사용
//API Data-key: test_state, content, stick_state, react_chk
void API_Key_Value_Setting(API_DATA_TypeDef* p_data, char key, char value)
{
  switch(key)
  {
    case TEST_STATE_KEY :
        memset(p_data->test_state, 0, sizeof(p_data->test_state));
        if(value == NONE_VAL){
          u_strncpy(p_data->test_state, "none", sizeof(p_data->test_state));
        }
        else if(value == START_VAL){
          u_strncpy(p_data->test_state, "start", sizeof(p_data->test_state));
        }
        else if(value == END_VAL){
          u_strncpy(p_data->test_state, "end", sizeof(p_data->test_state));
        }
        break;
    case STICK_STATE_KEY :
        memset(p_data->stick_state, 0, sizeof(p_data->stick_state));
        if(value == NONE_VAL){
          u_strncpy(p_data->stick_state, "none", sizeof(p_data->stick_state));
        }
        else if(value == NORM_VAL){
          u_strncpy(p_data->stick_state, "norm", sizeof(p_data->stick_state));
        }
        else if(value == ERR_CONTENT_VAL){
          u_strncpy(p_data->stick_state, "err_content", sizeof(p_data->stick_state));
        }
        else if(value == ERR_USED_VAL){
          u_strncpy(p_data->stick_state, "err_used", sizeof(p_data->stick_state));
        }
        else if(value == ERR_EJECT_VAL){
          u_strncpy(p_data->stick_state, "err_eject", sizeof(p_data->stick_state));
        }
        else if(value == ERR_CTRL_VAL){
          u_strncpy(p_data->stick_state, "err_ctrl", sizeof(p_data->stick_state));
        }
        break;
    case REACT_CHK_KEY :
        memset(p_data->react_chk, 0, sizeof(p_data->react_chk));
        if(value == NONE_VAL){
          u_strncpy(p_data->react_chk, "none", sizeof(p_data->react_chk));
        }
        else if(value == DOING_VAL){
          u_strncpy(p_data->react_chk, "doing", sizeof(p_data->react_chk));
        }
        else if(value == COMPLETE_VAL){
          u_strncpy(p_data->react_chk, "complete", sizeof(p_data->react_chk));
        }
        break;
    case CALIB_STICK_STATE_KEY :
        memset(p_data->calib_stick_stat, 0, sizeof(p_data->calib_stick_stat));
        if(value == CALIB_STICK_EJECTED_VAL){
          u_strncpy(p_data->calib_stick_stat, "eject", sizeof(p_data->calib_stick_stat));
        }
        else if(value == CALIB_STICK_INSERTED_VAL){
          u_strncpy(p_data->calib_stick_stat, "insert", sizeof(p_data->calib_stick_stat));
        }
        break;
    case CALIB_PROC_STATE_KEY :
        memset(p_data->calib_proc_stat, 0, sizeof(p_data->calib_proc_stat));
        if(value == NONE_VAL){
          u_strncpy(p_data->calib_proc_stat, "none", sizeof(p_data->calib_proc_stat));
        }
        else if(value == DOING_VAL){
          u_strncpy(p_data->calib_proc_stat, "doing", sizeof(p_data->calib_proc_stat));
        }
        else if(value == COMPLETE_VAL){
          u_strncpy(p_data->calib_proc_stat, "complete", sizeof(p_data->calib_proc_stat));
        }
        else if(value == FAIL_VAL){
          u_strncpy(p_data->calib_proc_stat, "fail", sizeof(p_data->calib_proc_stat));
        }
        break;
    case RAW_DATA_STATE_KEY :
        memset(p_data->raw_data_state, 0, sizeof(p_data->raw_data_state));
        if(value == NONE_VAL){
          u_strncpy(p_data->raw_data_state, "none", sizeof(p_data->raw_data_state));
        }
        else if(value == DOING_VAL){
          u_strncpy(p_data->raw_data_state, "doing", sizeof(p_data->raw_data_state));
        }
        else if(value == COMPLETE_VAL){
          u_strncpy(p_data->raw_data_state, "complete", sizeof(p_data->raw_data_state));
        }
        break;        
  }
}

void Get_Channel_Ratio_And_Raw_data(API_DATA_TypeDef *P_API_DATA)
{
  uint8_t i = 0;
  PTR_Sensing_Bandline(recent_band_sensing_data);
  
  for(i = 1; i <= 5; i++){
    //P_API_DATA->channel_ratio_data[i] = ((float)recent_band_sensing_data[i] / (float)initial_band_sensing_data[i]) * 100.0;
    P_API_DATA->channel_ratio_data[i] = ((float)recent_band_sensing_data[i] / (float)MEM_BANK_1.AGING_BAND_SENSING_DATA[i]) * 100.0;
  }
	//측정 결과 계산 Raw data
  P_API_DATA->rslt_rawdata[0] = (P_API_DATA->channel_ratio_data[2] / P_API_DATA->channel_ratio_data[1]) * 1000.0; //T1 line
  P_API_DATA->rslt_rawdata[1] = (P_API_DATA->channel_ratio_data[2] / P_API_DATA->channel_ratio_data[3]) * 1000.0; //T2 line
  P_API_DATA->rslt_rawdata[2] = (P_API_DATA->channel_ratio_data[4] / P_API_DATA->channel_ratio_data[5]) * 1000.0; //T3 line
  //

  //-nan, inf 확인
  for(i = 0; i < 3; i++)
  {
    if(isnan(P_API_DATA->rslt_rawdata[i]) != 0){ //NAN?
      P_API_DATA->rslt_rawdata[i] = -9999.9;
    }
    else
    {
      if(isinf(P_API_DATA->rslt_rawdata[i]) != 0){ //Inf?
        P_API_DATA->rslt_rawdata[i] = 9999.9;
      }
      else
      {
        if(P_API_DATA->rslt_rawdata[i] >= 8888.8){ //Limit maximum value
          P_API_DATA->rslt_rawdata[i] = 8888.8;
        }
        else if(P_API_DATA->rslt_rawdata[i] <= -8888.8){ //Limit minimum value
          P_API_DATA->rslt_rawdata[i] = -8888.8;
        }
        else{} //Normal value
      }
    }
  }
}

void API_Key_Value_Init(API_DATA_TypeDef* p_data)
{
  p_data->stick_color = 0;
  p_data->stick_color_detect = 'N';
  p_data->react_time = 0;
  p_data->stick_insert_wait_time = 0;
  p_data->proc_step = 0;
  p_data->fin_time = 0;
  p_data->cur_react_time = 0;
  p_data->rslt_level = 0;
  p_data->rslt_level_recv_chk = 0;
  memset(p_data->rslt_rawdata, 0, sizeof(p_data->rslt_rawdata)); //KEY: rslt_rawdata, VALUE: 0
  memset(p_data->channel_ratio_data, 0, sizeof(p_data->channel_ratio_data)); //KEY: channel_ratio_data, VALUE: 0
  memset(p_data->ble_name_setup_state, 0, sizeof(p_data->ble_name_setup_state));
  memset(p_data->ble_name_str, 0, sizeof(p_data->ble_name_str));
  API_Key_Value_Setting(&API_DATA, TEST_STATE_KEY, NONE_VAL); //KEY: test_state, VALUE: none
  API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, NONE_VAL); //KEY: stick_state, VALUE: none
  API_Key_Value_Setting(&API_DATA, REACT_CHK_KEY, NONE_VAL); //KEY: react_chk, VALUE: none
}

