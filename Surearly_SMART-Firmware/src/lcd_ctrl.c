#include "main.h"

LCD_COM COM0;
LCD_COM COM1;
LCD_COM COM2;
LCD_COM COM3;

char lcd_blink_state = 0;
static char lcd_status_setup_block = 0;

void LCD_Initialization()
{
  CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);   
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);
  LCD_Init(LCD_Prescaler_1, LCD_Divider_18, LCD_DUTY, LCD_BIAS, LCD_VoltageSource_Internal);
  LCD_PortMaskConfig(LCD_PORT_MASK, LCD_PORT_MASK_DATA);//Select SEG0 ~ 4
  LCD_PageSelect(LCD_PAGE_COM);
  LCD_ContrastConfig(LCD_CONTRAST);
  LCD_DeadTimeConfig(LCD_DeadTime_0);
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);
  LCD_Cmd(ENABLE);
}

void LCD_Deinitialization()
{
  LCD_DeInit();
  delay_ms(10); //LCD_DeInit() 후 10 msec 지연 추가: Icon 잔상 문제 해결
  LCD_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_Off, CLK_RTCCLKDiv_1);
  CLK_LSICmd(DISABLE);

/*
  GPIO_Init(GPIOA, M_LCD_COM0, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOA, M_LCD_COM1, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOA, M_LCD_COM2, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOD, M_LCD_COM3, GPIO_Mode_In_PU_No_IT);
  //
  GPIO_Init(GPIOA, M_LCD_SEG0, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOE, M_LCD_SEG1, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOE, M_LCD_SEG2, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOE, M_LCD_SEG3, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOE, M_LCD_SEG4, GPIO_Mode_In_PU_No_IT);
*/
  //COM0~3, SEG0~4 pin의 output open-drain reset status 설정: LCD에 접촉 시 소비 전류 증가 완화
  GPIO_Init(GPIOA, M_LCD_COM0, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOA, M_LCD_COM0, RESET);
  GPIO_Init(GPIOA, M_LCD_COM1, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOA, M_LCD_COM1, RESET);
  GPIO_Init(GPIOA, M_LCD_COM2, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOA, M_LCD_COM2, RESET);
  GPIO_Init(GPIOD, M_LCD_COM3, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOD, M_LCD_COM3, RESET);

  GPIO_Init(GPIOA, M_LCD_SEG0, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOA, M_LCD_SEG0, RESET);
  GPIO_Init(GPIOE, M_LCD_SEG1, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOE, M_LCD_SEG1, RESET);
  GPIO_Init(GPIOE, M_LCD_SEG2, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOE, M_LCD_SEG2, RESET);
  GPIO_Init(GPIOE, M_LCD_SEG3, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOE, M_LCD_SEG3, RESET);
  GPIO_Init(GPIOE, M_LCD_SEG4, GPIO_Mode_Out_OD_Low_Slow); GPIO_WriteBit(GPIOE, M_LCD_SEG4, RESET);
  //
}

/*PORT MAP
  LCD_COM0      PA4
  LCD_COM1      PA5
  LCD_COM2      PA6
  LCD_COM3      PD1
  LCD_SEG0      PA7
  LCD_SEG1      PE0
  LCD_SEG2      PE1
  LCD_SEG3      PE2
  LCD_SEG4      PE3
*/

/*LCD RAM REGISTER LIST
  LCD_RAMRegister_0     < COM0 - SEG0~7
  LCD_RAMRegister_3     < COM1 - SEG0~3
  LCD_RAMRegister_4     < COM1 - SEG4~11
  LCD_RAMRegister_7     < COM2 - SEG0~7
  LCD_RAMRegister_10    < COM3 - SEG0~3
  LCD_RAMRegister_11    < COM3 - SEG4~11
*/




/*
@param  icon
  *     This parameter can be one of the following values:
  *     @arg Display icon : BLE, NFC, BATTERY_LOW...
@param  option
  *     This parameter can be one of the following values:
  *     @arg Display option : ICON_OFF, ICON_ON, ICON_BLINK
*/
void LCD_Display(uint16_t icon, uint8_t option)
{
  lcd_status_setup_block = LCD_STATUS_SETUP_BUSY;
  
  uint8_t seg_data = (icon & 0x00ff);
  if((icon == INSERT)||(icon == DROP)||(icon == BOOK))//COM0_ICON
  {
    if(option == ICON_OFF){
      COM0.seg = COM0.seg &(~seg_data);
    }
    else if(option == ICON_ON){
      COM0.seg |= seg_data;
    }
    else if(option == BLINK_OFF)
    {
      COM0.seg = COM0.seg &(~seg_data); //ICON 'Off'
      COM0.blink_data = COM0.blink_data &(~seg_data); //Blink state 'Off'
      if(COM0.blink_data == 0){
        COM0.blink_onoff = BLINK_OFF;
      }
    }
    else if(option == BLINK_ON)
    {
      COM0.blink_data |= seg_data;
      COM0.blink_onoff = BLINK_ON;
    }
    else{}
    
    LCD_WriteRAM(LCD_RAMRegister_0, COM0.seg);//COM0 - SEG0~7
  }
  if((icon == PLUS_1)||(icon == PLUS_2)||(icon == PLUS_3)||(icon == MINUS))//COM1_ICON
  {
    if(option == ICON_OFF){
      COM1.seg = COM1.seg &(~seg_data);
    }
    else if(option == ICON_ON){
      COM1.seg |= seg_data;
    }
    else if(option == BLINK_OFF)
    {
       COM1.seg = COM1.seg &(~seg_data); //ICON 'Off'
      COM1.blink_data = COM1.blink_data &(~seg_data); //Blink state 'Off'
      if(COM1.blink_data == 0){
        COM1.blink_onoff = BLINK_OFF;
      }
    }
    else if(option == BLINK_ON)
    {
      COM1.blink_data |= seg_data;
      COM1.blink_onoff = BLINK_ON;
    }
    else{}

    LCD_WriteRAM(LCD_RAMRegister_3, COM1.seg);//COM1 - SEG0~3
  }
  if((icon == EJECT)||(icon == SQUARE_1)||(icon == SQUARE_2)||(icon == SQUARE_3)||(icon == SQUARE_4))//COM2_ICON
  {
    if(option == ICON_OFF){
      COM2.seg = COM2.seg &(~seg_data);
    }
    else if(option == ICON_ON){
      COM2.seg |= seg_data;
    }
    else if(option == BLINK_OFF)
    {
      COM2.seg = COM2.seg &(~seg_data); //ICON 'Off'
      COM2.blink_data = COM2.blink_data &(~seg_data); //Blink state 'Off'
      if(COM2.blink_data == 0){
        COM2.blink_onoff = BLINK_OFF;
      }
    }
    else if(option == BLINK_ON)
    {
      COM2.blink_data |= seg_data;
      COM2.blink_onoff = BLINK_ON;
    }
    else{}
    
    LCD_WriteRAM(LCD_RAMRegister_7, COM2.seg);//COM2 - SEG0~7
  }
  if((icon == BLE)||(icon == NFC)||(icon == BATTERY_LOW)||(icon == BATTERY_MID)||(icon == BATTERY_HIGH))//COM3_ICON_LOW
  {
    if(option == ICON_OFF){
      COM3.seg = COM3.seg &(~seg_data);
    }
    else if(option == ICON_ON){
      COM3.seg |= seg_data;
    }
    else if(option == BLINK_OFF)
    {
      COM3.seg = COM3.seg &(~seg_data); //ICON 'Off'
      COM3.blink_data = COM3.blink_data &(~seg_data); //Blink state 'Off'
      if(COM3.blink_data == 0){
        COM3.blink_onoff = BLINK_OFF;
      }
    }
    else if(option == BLINK_ON)
    {
      COM3.blink_data |= seg_data;
      COM3.blink_onoff = BLINK_ON;
    }
    else{}
    
    if(icon == BLE){
      LCD_WriteRAM(LCD_RAMRegister_11, COM3.seg);//COM3 - SEG4~11
    }
    else{
      LCD_WriteRAM(LCD_RAMRegister_10, COM3.seg&0xf0);//COM3 - SEG0~3    
    }
  }
  else{}

  lcd_status_setup_block = LCD_STATUS_SETUP_COMPLETE;
}


void LCD_Blink_Timer()
{
  if(lcd_status_setup_block == LCD_STATUS_SETUP_COMPLETE)
  {    
    if(lcd_blink_state == BLINK_STATE_ON)
    {
      lcd_blink_state = BLINK_STATE_OFF;
      if(COM0.blink_onoff == BLINK_ON)
      {
        COM0.seg = COM0.seg | COM0.blink_data;
        LCD_WriteRAM(LCD_RAMRegister_0, COM0.seg);//COM0 - SEG0~7
      }
      if(COM1.blink_onoff == BLINK_ON)
      {
        COM1.seg = COM1.seg | COM1.blink_data;
        LCD_WriteRAM(LCD_RAMRegister_3, COM1.seg);//COM1 - SEG0~3
      }
      if(COM2.blink_onoff == BLINK_ON)
      {
        COM2.seg = COM2.seg | COM2.blink_data;
        LCD_WriteRAM(LCD_RAMRegister_7, COM2.seg);//COM2 - SEG0~7
      }
      if(COM3.blink_onoff == BLINK_ON)
      {
        COM3.seg = COM3.seg | COM3.blink_data;      
        LCD_WriteRAM(LCD_RAMRegister_11, (COM3.seg & 0x0f));//COM3 - SEG4~11
        LCD_WriteRAM(LCD_RAMRegister_10, (COM3.seg & 0xf0));//COM3 - SEG0~3
      }
    }
    else if(lcd_blink_state == BLINK_STATE_OFF)
    {
      lcd_blink_state = BLINK_STATE_ON;
      if(COM0.blink_onoff == BLINK_ON)
      {
        COM0.seg = COM0.seg ^ COM0.blink_data;
        LCD_WriteRAM(LCD_RAMRegister_0, COM0.seg);//COM0 - SEG0~7
      }
      if(COM1.blink_onoff == BLINK_ON)
      {
        COM1.seg = COM1.seg ^ COM1.blink_data;
        LCD_WriteRAM(LCD_RAMRegister_3, COM1.seg);//COM1 - SEG0~3
      }
      if(COM2.blink_onoff == BLINK_ON)
      {
        COM2.seg = COM2.seg ^ COM2.blink_data;
        LCD_WriteRAM(LCD_RAMRegister_7, COM2.seg);//COM2 - SEG0~7
      }
      if(COM3.blink_onoff == BLINK_ON)
      {
        COM3.seg = COM3.seg ^ COM3.blink_data;
        LCD_WriteRAM(LCD_RAMRegister_11, (COM3.seg & 0x0f));//COM3 - SEG4~11
        LCD_WriteRAM(LCD_RAMRegister_10, (COM3.seg & 0xf0));//COM3 - SEG0~3
      }
    }
    else{}
  }
}


const uint16_t icon_array[17] = {0, EJECT, BLE, BATTERY_LOW, BATTERY_MID, BATTERY_HIGH, SQUARE_1, SQUARE_2, SQUARE_3, SQUARE_4, PLUS_1, PLUS_2, PLUS_3, MINUS, INSERT, DROP, BOOK};


void LCD_All_Off()
{
  char idx = 0;
  for(idx = 1; idx <= ICON_TOTAL_NUM; idx++)
  {
    //LCD_Display(icon_array[idx], ICON_OFF);
    LCD_Display(icon_array[idx], BLINK_OFF);
  }
  //LCD_Display(NFC, ICON_OFF);
  LCD_Display(NFC, BLINK_OFF);
}

void LCD_All_On()
{
  char idx = 0;
  for(idx = 1; idx <= ICON_TOTAL_NUM; idx++)
  {
    LCD_Display(icon_array[idx], ICON_ON);
  }
}

void LCD_Power_On_Action_1()//������� ICON ON �� ALL OFF
{
  char idx = 0;
  for(idx = 1; idx <= ICON_TOTAL_NUM; idx++)
  {
    LCD_Display(icon_array[idx], ICON_ON);
    delay_ms(100);
  }
  delay_ms(1000);
  for(idx = 1; idx <= ICON_TOTAL_NUM; idx++)
  {
    LCD_Display(icon_array[idx], ICON_OFF);
  }
}

void LCD_Power_Off_Action_1()
{
  char idx = 0;
  LCD_All_Off();
  delay_ms(10);
  LCD_All_On();
  delay_ms(500);
  for(idx = ICON_TOTAL_NUM; idx > 0; idx--)
  {
    //LCD_Display(icon_array[idx], ICON_OFF);
    LCD_Display(icon_array[idx], BLINK_OFF);
    delay_ms(100);
  }
  //LCD_Display(NFC, ICON_OFF);
  LCD_Display(NFC, BLINK_OFF);
}

void LCD_Event_All_Icon_Blink(uint16_t num, uint32_t interval_time_ms) //Icon 전체 3번 깜박임
{
  uint16_t idx = 0;
  LCD_All_Off();
  delay_ms(interval_time_ms);
  for(idx = 0; idx < num; idx++)
  {
    LCD_All_On();
    delay_ms(interval_time_ms);
    LCD_All_Off();
    delay_ms(interval_time_ms);
  }
}

void LCD_Result_Icon_Display(char level)
{
  char i = 0;
  if(level == MINUS_ICON)
  {
    for(i = 6; i <= 13; i++)
    {
      if((i == 9) || (i == 13)){
        LCD_Display(icon_array[i], ICON_ON);//SQUARE_4, MINUS
      }
      else{
        LCD_Display(icon_array[i], ICON_OFF);//SQUARE_1~3, PLUS_1~3
      }
    }
  }
  else if(level == PLUS_1_ICON)
  {
    for(i = 6; i <= 13; i++)
    {
      if((i == 6) || (i == 10)){
        LCD_Display(icon_array[i], ICON_ON);//SQUARE_1, PLUS_1
      }
      else{
        LCD_Display(icon_array[i], ICON_OFF);//SQUARE_2~4, PLUS_2~3, MINUS
      }
    }
  }
  else if(level == PLUS_2_ICON)
  {
    for(i = 6; i <= 13; i++)
    {
      if((i == 6) || (i == 7) || (i == 10) || (i == 11)){
        LCD_Display(icon_array[i], ICON_ON);//SQUARE_1~2, PLUS_1~2
      }
      else{
        LCD_Display(icon_array[i], ICON_OFF);//SQUARE_3~4, PLUS_3, MINUS
      }
    }
  }
  else if(level == PLUS_3_ICON)
  {
    for(i = 6; i <= 13; i++)
    {
      if((i == 9) || (i == 13)){
        LCD_Display(icon_array[i], ICON_OFF);//SQUARE_4, MINUS
      }
      else{
        LCD_Display(icon_array[i], ICON_ON);//SQUARE_1~3, PLUS_1~3
      }
    }
  }
  else if(level == ERROR_ICON)
  {
    LCD_Display(BOOK, BLINK_ON);
  }
}

void LCD_Result_Display_Off()
{
  char i = 0;
  for(i = 6; i <= 13; i++)//SQUARE_1~SQUARE_4, PLUS_1~PLUS_3, MINUS -> ICON_OFF
  {
    //LCD_Display(icon_array[i], ICON_OFF);
    LCD_Display(icon_array[i], BLINK_OFF);
  }
  //LCD_Display(BOOK, BLINK_OFF);
  //delay_ms(1);
  //LCD_Display(BOOK, ICON_OFF);  

  LCD_Display(BOOK, BLINK_OFF);
}

void LCD_Lower_Icon_Off()
{
  char i = 0;
  for(i = 14; i <= 16; i++)//INSERT, DROP, BOOK -> BLINK_OFF, ICON_OFF
  {
    LCD_Display(icon_array[i], BLINK_OFF);
    //LCD_Display(icon_array[i], ICON_OFF);
  }
}

void BLE_Icon_Display()
{
  if(BLE_FLAG.ble_icon_update == SET)
  {
    BLE_FLAG.ble_icon_update = RESET;
    if(BLE_FLAG.connect_status == BLE_CONNECTED)
    {
      LCD_Display(BLE, BLINK_OFF); //이전 상태가 BLINK 상태일 수 있으므로 BLINK OFF
      LCD_Display(BLE, ICON_ON); //BLE 가 Connection 된 이후 BLE ICON 상시 ON
      //BLE 연결시 +CONNECTED[MAC Address] 메시지 제거 위해
      memset(BLE_RX.data, 0, sizeof(BLE_RX.data));
      ble_rx_index = 0;
      BLE_RX.at_cmd_resp_chk = 0;
      //
    }
    else if(BLE_FLAG.connect_status == BLE_DISCONNECT)
    {
      LCD_Display(BLE, ICON_OFF); // 이전 상태가 상시 ON 상태일 수 있으므로 ICON OFF
      LCD_Display(BLE, BLINK_ON); // BLE가 켜져있고 연결 대기 상태 이므로 BLINK ON
    }
  }
}
/*
void lcd_on_off_test()
{
  char idx = 0;
  for(idx = 1; idx <= ICON_TOTAL_NUM; idx++)
  {
    LCD_Display(icon_array[idx], ICON_ON);
    delay_ms(100);
  }
  delay_ms(500);
  for(idx = ICON_TOTAL_NUM; idx > 0; idx--)
  {
    LCD_Display(icon_array[idx], ICON_OFF);
    delay_ms(100);
  }
}

void lcd_blink_test()
{
  char idx = 0;
  for(idx = 1; idx <= ICON_TOTAL_NUM; idx++)
  {
    LCD_Display(icon_array[idx], BLINK_ON);
  }
  delay_ms(2000);
  
  for(idx = ICON_TOTAL_NUM; idx > 0; idx--)
  {
    LCD_Display(icon_array[idx], BLINK_OFF);
  }
}
*/