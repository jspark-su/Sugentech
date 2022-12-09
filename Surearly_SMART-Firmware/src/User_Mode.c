#include "main.h"

USER_MODE_PROCESS_TypeDef USER_MODE_PROCESS = {0,};
API_DATA_TypeDef  API_DATA = {0,};

USER_TIMER TIMER_COUNT_UP_1 = {0,};
USER_TIMER TIMER_COUNT_UP_2 = {0,};
USER_TIMER TIMER_COUNT_DOWN_1 = {0,};

volatile char device_mode_select = DEV_APP_CONNECTION_MODE; //system 작동 모드 선택 (main/demo/test mode)
char once_operation = 0;
char stick_error_check = 0;
int sample_react_step[5] = {0};
char cur_sample_reaction_progress_step = 0;
char pre_sample_reaction_progress_step = 0;
uint8_t check_sample_flow_process = 0;

static char stick_color_backup = 0;
static int react_time_backup = 0;
static int insert_time_backup = 0;

void User_Mode_Process()
{
  //if((USER_MODE_PROCESS.flag == START)&&(BLE_FLAG.connect_status == BLE_CONNECTED))
  if(USER_MODE_PROCESS.flag == START)
  {
    switch(USER_MODE_PROCESS.function_number)
    {
      case MAIN_APP_CONNECTION_WAITING :
        App_Connection_Waiting();
        break;
      case MAIN_STICK_INSERT_WAITING :
        Stick_Insert_Waiting();
        break;
      case MAIN_STICK_CHECK :
        Stick_Check();
        break;
      case MAIN_ERROR_STICK_CHECK :
        Error_Stick_Check();
        break;
      case MAIN_SAMPLE_LOAD_WAITING :
        Sample_Loading_Waiting();
        break;  
      case MAIN_SAMPLE_REACT_WAITING :
        Sample_React_Waiting();
        break;
      case MAIN_SAMPLE_ANALYZE :
        Sample_Analyze();
        break;
      case MAIN_RESULT_DISPLAY :
        Result_Display();
        break;
      case MAIN_STICK_EJECTION :
        Stick_Ejection();
        break;   
    }
  }
}

void App_Connection_Waiting()
{
  if(!once_operation)
  {
    once_operation = 1;
    API_Key_Value_Init(&API_DATA);
    API_DATA.proc_step = MAIN_APP_CONNECTION_WAITING;
    Timer_Count_Setup(&TIMER_COUNT_UP_1, 0, TIMER_COUNT_ENABLE); //Start App connection timer (period: 1 sec)
  }
  
  if(TIMER_COUNT_UP_1.count >= APP_CONNECTION_WATIT_TIME)
  {
    /* Error Event Handle*/
    User_Mode_Variable_Init();
    //LCD ICON 전체 3번 깜박임으로 자동 종료 알림
    LCD_Event_All_Icon_Blink(3, 500);
    POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
    /* ****************** */
  }
  else
  {
    Next_User_Mode_Process(MAIN_APP_CONNECTION_WAITING);
  }

  if(BLE_FLAG.connect_status == BLE_CONNECTED) //Complete App connection
  {
    User_Mode_Variable_Init();
    Next_User_Mode_Process(MAIN_STICK_INSERT_WAITING);
  }
}

void Stick_Insert_Waiting()
{
  if(!once_operation)
  {
    once_operation = 1;
    API_DATA.proc_step = MAIN_STICK_INSERT_WAITING;
    LCD_Display(INSERT, BLINK_ON);
    Timer_Count_Setup(&TIMER_COUNT_UP_1, 0, TIMER_COUNT_ENABLE); //Start stick insertion timer
    
    if((stick_color_backup) && (react_time_backup) && (insert_time_backup)) //이전 'DEV_INFO' API를 통해 stick_color, react_time 설정 완료 후 stick content/used error로 stick 재 삽입 시 이전 설정한 color, react time 복구
    {
      API_DATA.stick_color = stick_color_backup; API_DATA.react_time = react_time_backup; API_DATA.stick_insert_wait_time = insert_time_backup; //stick color, react time 복구
      stick_color_backup = 0; react_time_backup = 0; insert_time_backup = 0; //임시 stick color, react time 변수 초기화
    }
  }
  
  if(TIMER_COUNT_UP_1.count >= STICK_INSERT_WATIT_TIME) //샘플을 2분내 점적하지 않았을 경우 자동 전원 Off  
  {
    /* Error Event Handle*/
    User_Mode_Variable_Init();
    //LCD ICON 전체 3번 깜박임으로 자동 종료 알림
    LCD_Event_All_Icon_Blink(3, 500);
    POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
    /* ****************** */
  }
  else{
    Next_User_Mode_Process(MAIN_STICK_INSERT_WAITING);
  }
  
  if((API_DATA.stick_color) && (API_DATA.react_time) && (API_DATA.stick_insert_wait_time)) //'DEV_INFO' API 수신되어 stick_color, react_time, stick_insert_wait_time 설정 완료 시
  {
    if(STRIP_CHECK_SWITCH.event == STICK_INSERTED) //Stick 삽입 완료
    {
      LCD_Display(INSERT, BLINK_OFF); LCD_Display(INSERT, ICON_ON);
      API_Key_Value_Setting(&API_DATA, TEST_STATE_KEY, START_VAL); //KEY: Test_state, VALUE: Start
      //delay_ms(3000);
      delay_ms(((uint32_t)API_DATA.stick_insert_wait_time) * 1000);
      OP_Amp_Enable();
      Stick_Insertion_Count();
      User_Mode_Variable_Init();
      Next_User_Mode_Process(MAIN_STICK_CHECK); //다음 단계 진행
    }
  }
  else{}
}

void Stick_Check()
{
  API_DATA.proc_step = MAIN_STICK_CHECK;
  char i = 0;
  char stick_status_check = 0;
  float ptr_ratio = 0;
  
  API_DATA.stick_color_detect = Get_Stick_Color(); //삽입된 Stick의 color 확인
  
  /* Stick color 일치 여부 확인 */
  if(API_DATA.stick_color_detect == API_DATA.stick_color)
  {
    PTR_Sensing_Bandline(recent_band_sensing_data); //현재 삽입된 Stick의 PTR data sensing
    //Aging PTR data와 현재 삽입 된 Stick의 PTR data 비교 진행
    //DAC data: Factory dac data 사용
    for(i = 1; i <= 5; i++)
    {
      ptr_ratio = (float)recent_band_sensing_data[i] / (float)MEM_BANK_1.AGING_BAND_SENSING_DATA[i];
      //Aging PTR data와 Ratio를 구했을 때 80% 미만 -> 불량
      if(ptr_ratio < 0.8){ //Yes
        stick_status_check = ERROR_COMP_AGING_PTR_DATA;
      }
      else{} //No
    }

    if(stick_status_check == ERROR_COMP_AGING_PTR_DATA) //Aging PTR data 비교 Error 처리
    {
      API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, ERR_USED_VAL); //KEY: stick_state, VALUE: err_used
      Next_User_Mode_Process(MAIN_ERROR_STICK_CHECK);
    }
    else //Stick Check OK!
    {
      API_DATA.fin_time = API_DATA.react_time; //App으로부터 수신된 react time 셋팅
      API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, NORM_VAL); //KEY: stick_state, VALUE: norm
      /* LED calibation 진행 */
      Auto_Calibration_Bandline(); //현재 삽입된 Stick의 1~5 ch LED calibration 진행
      calibration_check = CALIBRATION_NONE;
      PTR_Sensing_Bandline(recent_band_sensing_data); // 1~5 ch PTR data sensing
      /* ****************** */
      //Update aging PTR data
      memcpy(MEM_BANK_1.AGING_BAND_SENSING_DATA, recent_band_sensing_data, sizeof(MEM_BANK_1.AGING_BAND_SENSING_DATA)); //현재 삽입 Stick의 PTR data로 Aging data update
      memcpy(MEM_BANK_1.CALIBRATION_DATA, calibration_data, sizeof(MEM_BANK_1.CALIBRATION_DATA)); //삽입한 Stick의 Calibation data (DAC)로 MEM_BANK_1 영역 update
      Next_User_Mode_Process(MAIN_SAMPLE_LOAD_WAITING);
    }
  }
  else //Stick color 인식 불가 or 삽입된 stick과 App으로부터 수신된 stick color 일치하지 않음
  {
    API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, ERR_CONTENT_VAL); //KEY: stick_state, VALUE: err_content
    Next_User_Mode_Process(MAIN_ERROR_STICK_CHECK);
  }

  User_Mode_Variable_Init();
}

void Sample_Loading_Waiting()
{
  uint8_t i = 0;
  
  if(!once_operation)
  {
    once_operation = 1;
    API_DATA.proc_step = MAIN_SAMPLE_LOAD_WAITING;
    LCD_Display(DROP, BLINK_ON);
    Timer_Count_Setup(&TIMER_COUNT_UP_1, 0, TIMER_COUNT_ENABLE); //Start sample loading wait timer (period: 1 sec)
  }
  
  if(TIMER_COUNT_UP_1.count >= SAMPLE_LOADING_WATIT_TIME) //샘플을 5분내 점적하지 않았을 경우 자동 전원 Off  
  {
    /* Error Event Handle*/
    User_Mode_Variable_Init();
    //LCD ICON 전체 3번 깜박임으로 자동 종료 알림
    LCD_Event_All_Icon_Blink(3, 500);
    POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
    //내용 추가 필요
    /* ****************** */
  }
  else{
    Next_User_Mode_Process(MAIN_SAMPLE_LOAD_WAITING);
  }

  //Stick 임의 분리 상태 확인
  Error_Stick_Insert_Check();

  if(!stick_error_check) //Stick에 문제가 없다면
  {
    Get_Channel_Ratio_And_Raw_data(&API_DATA); //Get 1 ~ 5 channel ratio, T1 ~ 3 rawdata

    // line 1 만 DIPPING_ALARM_RATIO 이하 일때 (line 2~5는 발색 X)
    if((API_DATA.channel_ratio_data[1] < DIPPING_ALARM_RATIO) && (API_DATA.channel_ratio_data[2] >= BAND_NOR_RATIO) && (API_DATA.channel_ratio_data[3] >= BAND_NOR_RATIO) && (API_DATA.channel_ratio_data[4] >= BAND_NOR_RATIO) && (API_DATA.channel_ratio_data[5] >= BAND_NOR_RATIO))
    {
      LCD_Display(DROP, BLINK_OFF); LCD_Display(DROP, ICON_ON);
      LCD_Display(SQUARE_1, ICON_OFF); LCD_Display(SQUARE_2, ICON_OFF);
      LCD_Display(SQUARE_3, ICON_OFF); LCD_Display(SQUARE_4, ICON_OFF);
      delay_ms(250);
      for(i = 0; i < 5; i++) //Dipping alram
      {
        LCD_Display(SQUARE_1, ICON_ON); LCD_Display(SQUARE_2, ICON_ON);
        LCD_Display(SQUARE_3, ICON_ON); LCD_Display(SQUARE_4, ICON_ON);
        delay_ms(250);
        LCD_Display(SQUARE_1, ICON_OFF); LCD_Display(SQUARE_2, ICON_OFF);
        LCD_Display(SQUARE_3, ICON_OFF); LCD_Display(SQUARE_4, ICON_OFF);
        delay_ms(250);
      }
      User_Mode_Variable_Init();
      Next_User_Mode_Process(MAIN_SAMPLE_REACT_WAITING);
    }
  }
  else //stick 강제 분리 시
  {
    API_DATA.rslt_rawdata[0] = 0;
    API_DATA.rslt_rawdata[1] = 0;
    API_DATA.rslt_rawdata[2] = 0;
  }
}

void Sample_React_Waiting()
{
  if(!once_operation)
  {
    once_operation = 1;
    API_DATA.proc_step = MAIN_SAMPLE_REACT_WAITING;
    sample_react_step[0] = API_DATA.react_time - 1;
    sample_react_step[1] = (int)((API_DATA.react_time / 4) * 3);
    sample_react_step[2] = (int)((API_DATA.react_time / 4) * 2);
    sample_react_step[3] = (int)(API_DATA.react_time / 4);
    sample_react_step[4] = 0;
    Timer_Count_Setup(&TIMER_COUNT_DOWN_1, API_DATA.react_time, TIMER_COUNT_ENABLE); //Set sample react timer (stick contents 마다 다름) 
  }

  //Stick 임의 분리 상태 확인
  Error_Stick_Insert_Check();
  
  if((check_sample_flow_process == START) && (!stick_error_check)) //Stick 문제 없을 경우, 1초 간격 stick sample 전개 상태 확인 (Update strip channel ratio data)
  {
    //1 ~ 5 Channel ratio, T1 ~ 3 rawdata
    Get_Channel_Ratio_And_Raw_data(&API_DATA);
    check_sample_flow_process = END;
  }
  else if(stick_error_check) //stick 강제 분리 시
  {
    API_DATA.rslt_rawdata[0] = 0;
    API_DATA.rslt_rawdata[1] = 0;
    API_DATA.rslt_rawdata[2] = 0;
  }

  if(cur_sample_reaction_progress_step != pre_sample_reaction_progress_step) //Sample 반응 대기 단계에 맞춰 LCD icon을 한번만 update하기 위해 pre와 cur flag를 비교 함. 
  {                                                                          //cur_sample_reaction_progress_step의 값은 timer2에서 update 함.
    pre_sample_reaction_progress_step = cur_sample_reaction_progress_step;

    if(cur_sample_reaction_progress_step == 1)
    {
      LCD_Display(SQUARE_1, BLINK_ON);
    }
    else if(cur_sample_reaction_progress_step == 2)
    {
      LCD_Display(SQUARE_1, BLINK_OFF); LCD_Display(SQUARE_1, ICON_ON);
      LCD_Display(SQUARE_2, BLINK_ON);
    }
    else if(cur_sample_reaction_progress_step == 3)
    {
      LCD_Display(SQUARE_2, BLINK_OFF); LCD_Display(SQUARE_2, ICON_ON);
      LCD_Display(SQUARE_3, BLINK_ON);
    }
    else if(cur_sample_reaction_progress_step == 4)
    {
      LCD_Display(SQUARE_3, BLINK_OFF); LCD_Display(SQUARE_3, ICON_ON);
      LCD_Display(SQUARE_4, BLINK_ON);
    }

    if(cur_sample_reaction_progress_step == 5) //cur_sample_reaction_progress_step == 5는 SMAPLE_ANALYZE 단계 진행
    {
      LCD_Display(SQUARE_4, BLINK_OFF); LCD_Display(SQUARE_4, ICON_ON);
      User_Mode_Variable_Init();
      Next_User_Mode_Process(MAIN_SAMPLE_ANALYZE);
    }
    else{ //cur_sample_reaction_progress_step < 5는 SAMPLE_REACT_WAITING 단계 반복 진행, 
      Next_User_Mode_Process(MAIN_SAMPLE_REACT_WAITING);
    }
  }
  else{ //cur_sample_reaction_progress_step == 5는 SMAPLE_ANALYZE 단계 진행
    Next_User_Mode_Process(MAIN_SAMPLE_REACT_WAITING);
  }
}

void Sample_Analyze()
{
  API_DATA.proc_step = MAIN_SAMPLE_ANALYZE;
  API_Key_Value_Setting(&API_DATA, REACT_CHK_KEY, COMPLETE_VAL); //KEY: react_chk, VALUE: complete
  
  Get_Channel_Ratio_And_Raw_data(&API_DATA); //1 ~ 5 Channel ratio, T1 ~ 3 rawdata

  //Stick 삽입 상태에 문제가 없을 경우
  if(!stick_error_check)
  {
    MEM_BANK_1.ACCESS_ADDR_1 = SET;
    MEM_BANK_1.DEV_USAGE_CNT += 1; // 기기 사용 횟수 update
    
    //Save_Memory_Bank_Byte_Program(MEMORY_ADDR_1, &MEM_BANK_1); // 업데이트된 Data/Parameter -> Memory address 1 저장
    //memory_save_process = SET;
    Save_Memory_Bank_Block_Program(&MEM_BANK_1);
  }
  else{} //Stick 삽입 상태에 문제 있을 경우 data 저장 안함

  OP_Amp_Disable();
  LCD_Result_Display_Off();
  User_Mode_Variable_Init();
  Next_User_Mode_Process(MAIN_RESULT_DISPLAY);
}

void Result_Display()
{
  if(!once_operation)
  {
    once_operation = 1;
    API_DATA.proc_step = MAIN_RESULT_DISPLAY;
    LCD_Display(DROP, ICON_OFF);
    LCD_Display(INSERT, BLINK_OFF); LCD_Display(INSERT, ICON_ON);
    Timer_Count_Setup(&TIMER_COUNT_UP_1, 0, TIMER_COUNT_ENABLE); //Start result display timer
  }

  if(API_DATA.rslt_level_recv_chk == SET)
  {
    API_DATA.rslt_level_recv_chk = RESET;

    if(API_DATA.rslt_level == RESULT_LEVEL_0){
      LCD_Result_Icon_Display(MINUS_ICON);
    }
    else if(API_DATA.rslt_level == RESULT_LEVEL_1){
      LCD_Result_Icon_Display(PLUS_1_ICON);
    }
    else if(API_DATA.rslt_level == RESULT_LEVEL_2){
      LCD_Result_Icon_Display(PLUS_2_ICON);
    }
    else if(API_DATA.rslt_level == RESULT_LEVEL_3){
      LCD_Result_Icon_Display(PLUS_3_ICON);
    }
    else if(API_DATA.rslt_level == RESULT_FAIL){
      LCD_Result_Icon_Display(ERROR_ICON);
      API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, ERR_CTRL_VAL);
    }
    API_Key_Value_Setting(&API_DATA, TEST_STATE_KEY, END_VAL);
  }

  if(TIMER_COUNT_UP_1.count >= RESULT_DISPLAY_TIME) //5분 경과 시 Power off
  {
    User_Mode_Variable_Init();
    //Device Power Off 조건
    //사용자 입력 (Event)이 5분간 없을 경우 LCD ICON 전체 3번 깜박임으로 자동 종료 알림
    LCD_Event_All_Icon_Blink(3, 500);
    POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
  }
  Next_User_Mode_Process(MAIN_RESULT_DISPLAY);
}

void Stick_Ejection()
{
  if(!once_operation)
  {
    once_operation = 1;
    LCD_Lower_Icon_Off();//INSERT, DROP, BOOK -> BLINK_OFF, ICON_OFF
    LCD_Result_Display_Off();
    LCD_Display(EJECT, BLINK_ON);
    Timer_Count_Setup(&TIMER_COUNT_UP_1, 0, TIMER_COUNT_ENABLE); //Start stick ejection timer
  }

  if(TIMER_COUNT_UP_1.count >= SITCK_EJECTION_TIME) //2분내 stick을 분리하지 않았다면 
  {
    /* Error Event Handle*/
    User_Mode_Variable_Init();
    //LCD ICON 전체 3번 깜박임으로 자동 종료 알림
    LCD_Event_All_Icon_Blink(3, 500);
    POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
    /* ****************** */
  }
  else{
    Next_User_Mode_Process(MAIN_STICK_EJECTION);
  }

  if(STRIP_CHECK_SWITCH.event >= STICK_EJECTED)//Stick이 정상 분리 되었다면
  {
    User_Mode_Variable_Init();
    memset(&MEM_BANK_1, 0, sizeof(MEM_BANK_1)); //이전 사용 data 초기화
    Load_Memory_Bank(MEMORY_ADDR_1, &MEM_BANK_1); //초기 data (EEPROM) MEM_BANK_1 setting
    API_Key_Value_Init(&API_DATA); //API KEY, VALUE 초기화
    LCD_Display(EJECT, BLINK_OFF);
    //LCD_Display(EJECT, ICON_OFF);
    delay_ms(1500);
    //Test 처음 단계로 돌아감
    Next_User_Mode_Process(MAIN_APP_CONNECTION_WAITING);    
  }  
}

void Error_Stick_Check()
{
  if(!once_operation)
  {
    once_operation = 1;
    API_DATA.proc_step = MAIN_ERROR_STICK_CHECK;
    //Stick check 단계에서 Contents확인 + Calibration data에 문제가 있을 경우 
    //Error 표시 및 Stick 분리 유도
    LCD_Display(BOOK, ICON_ON);
    LCD_Display(EJECT, BLINK_ON);
    LCD_Display(INSERT, ICON_OFF);
    Timer_Count_Setup(&TIMER_COUNT_UP_1, 0, TIMER_COUNT_ENABLE); //Start error stick ejection timer 
  }
  
  if(TIMER_COUNT_UP_1.count >= ERROR_SITCK_CHECK_TIME) //2분내 stick이 분리되지 않았다면  
  {
    /* Error Event Handle*/
    User_Mode_Variable_Init();
    //LCD ICON 전체 3번 깜박임으로 자동 종료 알림
    LCD_Event_All_Icon_Blink(3, 500);
    POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
    /* ****************** */
  }
  else{
    Next_User_Mode_Process(MAIN_ERROR_STICK_CHECK);
  }

  if(STRIP_CHECK_SWITCH.event >= STICK_EJECTED) //2분내 Stick이 정상 분리 되었다면
  {
    User_Mode_Variable_Init();
    memset(&MEM_BANK_1, 0, sizeof(MEM_BANK_1)); //이전 사용 data 초기화
    Load_Memory_Bank(MEMORY_ADDR_1, &MEM_BANK_1); //초기 data (EEPROM) MEM_BANK_1 setting
    if((API_DATA.stick_color) && (API_DATA.react_time) && (API_DATA.stick_insert_wait_time)) //API 'DEV_INFO' 명령으로 stick_color, react_time, insert_time을 설정 했었다면 임시 백업
    {
      stick_color_backup = API_DATA.stick_color; react_time_backup = API_DATA.react_time; insert_time_backup = API_DATA.stick_insert_wait_time; //stick_color, react_time, insert_time 임시 백업
      API_Key_Value_Init(&API_DATA); //API KEY, VALUE 초기화
    }
    else{
      API_Key_Value_Init(&API_DATA); //API KEY, VALUE 초기화
    }
    delay_ms(1500);
    LCD_Display(BOOK, ICON_OFF);//Error 표시 제거 후
    LCD_Display(EJECT, BLINK_OFF);
    //LCD_Display(EJECT, ICON_OFF);
    //Test 처음 단계로 돌아감
    Next_User_Mode_Process(MAIN_APP_CONNECTION_WAITING);
  }
}

void Error_Stick_Insert_Check()
{
  char i = 0;
  char stick_color_check = 0;

  //Stick 강제 분리 후 재 삽입 되었나?
  if((STRIP_CHECK_SWITCH.event == STICK_INSERTED) && (stick_error_check)) 
  {
    delay_ms(((uint32_t)API_DATA.stick_insert_wait_time) * 1000);

    //Stick color check
    stick_color_check = Get_Stick_Color(); //삽입된 Stick color 확인
    API_DATA.stick_color_detect = stick_color_check;
    
    if(stick_color_check == API_DATA.stick_color) //API를 통해 수신된 stick color와 분리 후 재 삽입한 stick color 일치 여부 확인
    {
      stick_error_check = 0;
      
      //Stick Band Check
      PTR_Sensing_Bandline(recent_band_sensing_data);
      for(i = 1; i <= 5; i++)
      {
        //Unused stick의 calibration 진행 후 PTR sensing data 이용
        //Min (20%), Max (108%)로 현재 Stick의 정상 삽입 여부 확인 
        float min = (float)MEM_BANK_1.AGING_BAND_SENSING_DATA[i] * 0.2;
        float max = (float)MEM_BANK_1.AGING_BAND_SENSING_DATA[i] * 1.08;
        //삽입된 Stick이 정상인가?
        if((min <= recent_band_sensing_data[i])&&(recent_band_sensing_data[i] <= max)){} //Yes
        else{ //No
          stick_error_check = ERROR_PTR_SENSING_DATA;
        }
      }

      if(stick_error_check != ERROR_PTR_SENSING_DATA)// Normal state stick
      {
        stick_error_check = 0;
        API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, NORM_VAL); //KEY: stick_state, VALUE: norm
        LCD_Display(BOOK, ICON_OFF); //Error 표시 제거
        LCD_Display(INSERT, BLINK_OFF); LCD_Display(INSERT, ICON_ON);
        Timer_Count_Setup(&TIMER_COUNT_UP_2, 0, TIMER_COUNT_DISABLE);
      }
      else{ //Error used stick
        API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, ERR_USED_VAL); //KEY: stick_state, VALUE: err_used
      }
    }
    else //Error stick color
    {
      API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, ERR_CONTENT_VAL); //KEY: stick_state, VALUE: err_content
      stick_error_check = ERROR_STICK_COLOR_DETECT;
    }
    //Stick color check
  }
  else if(STRIP_CHECK_SWITCH.event >= STICK_EJECTED) //Stick 강제 분리 되었나?
  {
    API_Key_Value_Setting(&API_DATA, STICK_STATE_KEY, ERR_EJECT_VAL); //KEY: stick_state, VALUE: err_eject
    API_DATA.stick_color_detect = 'U';
    stick_error_check = ERROR_FORCE_STICK_EJECT;
  }
  else{}

  if(stick_error_check) //Stick에 문제가 있다면
  {
    LCD_Display(BOOK, ICON_ON);
    LCD_Display(INSERT, BLINK_ON);
    TIMER_COUNT_UP_2.start = START;
  }

  if(TIMER_COUNT_UP_2.count >= STICK_RE_INSERTION_TIME) //2분내 stick 재삽입하지 않을 경우 기기 자동 종료
  {
    /* Error Event Handle*/
    User_Mode_Variable_Init();
    //LCD ICON 전체 3번 깜박임으로 자동 종료 알림
    LCD_Event_All_Icon_Blink(3, 500);
    POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
    /* ****************** */
  }
}

char Get_Stick_Color()
{
  char color_detect = 0;
  /* Color sensing - raw data */
  PTR_Sensing_Colorline(recent_color_sensing_data);
  float green_ptr_ratio = ((float)recent_color_sensing_data[0] / (float)initial_color_sensing_data[0]) * 100.0;
  float red_ptr_ratio = ((float)recent_color_sensing_data[1] / (float)initial_color_sensing_data[1]) * 100.0;

  float g_r_ratio = green_ptr_ratio / red_ptr_ratio;

  if(g_r_ratio <= CONTENT_COLOR_CUTOFF_1){ //Red
    color_detect = 'R';
  }
  else if ((CONTENT_COLOR_CUTOFF_1 < g_r_ratio) && (g_r_ratio <= CONTENT_COLOR_CUTOFF_2)){ //Yellow
    color_detect = 'Y';
  }
  else if((CONTENT_COLOR_CUTOFF_5 < g_r_ratio) && (g_r_ratio <= CONTENT_COLOR_CUTOFF_6)){ //White
    color_detect = 'W';
  }
  else if((CONTENT_COLOR_CUTOFF_3 < g_r_ratio) && (g_r_ratio <= CONTENT_COLOR_CUTOFF_4)){ //Green
    color_detect = 'G';
  }
  else //Color error 인식 불가
  {
    color_detect = 'U';
  }

  return color_detect;
}

void User_Mode_Variable_Init()
{
  once_operation = 0;
  Timer_Count_Setup(&TIMER_COUNT_UP_1, 0, TIMER_COUNT_DISABLE);
  Timer_Count_Setup(&TIMER_COUNT_UP_2, 0, TIMER_COUNT_DISABLE);
  Timer_Count_Setup(&TIMER_COUNT_DOWN_1, 0, TIMER_COUNT_DISABLE);

  stick_error_check = 0;
  check_sample_flow_process = END;
  cur_sample_reaction_progress_step = 0;
  pre_sample_reaction_progress_step = 0;
}

void Next_User_Mode_Process(char process_number)
{
  USER_MODE_PROCESS.function_number = process_number;
  USER_MODE_PROCESS.flag = END;
}

void Run_Usermode()
{
  Admin_Variable_Init();
  API_Key_Value_Init(&API_DATA);
  User_Mode_Variable_Init();
  LCD_Result_Display_Off();
  LCD_Lower_Icon_Off();
  LCD_Display(EJECT, BLINK_OFF);
  device_mode_select = DEV_APP_CONNECTION_MODE;
  Next_User_Mode_Process(MAIN_APP_CONNECTION_WAITING);
}