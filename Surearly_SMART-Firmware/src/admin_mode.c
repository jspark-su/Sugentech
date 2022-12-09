#include "main.h"

uint16_t main_menu_select = 0;
char sub_menu_select = 0;
char admin_rx_complete = 0;

uint8_t admin_init_menu_disp_flag = ADMIN_MENU_DISPLAY_OFF;
//volatile uint8_t multi_time_test_start = READY;
//volatile uint8_t current_time_test_start = READY;

//uint8_t content_select = 0;
//uint8_t cutoff_select = 0;

uint8_t apply_usermode_type = 0;

void Admin_BLE_Rx_Process()
{
  if(BLE_RX.api_resp_chk == API_RESP_OK)
  {
    if(!strcmp(BLE_RX.data, "m\r\n"))
    {
      if(!main_menu_select){
        admin_init_menu_disp_flag = ADMIN_MENU_DISPLAY_ON;
      }
    }
    else if(!strcmp(BLE_RX.data, "|usermode#\r\n"))
    {
      Run_Usermode();
    }
    else{
      admin_rx_complete = 1; //메뉴 선택 문자 수신 (기능 문자 X)
    }
    if(!admin_rx_complete){ //수신된 문자가 기능 문자 (m, x, |usermode#)라면 RX 버퍼 초기화
      memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
    }
    BLE_RX.api_resp_chk = API_RESP_NONE;
  }
}

void Admin_Menu_Display()
{
  if((BLE_FLAG.connect_status == BLE_CONNECTED) && (device_mode_select == DEV_ADMIN_MODE))
  {
    if(!(admin_init_menu_disp_flag == ADMIN_MENU_DISPLAY_OFF))
    {
      admin_init_menu_disp_flag = ADMIN_MENU_DISPLAY_OFF;
      delay_ms(100);
      printf("\r\n*** Surearly SMART Admin Mode ***\r\n");
      printf("\r\n - Engineer: YBW\r\n");
      printf(" - Version: %s\r\n", FW_VER);
      printf(" - BLE Name: %s\r\n",ble_name);
      printf(" - BLE MAC: %s\r\n", ble_mac_id);
      printf(" - MCU UID: %s\r\n", mcu_uid);  
      printf("\r\n============== Menu ==============\r\n");
      printf("\r\n------- Device settings -------\r\n");
      printf(" %d. Memory bank erase\r\n", MAIN_MEMORY_BANK_ERASE);
      printf("\r\n----- Read settings -----\r\n");
      printf(" %d. Memory bank\r\n", MAIN_READ_MEMORY_BANK);
      printf(" %d. Device information\r\n", MAIN_DEVICE_INFORMATION);
      printf("\r\n");
      printf(" - Notice -\r\n");
      printf(" *Menu display => 'm'\r\n");
      printf("\r\n");
      printf(" > Enter the menu number => ");
      memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
    }
  }
}

void Admin_Menu_Process()
{
  if(admin_rx_complete)
  {
    admin_rx_complete = 0;
    
    if(!main_menu_select) //main menu process
    {
      main_menu_select = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
      //메뉴 선택시만 opamp enable
      if(main_menu_select){
        OP_Amp_Enable();
      }
      //
      switch(main_menu_select)
      {
        case MAIN_MEMORY_BANK_ERASE: //Memory Bank Erase
          printf("\r\n\r\n* Memory bank erase *\r\n\r\n");
          printf("- User data area [EEPROM addr: 0x1000]\r\n");
          printf(" 1) Erase\r\n");
          printf(" 2) Cancel\r\n\r\n");
          sub_menu_select = SUB_MEMORY_BANK_ERASE;
          break;  
        case MAIN_READ_MEMORY_BANK: //Read Memory Bank
          printf("\r\n\r\n* Read memory bank *\r\n");
          printf("- User data area [EEPROM addr: 0x1000]\r\n");
          Admin_Read_Memory_Bank();
          break;
        case MAIN_DEVICE_INFORMATION: //Device Information
          Admin_Device_Information();
          break;                 
        default:
          main_menu_select = 0;
          break;
      }
      memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
    }
    else //sub menu process
    {
      switch(sub_menu_select)
      {
        case SUB_MEMORY_BANK_ERASE:
          Admin_Erase_Memory_Bank();
          break;   
      }
    }
    if(!main_menu_select){//main_menu_select 변수 0으로 초기화 시만 disable => multi,one time test 대응
      OP_Amp_Disable();
    }
    memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
  }
}



void Admin_Erase_Memory_Bank()
{
  char rx_size = 0;
  char input_num = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  input_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((rx_size == 1)&&(1 <= input_num)&&(input_num <= 2))
  {
    switch(input_num)
    {
      case 1 ://Start memory bank erase
        printf("\r\n Erasing...");
        Erase_Memory_Bank(MEMORY_ADDR_1);
        Erase_Memory_Bank(MEMORY_ADDR_2);
        //MEM_BANK_1에 변경 사항 반영
        memset(&MEM_BANK_1, 0, sizeof(MEM_BANK_1));
        Load_Memory_Bank(MEMORY_ADDR_1, &MEM_BANK_1);
        Load_Init_Band_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); // MEM_BANK의 band calibration data + PTR sensing data 전역 변수로 load
        Load_Init_Color_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); // MEM_BANK의 color calibration data + PTR sensing data 전역 변수로 load
        //
        printf("\r\n Complete!\r\n");
        printf("\r\n > Enter the menu number => ");
        Admin_Variable_Init();
        break;
      case 2 : //Cancel
        printf("\r\nCancel.\r\n");
        printf("\r\n > Enter the menu number => ");
        Admin_Variable_Init();
        break;
    }   
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
}

void Admin_Read_Memory_Bank()
{
/*
  
  char j = 0;
  char loop = 0;
  char cutoff_idx = 0;
  uint16_t *p_idx = NULL;
  MEMORY_BANK MEM_BANK_3 = {0,}; // 임시 open MEM_BANK_3
  Load_Memory_Bank(MEMORY_ADDR_1, &MEM_BANK_3); //user mode 영역 data load
  //최근 측정한 Contents
  printf("\r\n- Contents: %s\r\n", MEM_BANK_3.RECENT_CONTENT);
  //최근 측정한 T1~3 Raw data
  printf("\r\n- Raw data\r\n");
  printf(" T1: %0.3f, T2: %0.3f, T3: %0.3f\r\n", MEM_BANK_3.RECENT_RSLT_RAWDATA_T1, MEM_BANK_3.RECENT_RSLT_RAWDATA_T2, MEM_BANK_3.RECENT_RSLT_RAWDATA_T3);
  //Stick Lot version
  printf("\r\n- Stick Lot Version\r\n");
  printf(" *Preg: %s\r\n", MEM_BANK_3.PREG_LOT_VERSION);
  printf(" *Ovul: %s\r\n", MEM_BANK_3.OVUL_LOT_VERSION);
  printf(" *Meno: %s\r\n", MEM_BANK_3.MENO_LOT_VERSION);
  //Read cutoff data
  printf("\r\n- Cutoff data\r\n");
  for(loop = 0; loop < 5; loop++)
  {
    if(loop == 0){
      printf(" *Preg\r\n");
    }
    else if(loop == 1){
      printf(" *Ovul\r\n"); 
    }
    else if(loop == 2){
      printf(" *Meno\r\n");   
    }
    else if(loop == 3){
      printf(" *Covid IgG\r\n");   
    }
    else if(loop == 4){
      printf(" *Covid Ag\r\n");   
    }
    //Band line (T1, T2, T3) cutoff display
    printf(" Num\t");
    for(i = 0; i < BAND_CUTOFF_DATA_NUM; i++){
      printf(" Level_%d\t\t", i + 1);
    }
    printf("\r\n");
    for(i = 1; i <= 3; i++)
    {
      printf(" T%d\t",i);
      if(i == 1)
      {
        if(loop == 0){
          p_idx = &MEM_BANK_3.PREG_CUTOFF_DATA.T1_CUTOFF[0];
        }
        else if(loop == 1){
          p_idx = &MEM_BANK_3.OVUL_CUTOFF_DATA.T1_CUTOFF[0];
        }
        else if(loop == 2){
          p_idx = &MEM_BANK_3.MENO_CUTOFF_DATA.T1_CUTOFF[0];
        }
        else if(loop == 3){
          p_idx = &MEM_BANK_3.COVID_IgG_CUTOFF_DATA.T1_CUTOFF[0];
        }
        else if(loop == 4){
          p_idx = &MEM_BANK_3.COVID_Ag_CUTOFF_DATA.T1_CUTOFF[0];
        }
        else;
      }
      else if(i == 2)
      {
        if(loop == 0){
          p_idx = &MEM_BANK_3.PREG_CUTOFF_DATA.T2_CUTOFF[0];
        }
        else if(loop == 1){
          p_idx = &MEM_BANK_3.OVUL_CUTOFF_DATA.T2_CUTOFF[0];
        }
        else if(loop == 2){
          p_idx = &MEM_BANK_3.MENO_CUTOFF_DATA.T2_CUTOFF[0];
        }
        else if(loop == 3){
          p_idx = &MEM_BANK_3.COVID_IgG_CUTOFF_DATA.T2_CUTOFF[0];
        }
        else if(loop == 4){
          p_idx = &MEM_BANK_3.COVID_Ag_CUTOFF_DATA.T2_CUTOFF[0];
        }
        else;
      }
      else if(i == 3)
      {
        if(loop == 0){
          p_idx = &MEM_BANK_3.PREG_CUTOFF_DATA.T3_CUTOFF[0];
        }
        else if(loop == 1){
          p_idx = &MEM_BANK_3.OVUL_CUTOFF_DATA.T3_CUTOFF[0];
        }
        else if(loop == 2){
          p_idx = &MEM_BANK_3.MENO_CUTOFF_DATA.T3_CUTOFF[0];
        }
        else if(loop == 3){
          p_idx = &MEM_BANK_3.COVID_IgG_CUTOFF_DATA.T3_CUTOFF[0];
        }
        else if(loop == 4){
          p_idx = &MEM_BANK_3.COVID_Ag_CUTOFF_DATA.T3_CUTOFF[0];
        }
      }
      if(i == 1) //T1 cutoff data number
      { //Preg, COVID IgG, Ag T1만 cutoff 5개
        if((loop == 0) || (loop == 3) || (loop == 4))
        cutoff_idx = BAND_CUTOFF_DATA_NUM;
      }
      else{ //T1 (Ovul, Meno), T2, T3 cutoff data number 3개
        cutoff_idx = BAND_CUTOFF_DATA_NUM - 2;      
      }
      for(j = 0; j < cutoff_idx; j++)
      {
        printf(" %u\t\t", *p_idx);
        p_idx++;
      }
      printf("\r\n");
    }
  }

  printf("\r\n- Calibration data\r\n");
  printf("\r\n Line\tL1\tL2\tL3\tL4\tL5\tL6[G]\tL7[R]\r\n");
  printf(" DAC\t");
  for(i = 1; i <= 7; i++)
  {
      printf("%4u\t", MEM_BANK_3.CALIBRATION_DATA[i]);
  }
  printf("\r\n PTR\t");
  for(i = 1; i <= 7; i++)
  {
      printf("%.1f\t", (float)(MEM_BANK_3.INITIAL_STICK_SENSING_DATA[i] / 10.0));
  }

  printf("\r\n\r\n- Correction factor\r\n");
  printf(" Preg: %u, Ovul: %u, Meno: %u\r\n", MEM_BANK_1.PREG_CORRECTION_FACTOR, MEM_BANK_1.OVUL_CORRECTION_FACTOR, MEM_BANK_1.MENO_CORRECTION_FACTOR);

  printf("\r\n- Reaction time [sec]\r\n");
  printf(" Preg: %u, Ovul: %u, Meno: %u, Covid_IgG: %u, Covid_Ag: %u\r\n", MEM_BANK_1.PREG_REACT_TIME, MEM_BANK_1.OVUL_REACT_TIME, MEM_BANK_1.MENO_REACT_TIME, MEM_BANK_1.COVID_IgG_REACT_TIME, MEM_BANK_1.COVID_Ag_REACT_TIME);

  
  
  printf("\r\n- Init operating mode\r\n");
  if(MEM_BANK_1.INIT_OPERATION_MODE == DEV_APP_CONNECTION_MODE){
    printf(" App connection mode");
  }
  else;

*/
  char i = 0;
  
  MEMORY_BANK MEM_BANK_3 = {0,}; // 임시 open MEM_BANK_3
  Load_Memory_Bank(MEMORY_ADDR_1, &MEM_BANK_3); //user mode 영역 data load
  
  printf("\r\n- Calibration data\r\n");
  printf("\r\n Line\tL1\tL2\tL3\tL4\tL5\tL6[G]\tL7[R]\r\n");
  printf(" DAC\t");
  for(i = 1; i <= 7; i++)
  {
      printf("%4u\t", MEM_BANK_3.CALIBRATION_DATA[i]);
  }
  printf("\r\n PTR\t");
  for(i = 1; i <= 7; i++)
  {
    if(i <= 5){
      printf("%.1f\t", (float)(MEM_BANK_3.INITIAL_BAND_SENSING_DATA[i] / 10.0));
    }
    else if (i >= 6){
      printf("%.1f\t", (float)(MEM_BANK_3.INITIAL_COLOR_SENSING_DATA[i - 6] / 10.0));
    }
  }

  printf("\r\n- Device usage count\r\n"); //측정 완료 횟수
  printf(" Num: %u\r\n", MEM_BANK_1.DEV_USAGE_CNT);

  printf("\r\n- Stick insertion count\r\n");
  printf(" Num: %u\r\n", MEM_BANK_1.STICK_INSERT_CNT);

  printf("\r\n\r\n > Enter the menu number => ");
  Admin_Variable_Init();
}

void Admin_Device_Information()
{
  printf("\r\n\r\n** DEVICE INFORMATION **\r\n\r\n");  
  //VBAT voltage
  printf("Battery Volt: %.3f V\r\n", Vbat_Read_Voltage());
  //
  //MCU voltage
  printf("MCU Volt: %.3f V\r\n", MCU_Read_Voltage());
  //
  //MCU tmperature
  printf("MCU Temperature: %.1f C\r\n\r\n", mcu_temperature_read());
  Next_User_Mode_Process(MAIN_MENU_VIEW);
  //        
  printf("\r\n > Enter the menu number => ");
  Admin_Variable_Init();
}

void Admin_Variable_Init()
{
  //multi_time_test_start = READY;
  //current_time_test_start = READY;
  main_menu_select = 0;
  sub_menu_select = 0;
}

void Run_Adminmode()
{
  User_Mode_Variable_Init();
  API_Key_Value_Init(&API_DATA);
  Admin_Variable_Init();
  Next_User_Mode_Process(MAIN_APP_CONNECTION_WAITING);
  device_mode_select = DEV_ADMIN_MODE;
  printf("\r\nStart admin mode !!\r\n");
}

/*
void Admin_Init_Operation_Mode()
{
  char rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;
  char input_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((rx_size == 1)&&(1 <= input_num)&&(input_num <= 4))
  {
    if(input_num == 1){
      MEM_BANK_1.INIT_OPERATION_MODE = DEV_APP_CONNECTION_MODE; //App connection mode
    }
    else if(input_num == 2){
      MEM_BANK_1.INIT_OPERATION_MODE = DEV_ADMIN_MODE; //Admin mode  
    }
    sub_menu_select = SUB_SAVE_FACTORY_DATA;
    apply_usermode_type = INIT_OPERATION_MODE_SAVE_FACTORYDATA;
    printf("\r\n Save to user mode area?\r\n");
    printf(" 1) Save\r\n");
    printf(" 2) Exit\r\n");
    printf(" >");
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
}
*/

/*
void Admin_Save_Factory_data()
{
  char rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF; // ex) 수신 data 2\r\n -> rx_size = 2 
  char input_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  MEMORY_BANK MEM_BANK_3 = {0,}; // Factory data 임시 저장 용 MEM_BANK_3 초기화 
  
  if(rx_size == 1) //수신 data 길이 검사
  {
    if(input_num == ADMIN_SAVE_OK) //Save
    {
      printf(" Saving..\r\n");
      Load_Memory_Bank(MEMORY_ADDR_3, &MEM_BANK_3); //기존 저장된 factory data load
      MEM_BANK_1.ACCESS_ADDR_1 = SET;
      MEM_BANK_3.ACCESS_ADDR_1 = SET;
      switch(apply_usermode_type)
      {
        case INIT_OPERATION_MODE_SAVE_FACTORYDATA:
          MEM_BANK_3.INIT_OPERATION_MODE = MEM_BANK_1.INIT_OPERATION_MODE;
          break;
      }
      Save_Memory_Bank_Byte_Program(MEMORY_ADDR_1, &MEM_BANK_1); //Save user mode data area (ADDR_1)
      Save_Memory_Bank_Byte_Program(MEMORY_ADDR_3, &MEM_BANK_3); //Save factory data area (ADDR 3)
      printf("\r\n Complete!\r\n");
    }
    else if(input_num == ADMIN_SAVE_NO){
        printf("\r\n Exit.\r\n");
    }
    // 최신 (변경된) data로 MEM_BANK_1 초기화
    memset(&MEM_BANK_1, 0, sizeof(MEM_BANK_1));
    Load_Memory_Bank(MEMORY_ADDR_1, &MEM_BANK_1);
    Load_Init_Band_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); // MEM_BANK의 band calibration data + PTR sensing data 전역 변수로 load
    Load_Init_Color_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); // MEM_BANK의 color calibration data + PTR sensing data 전역 변수로 load 
    //
    //content_select = 0;
    apply_usermode_type = 0;
    Admin_Variable_Init();
    printf("\r\n > Enter the menu number => ");
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" >");
  }
}
*/





void Admin_LED_Calibration()
{
 /*
  char i = 0;
  printf("\r\n\r\n** LED Calibration **\r\n\r\n");
  //led_calibaration
  Auto_Calibration_Bandline();
  Auto_Calibration_Colorline();
  printf("\r\n\r\nLine\tL1\tL2\tL3\tL4\tL5\tL6[G]\tL7[R]\r\n");
  printf("DAC\t");
  for(i = 1; i <= 7; i++)
  {
      printf("%5d\t", calibration_data[i]);
  }
  
  //ptr data sensing
  PTR_Sensing_Bandline(initial_band_sensing_data);
  PTR_Sensing_Colorline(initial_color_sensing_data);
  //
  printf("\r\nPTR\t");
  for(i = 1; i <= 7; i++)
  {
    if(i <= 5){
      printf("%.1f\t", (float)(initial_band_sensing_data[i] / 10.0));
    }
    else{
      printf("%.1f\t", (float)(initial_color_sensing_data[i - 6] / 10.0));
    }
  }
  //MEM_BANK_1 영역 최신 calibration, PTR sensing data update
  Input_Init_Band_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); //MEM_BANK_1의 Band line DAC, PTR data update
  Input_Init_Color_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); //MEM_BANK_3의 Color line DAC, PTR data update
  memcpy(MEM_BANK_1.AGING_BAND_SENSING_DATA, initial_band_sensing_data, sizeof(MEM_BANK_1.AGING_BAND_SENSING_DATA)); //MEM_BANK_1의 Aging band sensing data update

  printf("\r\n\r\n\r\nComplete.\r\n");
  printf("\r\n Save to user mode area?\r\n");
  printf(" 1) Save\r\n");
  printf(" 2) Exit\r\n");
  printf(" >");
  sub_menu_select = SUB_SAVE_FACTORY_DATA;
  apply_usermode_type = CALIBRATION_SAVE_FACTORYDATA;
*/
}

void Admin_Multi_Time_Test() // BLE 연결 후 MULTI TIME MENU 초기 화면 출력
{
/*  
  int loop_num = 0;
  char rx_size = 0;
  
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// 값 범위 1~600, ex) 5\r\n, ex) 500\r\n 
  if((1 <= rx_size)&&(rx_size <= 4)) 
  {
    loop_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
    
    if((1 <= loop_num)&&(loop_num <= 1200))
    {
      //printf("\r\n\r\nNum    ch1       ch2       ch3      ch4      ch5     ch6[G]   ch7[R]  Temperature\r\n");
      printf("\r\n\r\nNum\tL1\tL2\tL3\tL4\tL5\tL6[G]\tL7[R]\tTemperature\r\n");
      multi_time_test_start = START;
      TIME_BASED.loop_num = loop_num + 1; // 0부터 시작하기 위해 => 1 입력 시 0, 1 동작 위해 +1 해줌.
      TIME_BASED.timer_enable = START;
      //190903 ybw
      //function time check
      USER_MODE_PROCESS.function_time = 0;
      USER_MODE_PROCESS.function_time_flag = START;
      //
    }
    else// 값을 범위내 다시 입력
    {
      printf("\r\n\r\n > Please re-enter the value within range.\r\n");
      printf(" > Input loop value [range: 1 ~ 1200] => ");
    }
  }
  else// 값을 범위내 다시 입력
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Input loop value [range: 1 ~ 1200] => ");
  }
*/
}

void Admin_Cutoff_Content_Select()
{
/*
  char rx_size = 0;
  char input_num = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  input_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((rx_size == 1)&&(1 <= input_num)&&(input_num <= 4))
  {
    content_select = input_num;
    cutoff_select = T1_CUTOFF_SET;
    sub_menu_select = SUB_CUTOFF_DATA_SETTING;
    //최신 Cutoff data 입력 전 MEM_BANK_1 메모리 초기화 (이전 데이터와 겹침 방지)
    if(content_select == CONTENT_PREG){
      memset(&MEM_BANK_1.PREG_CUTOFF_DATA, 0, sizeof(&MEM_BANK_1.PREG_CUTOFF_DATA));
    }
    else if(content_select == CONTENT_OVUL){  
      memset(&MEM_BANK_1.OVUL_CUTOFF_DATA, 0, sizeof(&MEM_BANK_1.OVUL_CUTOFF_DATA));
    }
    else if(content_select == CONTENT_MENO){
      memset(&MEM_BANK_1.MENO_CUTOFF_DATA, 0, sizeof(&MEM_BANK_1.MENO_CUTOFF_DATA));
    }
    else if(content_select == CONTENT_COVID){
      memset(&MEM_BANK_1.COVID_CUTOFF_DATA, 0, sizeof(&MEM_BANK_1.COVID_CUTOFF_DATA));
    }
    printf("\r\n 1) T1 Cutoff\r\n");
    printf(" > Input Cutoff [ex)1000,1200,1300] => ");
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
*/
}

void Admin_Cutoff_Data_Setting()
{
/*  
  uint8_t i = 0;
  uint8_t col = 0;
  uint8_t row = 0;
  uint8_t len = 0;
  uint16_t* p_idx = NULL;
  char error_data = 0;
  char loop_num = 0;
  char tmp_rx[BAND_CUTOFF_DATA_NUM][5] = {0};
  uint8_t rx_size = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;
  
  //수신된 cutoff data 분류
  for(i = 0; i < rx_size; i++) 
  {
    if(BLE_RX.data[i] == ',')
    {  
      col++;
      row = 0;
    }
    else
    {
      tmp_rx[col][row] = BLE_RX.data[i];
      row++;
    }
  }
  //
  //수신 cutoff data 개수 확인
  if((content_select == CONTENT_PREG) && (cutoff_select == T1_CUTOFF_SET)) //Preg, T1만 level 6 단계까지 표시 이므로, cutoff data 5개 필요 (0 ~ 4)
  {
    if(col == (BAND_CUTOFF_DATA_NUM - 1)){
      loop_num = col;
    }
    else{
      error_data = 1;
    }
  }
  else if(col == (BAND_CUTOFF_DATA_NUM - 3)){// Preg의 T1제외한 나머지는 cutoff data 3개 필요 (0 ~ 2) 
    loop_num = col;
  }
  else{
    error_data = 1;
  }
  //  
  
  if(!error_data)
  {
    if(cutoff_select == T1_CUTOFF_SET)
    {
      if(content_select == CONTENT_PREG){
        p_idx = &MEM_BANK_1.PREG_CUTOFF_DATA.T1_CUTOFF[0];
      }
      else if(content_select == CONTENT_OVUL){
        p_idx = &MEM_BANK_1.OVUL_CUTOFF_DATA.T1_CUTOFF[0];
      }
      else if(content_select == CONTENT_MENO){
        p_idx = &MEM_BANK_1.MENO_CUTOFF_DATA.T1_CUTOFF[0];
      }
      else if(content_select == CONTENT_COVID){
        p_idx = &MEM_BANK_1.COVID_CUTOFF_DATA.T1_CUTOFF[0];
      }
      else;

      cutoff_select = T2_CUTOFF_SET;
      sub_menu_select = SUB_CUTOFF_DATA_SETTING;
      printf("\r\n Complete!\r\n");
      printf("\r\n 2) T2 Cutoff\r\n");
      printf(" > Input Cutoff [ex)1000,1200,1300] => ");
    }
    else if(cutoff_select == T2_CUTOFF_SET)
    {
      if(content_select == CONTENT_PREG){
        p_idx = &MEM_BANK_1.PREG_CUTOFF_DATA.T2_CUTOFF[0];
      }
      else if(content_select == CONTENT_OVUL){
        p_idx = &MEM_BANK_1.OVUL_CUTOFF_DATA.T2_CUTOFF[0];
      }
      else if(content_select == CONTENT_MENO){
        p_idx = &MEM_BANK_1.MENO_CUTOFF_DATA.T2_CUTOFF[0];
      }
      else if(content_select == CONTENT_COVID){
        p_idx = &MEM_BANK_1.COVID_CUTOFF_DATA.T2_CUTOFF[0];
      }
      else;
        
      cutoff_select = T3_CUTOFF_SET;
      sub_menu_select = SUB_CUTOFF_DATA_SETTING;
      printf("\r\n Complete!\r\n");
      printf("\r\n 3) T3 Cutoff\r\n");
      printf(" > Input Cutoff [ex)1000,1200,1300] => ");
    }
    else if(cutoff_select == T3_CUTOFF_SET)
    {
      if(content_select == CONTENT_PREG){
        p_idx = &MEM_BANK_1.PREG_CUTOFF_DATA.T3_CUTOFF[0]; 
      }
      else if(content_select == CONTENT_OVUL){
        p_idx = &MEM_BANK_1.OVUL_CUTOFF_DATA.T3_CUTOFF[0];
      }
      else if(content_select == CONTENT_MENO){
        p_idx = &MEM_BANK_1.MENO_CUTOFF_DATA.T3_CUTOFF[0];
      }
      else if(content_select == CONTENT_COVID){
        p_idx = &MEM_BANK_1.COVID_CUTOFF_DATA.T3_CUTOFF[0];
      }
      else;
      
      printf("\r\n Complete!\r\n");
      printf("\r\n Save to user mode area?\r\n");
      printf(" 1) Save\r\n");
      printf(" 2) Exit\r\n");
      printf(" >");
      apply_usermode_type = CUTOFFDATA_SAVE_FACTORYDATA;
      sub_menu_select = SUB_SAVE_FACTORY_DATA;
      //content_select = 0;
    }
    
    for(i = 0; i <= loop_num; i++)
    {
      len = strlen(&tmp_rx[i][0]);
      *p_idx = String_To_Decimal(&tmp_rx[i][0], len);
       p_idx++;
    }
  }
  else
  {
    //Error 조건
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Input Cutoff [ex)1000,1200,1300] => ");
  }
*/
}

void Admin_Read_Stick_Rawdata()
{
/*  
    MEMORY_BANK MEM_BANK_3 = {0,}; // 임시 open MEM_BANK_3
    load_meLoad_Memory_Bankmory(MEMORY_ADDR_1, &MEM_BANK_3); //Main 영역 저장 data load

    char i = 0;
    float line_ratio[8] = {0};
    float T1_line = 0;
    float T2_line = 0;
    float T3_line = 0;

    //LED calibration, PTR data load
    for(i = 1; i <= 7; i++)
    {
      calibration_data[i] = MEM_BANK_3.CALIBRATION_DATA[i];
      if(i <= 5){
        initial_band_sensing_data[i] = MEM_BANK_3.INITIAL_STICK_SENSING_DATA[i];
      }
      else{
        initial_color_sensing_data[i - 6] = MEM_BANK_3.INITIAL_STICK_SENSING_DATA[i];
      }
    }
    //발색 stick ratio 계산
    PTR_Sensing_Bandline(recent_band_sensing_data);
    PTR_Sensing_Colorline(recent_color_sensing_data);
    
    for(i = 1; i <= 7; i++)
    {
      if(i <= 5){
        line_ratio[i] = ((float)recent_band_sensing_data[i] / (float)initial_band_sensing_data[i]) * 100.0;
      }
      else{
        line_ratio[i] = ((float)recent_color_sensing_data[i - 6] / (float)initial_color_sensing_data[i - 6]) * 100.0;
      }
    }
    //

    //측정 결과 계산 Raw data
    T1_line = ((float)line_ratio[2] / (float)line_ratio[1]) * 1000.0;
    T2_line = ((float)line_ratio[2] / (float)line_ratio[3]) * 1000.0;
    T3_line = ((float)line_ratio[4] / (float)line_ratio[5]) * 1000.0;
    //
    printf("\r\nLine\t\tL1\tL2\tL3\tL4\tL5\tL6[G]\tL7[R]\r\n");
    
    printf("PTR (initial)\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n", initial_band_sensing_data[1], initial_band_sensing_data[2], 
          initial_band_sensing_data[3], initial_band_sensing_data[4], initial_band_sensing_data[5], initial_color_sensing_data[0], initial_color_sensing_data[1]);
    
    printf("PTR (recent)\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n", recent_band_sensing_data[1], recent_band_sensing_data[2], 
          recent_band_sensing_data[3], recent_band_sensing_data[4], recent_band_sensing_data[5], recent_color_sensing_data[0], recent_color_sensing_data[1]);
    
    printf("Ratio\t\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\r\n", line_ratio[1], line_ratio[2], line_ratio[3], line_ratio[4], line_ratio[5], 
           line_ratio[6], line_ratio[7]);

    printf("\r\nResult\tT1\tT2\tT3\r\n");
    printf("Data\t%0.1f\t%0.1f\t%0.1f\r\n", T1_line, T2_line, T3_line);

    printf("\r\n > Enter the menu number => ");
    Admin_Variable_Init();
*/
}

void Admin_Reacttime_Content_Select()
{
/*  
  char rx_size = 0;
  char input_num = 0;
  char content_name[5] = {0};
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  input_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((rx_size == 1) && (1 <= input_num) && (input_num <= 4))
  {
    switch(input_num)
    {
      case 1 ://Preg
        content_select = CONTENT_PREG;
        u_strncpy(content_name, "preg", sizeof(content_name));
        break;
      case 2 : //Ovul
        content_select = CONTENT_OVUL;
        u_strncpy(content_name, "ovul", sizeof(content_name));
        break;
      case 3 : //Meno
        content_select = CONTENT_MENO;
        u_strncpy(content_name, "meno", sizeof(content_name));
        break;
      case 4 : //Covid
        content_select = CONTENT_COVID;
        u_strncpy(content_name, "covid", sizeof(content_name));
        break;  
    }
    printf("\r\nInput %s react time (sec) => ", content_name);
    sub_menu_select = SUB_REACT_TIME_SETTING;
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
*/  
}

void Admin_Reacttime_Setting()
{
/*  
  char rx_size = 0;
  int input_num = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  input_num = (int)String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((1 <= rx_size) && (rx_size <= 4) && (0 < input_num) && (input_num <= 3600))
  {
    switch(content_select)
    {
      case CONTENT_PREG ://Preg
        MEM_BANK_1.PREG_REACT_TIME = input_num;
        break;
      case CONTENT_OVUL : //Ovul
        MEM_BANK_1.OVUL_REACT_TIME = input_num;
        break;
      case CONTENT_MENO : //Meno
        MEM_BANK_1.MENO_REACT_TIME = input_num;
        break;
      case CONTENT_COVID : //Meno
        MEM_BANK_1.COVID_REACT_TIME = input_num;
        break;  
    }
    printf("\r\n Save to user mode area?\r\n");
    printf(" 1) Save\r\n");
    printf(" 2) Exit\r\n");
    printf(" >");
    //content_select = 0;
    sub_menu_select = SUB_SAVE_FACTORY_DATA;
    apply_usermode_type = REACT_TIME_SAVE_FACTORYDATA;
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
*/  
}

void Admin_Correction_Factor_Content_Select()
{
/*  
  char rx_size = 0;
  char input_num = 0;
  char content_name[5] = {0};
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  input_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((rx_size == 1) && (1 <= input_num) && (input_num <= 3))
  {
    switch(input_num)
    {
      case 1 ://Preg
        content_select = CONTENT_PREG;
        u_strncpy(content_name, "preg", sizeof(content_name));
        break;
      case 2 : //Ovul
        content_select = CONTENT_OVUL;
        u_strncpy(content_name, "ovul", sizeof(content_name));
        break;
      case 3 : //Meno
        content_select = CONTENT_MENO;
        u_strncpy(content_name, "meno", sizeof(content_name));
        break;
    }
    printf("\r\n Input %s correction factor => ", content_name);
    sub_menu_select = SUB_CORRECTION_FACTOR_SETTING;
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
*/
}
void Admin_Correction_Factor_Setting()
{
/*
  char rx_size = 0;
  int input_num = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  input_num = (int)String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((1 <= rx_size) && (rx_size <= 4) && (0 < input_num) && (input_num <= 9999))
  {
    switch(content_select)
    {
      case CONTENT_PREG ://Preg
        MEM_BANK_1.PREG_CORRECTION_FACTOR = input_num;
        break;
      case CONTENT_OVUL : //Ovul
        MEM_BANK_1.OVUL_CORRECTION_FACTOR = input_num;
        break;
      case CONTENT_MENO : //Meno
        MEM_BANK_1.MENO_CORRECTION_FACTOR = input_num;
        break;
    }
    printf("\r\n Save to user mode area?\r\n");
    printf(" 1) Save\r\n");
    printf(" 2) Exit\r\n");
    printf(" >");
    //content_select = 0;
    sub_menu_select = SUB_SAVE_FACTORY_DATA;
    apply_usermode_type = CORRECTION_FACTOR_SAVE_FACTORYDATA;
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
*/
}

void Admin_Wireless_FW_Update()
{
/*  
  char rx_size = 0;
  char input_num = 0;
  char mac_id[13] = {0};
  char i = 0;
  char j = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  input_num = String_To_Decimal(BLE_RX.data, strlen(BLE_RX.data)-REMOVE_CR_LF);
  if((rx_size == 1)&&(1 <= input_num)&&(input_num <= 2))
  {
    switch(input_num)
    {
      case 1 ://Start firmware updata
        for(i = 0; i <= 16; i++)
        {
          if(ble_mac_id[i] != ':')
          {
             mac_id[j] = ble_mac_id[i];
             j++;
          }          
        }
        printf("\r\nStart F/W update\r\n");
        printf("1) Jump to 0x810B\r\n");
        printf("2) After jump, try connecting with the command below.\r\n");
        printf("3) AT+CONMAC=%s\r\n",mac_id);
        printf("4) Download to the ST demonstrator program\r\n");
        printf("Jump!!\r\n");
        FLASH_Unlock(FLASH_MemType_Data);
        FLASH_ProgramByte(CHECK_W_FIRMWARE_UPDATE_FLAG, (uint8_t)0x01);
        FLASH_Lock(FLASH_MemType_Data);
        asm("LDW X,  SP ");
        asm("LD  A,  $FF");
        asm("LD  XL, A  ");
        asm("LDW SP, X  ");
        asm("JPF $810B");
        break;
      case 2 : //Cancel
        printf("\r\nCancel F/W update.\r\n");
        printf("\r\n > Enter the menu number => ");
        Admin_Variable_Init();
        break;
    }    
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" > Enter the sub menu number => ");
  }
*/
}

void Admin_BLE_Name_Setting()
{
/*  
  char rx_size = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  
  if(rx_size <= (BLE_NAME_LEN - 1))
  {
    memcpy(MEM_BANK_1.BLE_NAME, BLE_RX.data, rx_size);
    printf("\r\n Save to user data area?\r\n");
    printf(" 1) Save & Apply (BLE will be restarted.)\r\n");
    printf(" 2) Exit\r\n");
    printf(" >");
    sub_menu_select = SUB_SAVE_FACTORY_DATA;
    apply_usermode_type = BLE_NAME_SAVE_FACTORYDATA;
  }
  else
  {
    printf("\r\n\r\n > Please re-enter the value within range.\r\n");
    printf(" >");
  }
*/
}

void Admin_Multi_Time_Test_Display()
{
/*  
  static int loop_cnt = 0;    
  char i = 0;  
  float cal_ptr_ratio = 0;
  
  TIME_BASED.loop_num--;
  if(!TIME_BASED.loop_num){ // 마지막 loop에서 function timer stop => 정확한 1초간격 시간 표현 위해
    USER_MODE_PROCESS.function_time_flag = END;
  }
  PTR_Sensing_Bandline(recent_band_sensing_data);
  PTR_Sensing_Colorline(recent_color_sensing_data);
  float mcu_temperature = mcu_temperature_read(); //MCU temperature sensing
  
  printf("%4d\t" ,loop_cnt); // 0부터 표시하기 위해 => 1입력 시 0, 1 순으로 표시
  
  for(i = 1; i <= 7; i++)
  {
    if(i <= 5){
      cal_ptr_ratio = ((float)recent_band_sensing_data[i] / (float)initial_band_sensing_data[i]) * 100.0;
    }
    else{
      cal_ptr_ratio = ((float)recent_color_sensing_data[i-6] / (float)initial_color_sensing_data[i-6]) * 100.0;
    }
    printf("%.2f%%\t", cal_ptr_ratio);
  }
  //
  //MCU tmperature
  printf("%.1f C\t", mcu_temperature);
  //
  printf("\r\n");
  loop_cnt++;
  multi_time_test_start = END;
  if((!TIME_BASED.loop_num)||(TIME_BASED.stop_operation)) //Loop END
  {
    //function 진행 시간 출력
    function_time_display();
    Next_User_Mode_Process(MAIN_MENU_VIEW);
    //
    OP_Amp_Disable();
    printf("\r\nEND.\r\n");      
    printf("\r\n > Enter the menu number => ");
    // 사용 변수 초기화
    loop_cnt = 0;
    Admin_Variable_Init();
  }
*/
}

/*
void admin_one_time_display() // BLE 연결 후 ONE TIME MENU 선택 시 초기 화면 출력
{
  printf("\r\n\r\n*** One-time test START ***\r\n\r\n");
  //printf("\r\n\r\nNum    ch1       ch2       ch3      ch4      ch5     ch6[G]   ch7[R]  Temperature\r\n");
  printf("\r\n\r\nNum\tL1\tL2\tL3\tL4\tL5\tL6[G]\tL7[R]\tTemperature\r\n");
  TIME_BASED.loop_num = 2; // 0번째 출력 위해 2번 진행 (0, 1 출력)
  TIME_BASED.timer_enable = START;
  multi_time_test_start = START;
  //190903 ybw
  //function time check
  USER_MODE_PROCESS.function_time = 0;
  USER_MODE_PROCESS.function_time_flag = START;
  //
}
*/
/*
void admin_color_test()
{
  USER_MODE_PROCESS.function_time = 0;
  USER_MODE_PROCESS.function_time_flag = START;
  char i = 0;
  float cal_ptr_ratio = 0;
  printf("\r\n\r\n*** Color test START ***\r\n\r\n");
  PTR_Sensing_Colorline(recent_color_sensing_data);
  printf("Line\tL6(Green)\tL7(Red)\r\n");
  printf("Ratio\t");
  for(i = 0; i <= 1; i++)
  {
    cal_ptr_ratio = ((float)recent_color_sensing_data[i] / (float)initial_color_sensing_data[i]) * 100.0;
    printf("%.2f%%\t\t", cal_ptr_ratio);
  }
  printf("\r\n");
  printf("\r\n*** Color test END ***\r\n");
  //
  function_time_display();
  Next_User_Mode_Process(MAIN_MENU_VIEW);
  //        
  printf("\r\n > Enter the menu number => ");
  Admin_Variable_Init();
}
*/
/*
void admin_rtc_date_settting()
{
  char rx_size = 0;
  char i = 0;
  uint8_t tmp_date[3] = {0};
  
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 191225\r\n (19.12.25)
  if(rx_size == 6)
  {
    for(i = 0; i < 3; i++)
      tmp_date[i] = ((BLE_RX.data[2*i] - 0x30)*10) + (BLE_RX.data[(2*i)+1] - 0x30);
    
    RTC_Date_Set.RTC_Year = tmp_date[0];
    
    if(tmp_date[1] < 10)
      RTC_Date_Set.RTC_Month = (RTC_Month_TypeDef)tmp_date[1];
    else
    {
      if(tmp_date[1] == 10)
        RTC_Date_Set.RTC_Month = RTC_Month_October;
      else if(tmp_date[1] == 11)
        RTC_Date_Set.RTC_Month = RTC_Month_November;
      else if(tmp_date[1] == 12)
        RTC_Date_Set.RTC_Month = RTC_Month_December;
    }
    RTC_Date_Set.RTC_Date = tmp_date[2];

    RTC_SetDate(RTC_Format_BIN, &RTC_Date_Set);
    
    printf(" > Input Hour, Minute, Second value [ ex)12:34:56 -> 123456 ] => ");
    sub_menu_select = SUB_RTC_TIME_SETTING;
  }
  else
  {
    printf("\r\n\r\n%s\r\n, re_enter_msg");
    printf(" > Input Year, Month, Date value [ ex)2019.12.25 -> 191225 ] => ");
  }
}
*/
/*
void admin_rtc_time_settting()
{
  char rx_size = 0;
  char i = 0;
  uint8_t tmp_time[3] = {0};
  
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;// ex) 값 121005\r\n (12:10:05)
  if(rx_size == 6)
  {
    for(i = 0; i < 3; i++)
      tmp_time[i] = ((BLE_RX.data[2*i] - 0x30)*10) + (BLE_RX.data[(2*i)+1] - 0x30);
    RTC_Time_Set.RTC_Hours = tmp_time[0];
    RTC_Time_Set.RTC_Minutes = tmp_time[1];
    RTC_Time_Set.RTC_Seconds = tmp_time[2];
    
    RTC_SetTime(RTC_Format_BIN, &RTC_Time_Set);
    
    Admin_Variable_Init();
    printf("\r\n\r\n*** Device time setting END ***\r\n\r\n");
    printf("\r\n > Enter the menu number => ");
  }
  else
  {
    printf("\r\n\r\n%s\r\n, re_enter_msg");
    printf(" > Input Hour, Minute, Second value [ ex)12:34:56 -> 123456 ] => ");
  }
}
*/
/*
void admin_color_cutoff_setting()
{
  uint8_t i = 0;
  uint8_t col = 0;
  uint8_t row = 0;
  uint8_t len = 0;
  uint16_t* p_idx = 0;
  char tmp_rx[CONTENT_CUTOFF_DATA_NUM][5] = {0};
  uint8_t rx_size = 0;
  rx_size = strlen(BLE_RX.data) - REMOVE_CR_LF;
  
  for(i = 0; i < rx_size; i++)
  {
    if(BLE_RX.data[i] == ',')
    {  
      col++;
      row = 0;
    }
    else
    {
      tmp_rx[col][row] = BLE_RX.data[i];
      row++;
    }
  }
  
  if(col == CONTENT_CUTOFF_DATA_NUM - 1)
  {
    if(content_select == CONTENT_PREG)
      p_idx = &MEM_BANK_2.PREG_CUTOFF_DATA.COLOR_CUTOFF.level_0;
    else if(content_select == CONTENT_OVUL)
      p_idx = &MEM_BANK_2.OVUL_CUTOFF_DATA.COLOR_CUTOFF.level_0;
    else if(content_select == CONTENT_MENO)
      p_idx = &MEM_BANK_2.MENO_CUTOFF_DATA.COLOR_CUTOFF.level_0;
    
    for(i = 0; i < CONTENT_CUTOFF_DATA_NUM; i++)
    {
      len = strlen(&tmp_rx[i][0]);
      *p_idx = String_To_Decimal(&tmp_rx[i][0], len);
       p_idx++;
    }
    MEM_BANK_2.ACCESS_ADDR_0 = SET;
    //Save_Memory_Bank_Byte_Program(MEMORY_ADDR_0, &MEM_BANK_2);
    printf("\r\n Complete!\r\n");
    printf("\r\n > Enter the menu number => ");
    Admin_Variable_Init();
  }
  else
  {
    //Error 조건
    printf("\r\n\r\n%s\r\n, re_enter_msg");
    printf(" > Enter the sub menu number => ");
  }
}
*/

/*
void admin_read_receive_buffer()
{
  uint8_t i = 0;
  uint8_t j = 0;
  const uint8_t row = 64;
  printf("\r\n\r\n*** Read receive buffer START ***\r\n\r\n");
  printf("\r\n Queue information\r\n");
  printf(" *Front: %d\r\n", BLE_BOT_QUEUE.front);
  printf(" *Rear: %d\r\n\r\n", BLE_BOT_QUEUE.rear);

  printf("Index\tv(0)\tv(8)\tv(16)\tv(24)\tv(32)\tv(40)\tv(48)\tv(56)");

  for(i = 0; i <= (int)(QUE_LEN/row); i++)
  {
    printf("\r\n\r\n%d~%d\t",(row * i), (row * (i+1)) - 1); //0~63
    for(j = 0; j < 64; j++)
    {
      if((0x21 <= BLE_BOT_QUEUE.queArr[(row*i)+j])&&(BLE_BOT_QUEUE.queArr[(row*i)+j] <= 0x7E))
        printf("%c", BLE_BOT_QUEUE.queArr[(row*i)+j]);
      else 
        printf(" ");      
    }
  }
  
  Admin_Variable_Init();
  printf("\r\n\r\n > Enter the menu number => ");
}
*/
/*
void admin_current_time()
{
  Calendar_Read(&RTC_Date_Read, &RTC_Time_Read);
  
  printf("Date: %d.%d.%d / Time: %d:%d:%d\r\n", RTC_Date_Read.RTC_Year, RTC_Date_Read.RTC_Month, RTC_Date_Read.RTC_Date, 
          RTC_Time_Read.RTC_Hours, RTC_Time_Read.RTC_Minutes, RTC_Time_Read.RTC_Seconds);
  TIME_BASED.loop_num--;
  current_time_test_start = END;      

  //if((TIME_BASED.loop_cnt == TIME_BASED.loop_num)||(TIME_BASED.stop_operation)) //Loop END
  if((!TIME_BASED.loop_num)||(TIME_BASED.stop_operation)) //Loop END
  {
    printf("\r\n > Enter the menu number => ");
    // 사용 변수 초기화
    Admin_Variable_Init();
  }  
}
*/