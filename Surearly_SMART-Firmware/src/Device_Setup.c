#include "main.h"

SWITCH_CTRL_Typedef POWER_SWITCH = {0,};
SWITCH_CTRL_Typedef STRIP_CHECK_SWITCH = {0,};

volatile char dev_power_status = 0;
char forced_power_down = 0;

char mcu_uid[25] = {0};

void User_MCU_Clock_Init();
void User_GPIO_Init();
void USER_TIM2_Init();
void USER_TIM3_Init();
void USER_TIM4_Init();

void User_MCU_Clock_Init()
{
  /* Select HSI as system clock source */
  CLK->SWCR |= CLK_SWCR_SWEN;
  CLK->SWR = (uint8_t)CLK_SYSCLKSource_HSI;
  /* system clock 1 Mhz setup*/
  //CLK->CKDIVR = (uint8_t)(CLK_SYSCLKDiv_16);
  /* system clock 4 Mhz setup*/
  CLK->CKDIVR = (uint8_t)(CLK_SYSCLKDiv_4);
  CLK_LSICmd(ENABLE);
  CLK_LSEConfig(CLK_LSE_OFF);
  CLK_HSEConfig(CLK_HSE_OFF);
}

void User_GPIO_Init()
{
  GPIO_DeInit(GPIOA);
  
  GPIO_DeInit(GPIOB);
    //LED
    GPIO_Init(nLED_PORT, nLED_1, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(nLED_PORT, nLED_2, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(nLED_PORT, nLED_3, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(nLED_PORT, nLED_4, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(nLED_PORT, nLED_5, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(nLED_PORT, nLED_6, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(nLED_PORT, nLED_7, GPIO_Mode_Out_PP_High_Fast);
    //BLE Disconnect, Factory reset
    GPIO_Init(DISCO_FACT_BLE_PORT, DISCO_FACT_BLE_PIN, GPIO_Mode_Out_PP_Low_Slow);//HIGH level(Rising Edge) 감지 시 client 장치와 연결되어 있다면 연결을 종료, HIGH level(Rising Edge) 4초 이상 유지시 +OK 응답 후 공장 초기화 
  
  GPIO_DeInit(GPIOC);
    //Switch check stick insertion/ejection
    GPIO_Init(SW_STRIP_PORT, SW_STRIP_PIN, GPIO_Mode_In_FL_No_IT);
    //Switch device On/Off
    GPIO_Init(PWR_ONOFF_SW_PORT, PWR_ONOFF_SW_PIN, GPIO_Mode_In_FL_No_IT);
    //BLE_C_STATUS
    GPIO_Init(BLE_C_STATUS_PORT, BLE_C_STATUS_PIN, GPIO_Mode_In_FL_No_IT);
  
  GPIO_DeInit(GPIOD);
    //BLE Sleep & Wake-up
    GPIO_Init(BLE_SLEEP_WAKE_PORT, BLE_SLEEP_WAKE_PIN, GPIO_Mode_Out_PP_High_Slow);//HIGH level이 감지되면 저전력 모드로 진입, LOW level이 감지되면 Wakeup (chipsen BoT-nLE521은 HIGH == Rising Edge, LOW == Falling Edge)
    //BLE AT/Bypass mode
    GPIO_Init(BLE_AT_BYP_PORT, BLE_AT_BYP_PIN, GPIO_Mode_Out_PP_Low_Slow);//Connected 상태에서 HIGH level(Rising Edge) 유지시 AT COMMAND 모드로 동작, LOW level(Falling Edge) 유지시 BYPASS(Default mode) 모드로 동작
    //nEN OPAMP
    GPIO_Init(nEN_OPAMP_PORT, nEN_OPAMP_PIN, GPIO_Mode_Out_PP_High_Fast);
  
  GPIO_DeInit(GPIOE);
    //BLE CTS
    GPIO_Init(BLE_CTS_PORT, BLE_CTS_PIN, GPIO_Mode_In_FL_No_IT);
    //BLE RTS
    GPIO_Init(BLE_RTS_PORT, BLE_RTS_PIN, GPIO_Mode_Out_PP_Low_Slow); // UART On/Off Pin 공유 (PCB v1.2.3 사용 가능)


    //GPIO_WriteBit(BLE_CTS_PORT, BLE_CTS_PIN, RESET);
}

void USER_TIM2_Init() // 1 msec setup
{
  // 8-Bit Timer Interrupt 1 msec period
  CLK->PCKENR1 |= 0x01; // SYSCLK to peripheral enabled TIM2
  TIM2->PSCR = 0x02;    // sysclk / 2^PCSR: sysclk=4MHz -> 1/4(PSCR)-> 1us
  TIM2->ARRH  = 3;//0x03
  TIM2->ARRL  = 231;//0xE7     // 1us * (999+1) = 1ms  
  TIM2->IER  = 0x01;    // Update interrupt enable
  TIM2->CR1  = 0x01;    // Counter enable
}

void USER_TIM3_Init() // 100 msec setup
{
  /*
  // 8-Bit Timer Interrupt 100 msec period
  CLK->PCKENR1 |= 0x02; // SYSCLK to peripheral enabled TIM3
  
  TIM3->PSCR = 0x04;    // sysclk / 2^PCSR: sysclk=4MHz -> 1/16(PSCR)-> 4us
  TIM3->ARRH  = 97;//0x61
  TIM3->ARRL  = 167;//0xA7     // 4us * (24999+1) = 100ms
  */
  /*
  // 8-Bit Timer Interrupt 10 msec period
  CLK->PCKENR1 |= 0x02; // SYSCLK to peripheral enabled TIM3
  
  TIM3->PSCR = 0x04;    // sysclk / 2^PCSR: sysclk=4MHz -> 1/16(PSCR)-> 4us
  TIM3->ARRH  = 9;//0x09
  TIM3->ARRL  = 195;//0xC3     // 4us * (2499+1) = 10ms
  */
  
  // 8-Bit Timer Interrupt 5 msec period
  CLK->PCKENR1 |= 0x02; // SYSCLK to peripheral enabled TIM3
  
  TIM3->PSCR = 0x02;    // sysclk / 2^PCSR: sysclk=4MHz -> 1/4(PSCR)-> 1us
  TIM3->ARRH  = 19;//0x13
  TIM3->ARRL  = 135;//0x87    // 1us * (4999+1) = 5ms

  TIM3->IER  = 0x01;    // Update interrupt enable
  TIM3->CR1  = 0x01;    // Counter enable
}

void USER_TIM4_Init() // 1 sec setup
{
  // 8-Bit Timer Interrupt 1 msec period
  CLK->PCKENR1 |= 0x04; // SYSCLK to peripheral enabled TIM4
  TIM4->PSCR = 0x0F;    // sysclk / 2^PCSR: sysclk=4MHz -> 1/32(PSCR)-> 8us
  TIM4->ARR  = 121;//0x79
  TIM4->IER  = 0x01;    // Update interrupt enable
  TIM4->CR1  = 0x01;    // Counter enable
}

void Device_Instant_Wakeup()
{
  CLK_MainRegulatorCmd(ENABLE);
  PWR_UltraLowPowerCmd(DISABLE);
  User_MCU_Clock_Init(); //Core clock 4 Mhz
  GPIO_Init(PWR_ONOFF_SW_PORT, PWR_ONOFF_SW_PIN, GPIO_Mode_In_FL_No_IT);
  USER_TIM2_Init();
  enableInterrupts();
}

//Device instant sleep() 함수 필요 이유
//ON/OFF switch 입력이 짧게 눌리는 경우
//다시 Halt mode로 빠져야함, Halt mode를 빠지기 전 Device_instant_wakeup()에서 
//init한 부분만 deinit 하기 위해 instant_sleep()함수를 만듦.
void Device_Instant_Sleep()
{
  disableInterrupts();
  //Halt mode 에서 깨어나기 위해 ON/OFF switch interrupt input mode로 변경
  //GPIO_DeInit(GPIOC);
  GPIO_Init(PWR_ONOFF_SW_PORT, PWR_ONOFF_SW_PIN, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(PWR_ONOFF_SW_EXTI_PIN, EXTI_Trigger_Falling);
  //
  TIM2_DeInit();
  TIM2_ClearITPendingBit(TIM2_IT_Update);
  CLK_HSICmd(DISABLE);
  CLK_LSICmd(DISABLE);
  CLK_HaltConfig(CLK_Halt_SlowWakeup, ENABLE);
  PWR_UltraLowPowerCmd(ENABLE); //ULP mode 진입 (MCU reference voltage off)
  halt(); //Halt mode 진입
}

void Device_Wakeup()
{
   CLK_MainRegulatorCmd(ENABLE);
   PWR_UltraLowPowerCmd(DISABLE); 
   User_MCU_Clock_Init(); //Core clock 4 Mhz
   User_GPIO_Init();
   USER_TIM2_Init();
   USER_TIM3_Init();
   USER_TIM4_Init();   
   DAC_Initialization();
   BLE_Init();
   ADC_Initialization();
   LCD_Initialization();
   Interrupt_Priority_Init();
   enableInterrupts();
   
   //Calendar_Setup();//
   
   if(!Battery_Low_Power_Check()) //Battery 전압 정상
   {
    MCU_Unique_Id_Setting(mcu_uid);
    
    if(Memory_Usage_Check()){
      Memory_Bank_Initialization();// 기기 처음 사용 시
    }
    
    LCD_Power_On_Action_1();
    
    BLE_Start();
    
    Device_Variable_Initialization();
    
    if(!Optical_Unit_Check()) // 광학부 정상
    {
      //Stick 분리 되었나?
      if(STRIP_CHECK_SWITCH.event >= STICK_EJECTED){//Yes
        Next_User_Mode_Process(MAIN_APP_CONNECTION_WAITING);
      }
      else{
        Next_User_Mode_Process(MAIN_ERROR_STICK_CHECK); //No
      }
    }
    else //광학부 문제, 강제 Power off
    {
      LCD_Display(BOOK, ICON_ON);
      delay_ms(10000);
      LCD_Event_All_Icon_Blink(3, 500);
      forced_power_down = SET;
    }
   }
   else //Battery Low Voltage, 강제 Power off
   {
      LCD_Display(BATTERY_LOW, ICON_ON);
      delay_ms(5000);
      forced_power_down = SET;
   }
}

void Device_Sleep()
{
  LCD_Power_Off_Action_1();
  LCD_Deinitialization();
  BLE_Deinit();
  BLE_Buffer_Init();//BLE buffer 초기화
  BLE_Disconnect();// BLE 연결 상태라면 연결 해제
  BLE_Sleep();
  disableInterrupts();

  OP_Amp_Disable();
  DAC_Deinitialization();
  ADC_Deinitialization();

  //사용 변수 초기화
  dev_power_status= PWR_SLEEP_STATUS;
  BLE_FLAG.ble_icon_con_en = RESET;
  BLE_FLAG.ble_icon_disc_en = RESET;
  BLE_FLAG.connect_status = BLE_DISCONNECT;
  Timer_Count_Setup(&TIMER_100ms, 0, TIMER_COUNT_DISABLE);
  Timer_Count_Setup(&TIMER_500ms, 0, TIMER_COUNT_DISABLE);
  //
  //Halt mode 에서 깨어나기 위해 ON/OFF switch interrupt input mode로 변경
  GPIO_Init(PWR_ONOFF_SW_PORT, PWR_ONOFF_SW_PIN, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(PWR_ONOFF_SW_EXTI_PIN, EXTI_Trigger_Falling);
  //
  
  TIM2_DeInit();
  TIM3_DeInit();
  TIM4_DeInit();
  //interrupt pending clear
  //EXTI_ClearITPendingBit(SW_STRIP_EXTI_IT_PIN);
  EXTI_ClearITPendingBit(PWR_ONOFF_SW_EXTI_IT_PIN);
  TIM2_ClearITPendingBit(TIM2_IT_Update);
  TIM3_ClearITPendingBit(TIM3_IT_Update);
  TIM4_ClearITPendingBit(TIM4_IT_Update);
  USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
  //
  CLK_HSICmd(DISABLE);
  CLK_LSICmd(DISABLE);
  CLK_HaltConfig(CLK_Halt_SlowWakeup, ENABLE);
  PWR_UltraLowPowerCmd(ENABLE); //ULP mode 진입 (MCU reference voltage off)
  halt(); //Halt mode 진입
}

void Power_Switch_Event_Handle()
{
  switch(POWER_SWITCH.event)
  {
    case PWR_SW_EVENT_SHORT_PRESSED:
      if(dev_power_status == PWR_INSTANT_WAKE_UP_STATUS)
      {
        dev_power_status = PWR_SLEEP_STATUS;
        Device_Instant_Sleep();
      }
      else{}
      POWER_SWITCH.event = PWR_SW_EVENT_NONE;
      break;
    case PWR_SW_EVENT_LONG_PRESSED:
      if(dev_power_status == PWR_WAKE_UP_STATUS)
      {
        dev_power_status = PWR_SLEEP_STATUS;
        Device_Sleep();
      }
      else if(dev_power_status == PWR_INSTANT_WAKE_UP_STATUS) //Power switch를 2초 이상 길게 눌러 device wakeup (종료 조건 2)
      {
        Device_Wakeup();
        dev_power_status = PWR_WAKE_UP_STATUS;
      }
      else{}
      POWER_SWITCH.event = PWR_SW_EVENT_NONE;
      break;
    case PWR_SW_EVENT_VERY_LONG_PRESSED:
      if(dev_power_status == PWR_WAKE_UP_STATUS)
      {
        POWER_SWITCH.timer = 0;
        Factory_Reset_Process();
      }
      else{}
      POWER_SWITCH.event = PWR_SW_EVENT_NONE;
      break;
    default :

      break;
  }
}

void Factory_Reset_Process()
{
  if((dev_power_status == PWR_WAKE_UP_STATUS) && (BLE_FLAG.connect_status == BLE_DISCONNECT))
  {
    if((USER_MODE_PROCESS.function_number == MAIN_APP_CONNECTION_WAITING) || (USER_MODE_PROCESS.function_number == MAIN_STICK_INSERT_WAITING))
    {//Factory reset 진행
      Erase_Memory_Bank(MEMORY_ADDR_1);
      Erase_Memory_Bank(MEMORY_ADDR_2);
      Load_Memory_Bank(MEMORY_ADDR_3, &MEM_BANK_1); //Factory 영역에 저장된 data load      
      Save_Memory_Bank_Block_Program(&MEM_BANK_1);

      EEPROM_Byte_Write(BLE_NAME_SETUP_COMPLETE_ADDR, BLE_SETUP_NONE); //BLE name 설정 확인 초기화
      
      /* Battery, BLE icon update */
      vbat_update = SET;
      vbat_status = VBAT_STATUS_NONE;
      BLE_FLAG.ble_icon_update = SET;
      /* ************************ */
      once_operation = 0;
      LCD_Event_All_Icon_Blink(5, 400); //Factory reset complete
    }
    else{}
  }
  else{}
}

void Check_Power_Supply_Test()
{
  //LED swipe
/*  
  DAC_Initialization();
  OP_Amp_Enable();
  DAC_Start();
  DAC_Data_Set(1000);
*/  
  char i = 0;
  for(i = 1; i <= 7; i++)
  {
    LED_Ctrl((GPIO_Pin_TypeDef)led_num[i], LED_ON);
    delay_ms(50);
    LED_Ctrl((GPIO_Pin_TypeDef)led_num[i], LED_OFF);
  }
/*  
  DAC_Data_Set(0);
  DAC_End();
  OP_Amp_Disable();
  DAC_Deinitialization();
*/
  //LCD_Initialization();
  // Battery 삽입 시 3초간 battey 전압레벨 LCD 표시
  ADC_Initialization();
  
  delay_ms(100);
  vbat_update = SET;
  Vbat_Display();
  delay_ms(1000);
  vbat_update = RESET;
  LCD_All_Off();
  ADC_Deinitialization();
  //LCD_Deinitialization();
  //
}

void Stick_Insertion_Count()
{
  MEM_BANK_1.STICK_INSERT_CNT += 1;
  if(MEM_BANK_1.STICK_INSERT_CNT >= 65000){
    MEM_BANK_1.STICK_INSERT_CNT = 65000;
  }
  EEPROM_Byte_Write(STICK_INSERT_CNT_ADDR_H, (uint8_t)((MEM_BANK_1.STICK_INSERT_CNT >> 8) & 0x00ff));
  EEPROM_Byte_Write(STICK_INSERT_CNT_ADDR_L, (uint8_t)(MEM_BANK_1.STICK_INSERT_CNT & 0x00ff));
}

void GPIO_EXTI_Callback(EXTI_IT_TypeDef EXTI_IT)
{
  if(EXTI_IT == SW_STRIP_EXTI_IT_PIN)
  {
    //EXTI_ClearITPendingBit(SW_STRIP_EXTI_IT_PIN);
  }
  if(EXTI_IT == PWR_ONOFF_SW_EXTI_IT_PIN)
  {
    Device_Instant_Wakeup();
    POWER_SWITCH.event = PWR_SW_EVENT_NONE;
    dev_power_status = PWR_INSTANT_WAKE_UP_STATUS;
  }
}

void Interrupt_Priority_Init()
{
  ITC_SetSoftwarePriority(TIM4_UPD_OVF_TRG_IRQn, ITC_PriorityLevel_3); //TIM4 (1 msec) - Level 3
  ITC_SetSoftwarePriority(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQn, ITC_PriorityLevel_2); //TIM2 (1 msec) - Level 2
  ITC_SetSoftwarePriority(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQn, ITC_PriorityLevel_2); //TIM3 (10 msec) - Level 2

  ITC_SetSoftwarePriority(USART1_RX_TIM5_CC_IRQn, ITC_PriorityLevel_3); //USART1 RX - Level 3
  ITC_SetSoftwarePriority(EXTI6_IRQn, ITC_PriorityLevel_3); //EXTI (Check On/Off switch) - Level 3
}