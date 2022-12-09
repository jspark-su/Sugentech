/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
PRODUCT_MODE_VAL_TypeDef PRODUCT_MODE_VAL;
PRODUCT_MODE_PROCESS_TypeDef PRODUCT_MODE_PROCESS;
PRODUCT_MODE_READ_PTR_Typedef PRODUCT_MODE_PTR;

uint8_t fw_update_mode_execution = 0;

ADC_Channel_TypeDef ptr_channel = PTR_1;
GPIO_Pin_TypeDef led_channel;
uint8_t dac_interval = 0;
uint16_t vDac = 0;

/* Private function prototypes -----------------------------------------------*/
void Production_Mode_Start();
void Produnction_LED_Calibration_Start();
void Production_Read_Rawdata_Start();
void Production_Read_PTR_Start();
void Production_Save_Result_Data_In_EEPROM();
char Production_BLE_Name_Setup_Start();
void Production_Mode_End();
void API_READ_PTR_Variable_Init(void);

/* ============================================================================
                 ##### Production Mode functions #####
 =============================================================================*/

void Production_Mode_Process()
{
  if(PRODUCT_MODE_PROCESS.state == START)
  {
    switch(PRODUCT_MODE_PROCESS.process_number)
    {
      case PRODUCT_MODE_START :
        Production_Mode_Start();
        break;
      case PRODUCT_CALIB_START :
        Produnction_LED_Calibration_Start();
        break;
      case PRODUCT_READ_RAWDATA_START :
        Production_Read_Rawdata_Start();
        break;
      case PRODUCT_READ_PTR_START :
        Production_Read_PTR_Start();
        break;
      case PRODUCT_MODE_END :
        Production_Mode_End();
        break;
    }
  }
}

void Production_Mode_Start()
{
  User_Mode_Variable_Init(); //mode 전환 전 App connection mode 사용 변수 초기화
  
  API_Key_Value_Setting(&API_DATA, CALIB_STICK_STATE_KEY, CALIB_STICK_EJECTED_VAL);
  API_Key_Value_Setting(&API_DATA, CALIB_PROC_STATE_KEY, NONE_VAL);
  API_Key_Value_Setting(&API_DATA, RAW_DATA_STATE_KEY, NONE_VAL);
  API_READ_PTR_Variable_Init();
  
  LCD_Result_Display_Off();
  LCD_Lower_Icon_Off();
  LCD_Display(SQUARE_1, ICON_ON); LCD_Display(SQUARE_2, ICON_ON); 
  LCD_Display(SQUARE_3, ICON_ON); LCD_Display(SQUARE_4, ICON_ON);
  
  Production_Mode_Next_Process(PRODUCT_MODE_NONE, END);
}

void Production_Calibration_Stick_Check()
{
  if(STRIP_CHECK_SWITCH.event == STICK_INSERTED){ //Stick 삽입
    API_Key_Value_Setting(&API_DATA, CALIB_STICK_STATE_KEY, CALIB_STICK_INSERTED_VAL);
  }
  else if(STRIP_CHECK_SWITCH.event >= STICK_EJECTED){ //Stick 분리
    API_Key_Value_Setting(&API_DATA, CALIB_STICK_STATE_KEY, CALIB_STICK_EJECTED_VAL);
  }
}

void Produnction_LED_Calibration_Start()
{
  OP_Amp_Enable();
  API_Key_Value_Setting(&API_DATA, CALIB_PROC_STATE_KEY, DOING_VAL);
  Auto_Calibration_Bandline();
  Auto_Calibration_Colorline();

  //ptr data sensing
  PTR_Sensing_Bandline(initial_band_sensing_data);
  PTR_Sensing_Colorline(initial_color_sensing_data);
  
  OP_Amp_Disable();
  //MEM_BANK_1 영역 최신 calibration, PTR sensing data update
  Input_Init_Band_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); //MEM_BANK_1의 Band line DAC, PTR data update
  Input_Init_Color_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); //MEM_BANK_3의 Color line DAC, PTR data update
  memcpy(MEM_BANK_1.AGING_BAND_SENSING_DATA, initial_band_sensing_data, sizeof(MEM_BANK_1.AGING_BAND_SENSING_DATA)); //MEM_BANK_1의 Aging band sensing data update
  if(calibration_check == CALIBRATION_FAIL){ //Fail
    API_Key_Value_Setting(&API_DATA, CALIB_PROC_STATE_KEY, FAIL_VAL);
  }
  else{ //Complete
    API_Key_Value_Setting(&API_DATA, CALIB_PROC_STATE_KEY, COMPLETE_VAL);
  }
  calibration_check = CALIBRATION_NONE;
  
  Production_Mode_Next_Process(PRODUCT_MODE_NONE, END);
}

void Production_Read_Rawdata_Start()
{
  char i = 0;
  API_Key_Value_Setting(&API_DATA, RAW_DATA_STATE_KEY, DOING_VAL);
  OP_Amp_Enable();
  PTR_Sensing_Bandline(PRODUCT_MODE_VAL.ch_rawdata);
  PTR_Sensing_Colorline(&PRODUCT_MODE_VAL.ch_rawdata[6]);
  OP_Amp_Disable();
//
  for(i = 1; i <= 7; i++)
  {
    if(i <= 5){
      PRODUCT_MODE_VAL.ch_ratio_data[i] = ((float)PRODUCT_MODE_VAL.ch_rawdata[i] / (float)MEM_BANK_1.AGING_BAND_SENSING_DATA[i]) * 100.0;
    }
    else{
      PRODUCT_MODE_VAL.ch_ratio_data[i] = ((float)PRODUCT_MODE_VAL.ch_rawdata[i] / (float)MEM_BANK_1.INITIAL_COLOR_SENSING_DATA[i-6]) * 100.0;
    }
  }
//
  API_Key_Value_Setting(&API_DATA, RAW_DATA_STATE_KEY, COMPLETE_VAL);
  
  Production_Mode_Next_Process(PRODUCT_MODE_NONE, END);
}

void Production_Read_PTR_Start()
{
  uint16_t p_data = 0;
  uint8_t cnt_ReadPTR = 0;
  uint8_t index_buffer = 0;

  if(PRODUCT_MODE_PTR.OneTimeProcess == 0)
  {
    switch(PRODUCT_MODE_PTR.OpticsChannel)
    {
    case 0:
      led_channel = LED_1;
      ptr_channel = PTR_1;
      break;
    case 1:
      led_channel = LED_2;
      ptr_channel = PTR_2;
      break;
    case 2:
      led_channel = LED_3;
      ptr_channel = PTR_3;
      break;
    case 3:
      led_channel = LED_4;
      ptr_channel = PTR_4;
      break;
    case 4:
      led_channel = LED_5;
      ptr_channel = PTR_5;
      break;
    case 5:
      led_channel = LED_6;
      ptr_channel = PTR_6;
      break;
    case 6:
      led_channel = LED_7;
      ptr_channel = PTR_6;
      break;
    default:
      // Production_Mode_Next_Process(PRODUCT_MODE_NONE, END);
      return;
    }
    
    dac_interval = PRODUCT_MODE_PTR.vDacInterval;
    vDac = 0;
    PRODUCT_MODE_PTR.OneTimeProcess = 1;
  }
  else {
    if(PRODUCT_MODE_PTR.status == READ_PTR_NONE){
      if(index_buffer == 0){
        memset(&PRODUCT_MODE_PTR.DacData, 0, sizeof(PRODUCT_MODE_PTR.DacData));
        memset(&PRODUCT_MODE_PTR.PtrData, 0, sizeof(PRODUCT_MODE_PTR.PtrData));
      }
      
      OP_Amp_Enable();
      
      LED_Ctrl(led_channel, LED_ON);
      DAC_Start();
      ADC_Start(ptr_channel);
      
      while(1){
        DAC_Data_Set(vDac);
        
        p_data = ADC_Data();
        p_data = 0;
        // Oversampling 10
        for(cnt_ReadPTR = 0; cnt_ReadPTR < 10; cnt_ReadPTR++){
          p_data += ADC_Data();
        }
        
        PRODUCT_MODE_PTR.DacData[index_buffer] = vDac;
        PRODUCT_MODE_PTR.PtrData[index_buffer] = p_data;

        vDac = vDac + dac_interval;
        if(vDac > 4095){
          // AT_Tx_Message
          PRODUCT_MODE_PTR.status = READ_PTR_READ_COMPLETE;
          break;
        }
        
        index_buffer++;
        if(index_buffer == 10){
          index_buffer = 0;
          PRODUCT_MODE_PTR.status = READ_PTR_READ_COMPLETE;
          break;
        }
      }
      
      ADC_End(ptr_channel);
      LED_Ctrl(led_channel, LED_OFF);
      DAC_End();
      OP_Amp_Disable();
    }
  }
  
  Production_Mode_Next_Process(PRODUCT_MODE_NONE, END);
}


void Production_Mode_End()
{
  char i = 0;
  char ble_name_setup_result = 0;

  delay_ms(500); //API 'PRODUCT_MODE'의 'end' 응답 송신을 위한 대기 (EEPROM 저장 진행 중 system 일시중지로 API 응답 불가를 막기 위해)
  Production_Save_Result_Data_In_EEPROM(); //설정 결과 data EEPROM에 저장. (EEPROM address: MEMORY_ADDR_1, MEMORY_ADDR_3)

  if(!strcmp(API_DATA.ble_name_setup_state,"start")){ //BLE name 문제로 설정 진행
    ble_name_setup_result = Production_BLE_Name_Setup_Start();
  }
  else //BLE name 정상으로 설정 과정 생략 
  {
    BLE_Disconnect();
    ble_name_setup_result = BLE_NAME_SETUP_COMPLETE;
  }

  for(i = 0; i < 6; i++)
  {
    LCD_Display(SQUARE_1, ICON_ON); LCD_Display(SQUARE_2, ICON_ON);
    LCD_Display(SQUARE_3, ICON_ON); LCD_Display(SQUARE_4, ICON_ON);
    if(ble_name_setup_result != BLE_NAME_SETUP_COMPLETE){LCD_Display(BOOK, ICON_ON); LCD_Display(INSERT, ICON_ON);}
    if(!strcmp(API_DATA.calib_proc_stat, "fail")){LCD_Display(BOOK, ICON_ON); LCD_Display(EJECT, ICON_ON);}
    delay_ms(200);
    LCD_Display(SQUARE_1, ICON_OFF); LCD_Display(SQUARE_2, ICON_OFF);
    LCD_Display(SQUARE_3, ICON_OFF); LCD_Display(SQUARE_4, ICON_OFF);
    if(ble_name_setup_result != BLE_NAME_SETUP_COMPLETE){LCD_Display(BOOK, ICON_OFF); LCD_Display(INSERT, ICON_OFF);}
    if(!strcmp(API_DATA.calib_proc_stat, "fail")){LCD_Display(BOOK, ICON_OFF); LCD_Display(EJECT, ICON_OFF);}
    delay_ms(200);
  }

  Run_Usermode(); //produnction mode 종료 후 App connection mode 시작
  Production_Mode_Next_Process(PRODUCT_MODE_NONE, END);
}

void Production_Save_Result_Data_In_EEPROM()
{
  MEMORY_BANK MEM_BANK_3 = {0,}; // Factory data 임시 저장 용 MEM_BANK_3 초기화 
  
  MEM_BANK_1.ACCESS_ADDR_1 = SET;
  MEM_BANK_3.ACCESS_ADDR_1 = SET;
      
  //Calibration data
  Input_Init_Band_Sensing_Data_In_Memory_Bank(&MEM_BANK_3); //MEM_BANK_3의 Band line DAC, PTR data update
  Input_Init_Color_Sensing_Data_In_Memory_Bank(&MEM_BANK_3); //MEM_BANK_3의 Color line DAC, PTR data update
  memcpy(MEM_BANK_3.AGING_BAND_SENSING_DATA, initial_band_sensing_data, sizeof(MEM_BANK_3.AGING_BAND_SENSING_DATA)); //MEM_BANK_3의 Aging band sensing data update

  delay_ms(300);
  Save_Memory_Bank_Byte_Program(MEMORY_ADDR_1, &MEM_BANK_1); //Save user mode data area (ADDR_1)
  Save_Memory_Bank_Byte_Program(MEMORY_ADDR_3, &MEM_BANK_3); //Save factory data area (ADDR 3)
}

char Production_BLE_Name_Setup_Start()
{
  char ble_name_setup_retry = 0;
  char at_info_retry = 0;
  char setup_result = 0;

  BLE_Disconnect();
  BLE_FLAG.connect_status = BLE_DISCONNECT;
  while(ble_name_setup_retry < 5)
  {
    if(BLE_Name_Setting(API_DATA.ble_name_str) == BLE_NAME_SETUP_OK){break;}
    else{ble_name_setup_retry++;} //설정 재시도
  }

  if(ble_name_setup_retry >= 5) //BLE name 설정 실패
  {
    EEPROM_Byte_Write(BLE_NAME_SETUP_COMPLETE_ADDR, BLE_SETUP_NONE);
    setup_result = BLE_NAME_SETUP_FAIL_AT_MANUF;
  }
  else //BLE name 설정 성공
  {
    //설정 완료, "AT+INFO" 명령으로 검증 시작
    while(at_info_retry < 5) //"AT+INFO?" 명령으로 name 정상 설정 완료 여부 확인
    {
      if(Send_AT_Command("AT+INFO?\r") == AT_CMD_RESP_STRING_OK)
      {
        if(!strcmp(API_DATA.ble_name_str, ble_name)){break;} //API로 수신된 BLE name이 BLE module에 정상 적용 완료
        else{at_info_retry++;} //API로 수신된 BLE name과 BLE module에 적용된 name과 다름, 재시도
      }
      else{at_info_retry++;} //"AT+INFO?" 명령 재시도 (응답 수신 불가)
    }

    if(at_info_retry >= 5) //BLE name 설정 적용 (검증) 실패
    {
      EEPROM_Byte_Write(BLE_NAME_SETUP_COMPLETE_ADDR, BLE_SETUP_NONE);
      setup_result = BLE_NAME_SETUP_FAIL_AT_INFO;
    }
    else //BLE name 정상 적용 검증 완료
    {
      EEPROM_Byte_Write(BLE_NAME_SETUP_COMPLETE_ADDR, BLE_SETUP_COMPLETE);
      setup_result = BLE_NAME_SETUP_COMPLETE;
    }
  }
  
  return setup_result;
}

void Production_Mode_Next_Process(char process_number, char state)
{
  if(state == START){
    PRODUCT_MODE_PROCESS.state = START;
  }
  else if(state == END){
    PRODUCT_MODE_PROCESS.state = END;
  }
  PRODUCT_MODE_PROCESS.process_number = process_number;
}

void API_READ_PTR_Variable_Init(void)
{
  PRODUCT_MODE_PTR.OneTimeProcess = 0;
  PRODUCT_MODE_PTR.OpticsChannel = 0;
  PRODUCT_MODE_PTR.vDacInterval = 0;
  PRODUCT_MODE_PTR.status = READ_PTR_READ_COMPLETE;
  memset(&PRODUCT_MODE_PTR.DacData, 0, sizeof(PRODUCT_MODE_PTR.DacData));
  memset(&PRODUCT_MODE_PTR.PtrData, 0, sizeof(PRODUCT_MODE_PTR.PtrData));

  dac_interval = 0;
}