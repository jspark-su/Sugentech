/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t BLE_Response_ms; ///xx
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//uint16_t read_data_1[1000] = {0};

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  while(!(BLE_USART->SR & USART_SR_TXE)){}
  BLE_USART->DR =ch;
  return ch;
}


void Timer_Count_Setup(USER_TIMER *TIMER_COUNT, int init_count_value, char enable_status)
{
	TIMER_COUNT->update_block = TIMER_VAR_UPDATE_BUSY;
	if(enable_status == TIMER_COUNT_ENABLE){
		TIMER_COUNT->start = START;
	}
	else if(enable_status == TIMER_COUNT_DISABLE){
		TIMER_COUNT->start = END;
	}
	TIMER_COUNT->count = init_count_value;
	TIMER_COUNT->update_block = TIMER_VAR_UPDATE_COMPLETE;
}

extern void RAM_Vector_Table_Update_For_Main_App(void);

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Main program.

  * @param  None
  * @retval None
  */

void main(void)
{
  /* Infinite loop */
  RAM_Vector_Table_Update_For_Main_App();
  
  Device_Instant_Wakeup();
  BLE_Response_ms = 0;///xx
  User_GPIO_Init();
  LCD_Initialization();
  BLE_Init(); //AT command (AT+INTPULLDOWN=OFF) 적용 위해 BLE_Init();
  Interrupt_Priority_Init();
  delay_ms(100);//BLE Power on settling time
  BLE_Wakeup();
  BLE_Disconnect(); // BLE 연결 상태라면 AT commmand ("AT+INTPULLDOWN=OFF") 적용 위해 연결 해제

  DAC_Initialization(); OP_Amp_Enable();
  DAC_Start(); DAC_Data_Set(1000);

  Check_Power_Supply_Test(); // LED Ch1~7 On->Off, Battery 전압레벨 LCD 표시 (3초간)
  
  BLE_Response_ms = 0;///xx

  uint8_t ble_responce = Send_AT_Command("AT+INTPULLDOWN?\r");
  
  if(ble_responce == AT_CMD_RESP_OFF)
  { // INTPULLDOWN = OFF 상태 정상 (LCD icon: 'BLE', LED-6 (Green) 'On')
    LED_Ctrl(LED_6, LED_ON);
    LCD_Display(BLE, ICON_ON);
    delay_ms(300);
    LED_Ctrl(LED_6, LED_OFF);
    LCD_Display(BLE, ICON_OFF);
  }
  else if(ble_responce == AT_CMD_RESP_ON)
  {
    ///xx
    //USART_Init(BLE_USART, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
    //delay_ms(500);
    //BLE_Response_ms = 0;
    ///xx

    if(Send_AT_Command("AT+INTPULLDOWN=OFF\r") == AT_CMD_RESP_ADVERTISING)
    { // INTPULLDOWN = OFF 설정 정상 (LCD icon: 'BLE', LED-6 (Green) 'On')
      LED_Ctrl(LED_6, LED_ON);
      LCD_Display(BLE, ICON_ON);
      delay_ms(300);
      LED_Ctrl(LED_6, LED_OFF);
      LCD_Display(BLE, ICON_OFF);
    }
    else
    { // INTPULLDOWN = OFF 설정 실패 (LCD icon: 'INSERT' + 'BLE' + 'BOOK', LED-7 (Red) 'On')
      LED_Ctrl(LED_7, LED_ON);
      LCD_Display(BLE, ICON_ON); LCD_Display(BOOK, ICON_ON); LCD_Display(INSERT, ICON_ON);
      delay_ms(5000);
      LED_Ctrl(LED_7, LED_OFF);
      LCD_Display(BLE, ICON_OFF); LCD_Display(BOOK, ICON_OFF); LCD_Display(INSERT, ICON_OFF);
    }
  }
  else
  { // BLE 통신 오류 (LCD icon: 'EJECT' + 'BLE' + 'BOOK', LED-7 (Red) 'On')
    LED_Ctrl(LED_7, LED_ON);
    LCD_Display(BLE, ICON_ON); LCD_Display(BOOK, ICON_ON); LCD_Display(EJECT, ICON_ON);
    delay_ms(5000);
    LED_Ctrl(LED_7, LED_OFF);
    LCD_Display(BLE, ICON_OFF); LCD_Display(BOOK, ICON_OFF); LCD_Display(EJECT, ICON_OFF);
  }
  
  Send_AT_Command("AT+INTPULLDOWN=ON\r"); ///xx

  DAC_Data_Set(0); DAC_End();
  OP_Amp_Disable(); DAC_Deinitialization();
  BLE_Deinit();
  BLE_Sleep();
  LCD_Deinitialization();
  Device_Instant_Sleep();
  while (1)
  {
    Power_Switch_Event_Handle();
    if(forced_power_down) //wake-up 시 광학부 문제, 배터리 저전압시 system 강제 종료
    {
      forced_power_down = 0;
      dev_power_status = PWR_SLEEP_STATUS;
      Device_Sleep();
    }

    if(dev_power_status == PWR_WAKE_UP_STATUS)
    {
      //Vref_Read_Test(read_data_1, 1000);
      //temperature_read_test(read_data, 1000);
      //led_settling_time_test(LED_5, 1800, read_data_1);
      //sec_1_test();
      
      /*       빠른 처리 명령들      */
        Vbat_Display(); //Battery icon display
        BLE_Icon_Display(); //BLE icon display
/*        
        if((memory_save_process == SET)&&(BLE_TX.flag == RESET))
        {
          memory_save_process = RESET;
          MEM_BANK_1.ACCESS_ADDR_1 = SET;
          //Save_Memory_Bank_Byte_Program(MEMORY_ADDR_1, &MEM_BANK_1);
          Save_Memory_Bank_Block_Program(&MEM_BANK_1);
        }
*/        
      /* ************************* */
      
      /*     System main process     */
        switch(device_mode_select)
        {
          case DEV_APP_CONNECTION_MODE:
            User_Mode_Process();
            break;
          case DEV_ADMIN_MODE:
            Admin_Menu_Display();
            Admin_Menu_Process();
            break;
          case DEV_PRODUCTION_MODE:
            Production_Mode_Process();
            break;  
        }

        
      if(fw_update_mode_execution)
      {
        fw_update_mode_execution = 0;
        delay_ms(500); //API "FW_UPD" 응답 전송 완료 대기
        EEPROM_Byte_Write(FW_UPDATE_MODE_EXECUTION_ADDR, FW_UPDATE_MODE_EXECUTION);
        WWDG->CR = 0x80;//Software reset
      }
    }
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
