/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
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
#include "stm8l15x_it.h"
#include "main.h"

extern uint8_t vbat_update;
//extern uint8_t multi_time_test_start;
//extern uint8_t current_time_test_start;

USER_TIMER  TIMER_100ms = {0,};
USER_TIMER  TIMER_500ms = {0,};

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

INTERRUPT_HANDLER(TLI_IRQHandler,0)
{
    
}

/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    //GPIO_EXTI_Callback(SW_STRIP_EXTI_IT_PIN);
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    EXTI_ClearITPendingBit(PWR_ONOFF_SW_EXTI_IT_PIN);
    GPIO_EXTI_Callback(PWR_ONOFF_SW_EXTI_IT_PIN);
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
uint8_t factory_reset_cnt = 0;
const unsigned char nTxByte = 150;
const unsigned char nTxInterval = 100;
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19) // 1 ms timer
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  
    TIM2_ClearITPendingBit(TIM2_IT_Update);
    
    BLE_Response_ms++;///xx
    
    if((BLE_TX.data[BLE_TX.index]) && (BLE_TX.flag == SET) && (ble_uart_block == BLE_UART_BLOCK_DISABLE))
    {
      if(BLE_TX.data_cnt < nTxByte)		// Tx 150-byte
      {
        if((BLE_USART->SR & 0x80))  // Transmit data register empty
        {
          BLE_USART->DR = BLE_TX.data[BLE_TX.index];
          BLE_TX.index++;
          BLE_TX.data_cnt++;
        }
      }
      else if(BLE_TX.data_cnt < (nTxByte + nTxInterval)){	// Tx interval time (250-150) * TIM2
        BLE_TX.data_cnt++;
      }
      else{
        BLE_TX.data_cnt = 0;
      }
    }
    else
    {
      BLE_TX.index = 0;
      BLE_TX.data_cnt = 0;
      BLE_TX.flag = RESET;
    }

    TimingDelay_Decrement();//delay_ms 함수용 시간 처리
    
     //BLE 연결 시 수신 처리
    BLE_Queue_Process();
    
    if(TIMER_100ms.update_block == TIMER_VAR_UPDATE_COMPLETE)
    {
      if(TIMER_100ms.start == START)
      {
        TIMER_100ms.count++;
        if(TIMER_100ms.count >= 100) //period 100 ms
        {
          TIMER_100ms.count = 0;
          if((USER_MODE_PROCESS.flag == END)&&(USER_MODE_PROCESS.function_number > 0)){ // next function 진행 조건
            USER_MODE_PROCESS.flag = START;
          }
        }
      }
    }

    if(TIMER_500ms.update_block == TIMER_VAR_UPDATE_COMPLETE)
    {
      if(TIMER_500ms.start == START)
      {
        TIMER_500ms.count++;
        if(TIMER_500ms.count >= 500) //period 500 ms
        {
          TIMER_500ms.count = 0;
          LCD_Blink_Timer();
        }
      }
    }
    
    //BLE AT 명령 전송 후 BLE 모듈로 부터 수신 응답 대기 시간 카운트
    if(BLE_TX.tact_time_flag){BLE_TX.tact_time++;}
    
    //첫 문자 수신 후 정해진 시간내 모든 문자가 수신 확인 시간 카운트
    if(BLE_RX.tact_time_flag){BLE_RX.tact_time++;}
    //Stick check 삽입/분리 상태 확인
    // Stick 삽입 시: STRIP_CHECK_SWITCH.event == 0
    // Stick 분리 시: STRIP_CHECK_SWITCH.event == 1
    STRIP_CHECK_SWITCH.event = GPIO_ReadInputDataBit(SW_STRIP_PORT, SW_STRIP_PIN);
    //
    
    if(!GPIO_ReadInputDataBit(PWR_ONOFF_SW_PORT, PWR_ONOFF_SW_PIN)){
      POWER_SWITCH.timer++;
    }
    else
    {
      if((PWR_SW_SHORT_PRESS_MIN <= POWER_SWITCH.timer) && (POWER_SWITCH.timer < PWR_SW_SHORT_PRESS_MAX))
      {
        POWER_SWITCH.timer = 0;
        POWER_SWITCH.event = PWR_SW_EVENT_SHORT_PRESSED;
      }
      else{
        POWER_SWITCH.timer = 0;
        //ONOFF_SW.event = PWR_SW_EVENT_NONE;
      }
    }

    if(POWER_SWITCH.event == PWR_SW_EVENT_NONE)
    {
      if((PWR_SW_LONG_PRESS_MIN <= POWER_SWITCH.timer) && (POWER_SWITCH.timer < PWR_SW_LONG_PRESS_MAX)){
        POWER_SWITCH.event = PWR_SW_EVENT_LONG_PRESSED;
      }
      else if(PWR_SW_VERY_LONG_PRESS_MIN <= POWER_SWITCH.timer){
        POWER_SWITCH.event = PWR_SW_EVENT_VERY_LONG_PRESSED;
      }
    }
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */


INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21) //5 ms timer
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  TIM3_ClearITPendingBit(TIM3_IT_Update);

  if(BLE_FLAG.connect_status == BLE_CONNECTED)
  {
    switch(device_mode_select)
    {
      case DEV_APP_CONNECTION_MODE:  
        case DEV_PRODUCTION_MODE: 
          API_Rx_Message_Parsing();
          break;
      case DEV_ADMIN_MODE:
        Admin_BLE_Rx_Process();
        break;
    }       
  }
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */


INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25) //1 sec timer
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    TIM4_ClearITPendingBit(TIM4_IT_Update);

    if(TIMER_COUNT_UP_1.update_block == TIMER_VAR_UPDATE_COMPLETE)
    {
      if(TIMER_COUNT_UP_1.start == START){TIMER_COUNT_UP_1.count++;}
      else{TIMER_COUNT_UP_1.count = 0;}
    }

    if(TIMER_COUNT_UP_2.update_block == TIMER_VAR_UPDATE_COMPLETE)
    {
      if(TIMER_COUNT_UP_2.start == START){TIMER_COUNT_UP_2.count++;}
      else{TIMER_COUNT_UP_2.count = 0;}
    }

    if(TIMER_COUNT_DOWN_1.update_block == TIMER_VAR_UPDATE_COMPLETE)
    {
      if(TIMER_COUNT_DOWN_1.start == START){TIMER_COUNT_DOWN_1.count--;}
      else{TIMER_COUNT_DOWN_1.count = 0;}
    }
    
    if((USER_MODE_PROCESS.function_number == MAIN_SAMPLE_REACT_WAITING) && (TIMER_COUNT_DOWN_1.start == START))
    {
      API_DATA.cur_react_time = TIMER_COUNT_DOWN_1.count;
      if(API_DATA.cur_react_time < 0){API_DATA.cur_react_time = 0;}
      
      if(TIMER_COUNT_DOWN_1.count == sample_react_step[0]){
        API_Key_Value_Setting(&API_DATA, REACT_CHK_KEY, DOING_VAL); //KEY: react_chk, VALUE: doing
        cur_sample_reaction_progress_step = 1;
      }
      else if(TIMER_COUNT_DOWN_1.count == sample_react_step[1]){cur_sample_reaction_progress_step = 2;}
      else if(TIMER_COUNT_DOWN_1.count == sample_react_step[2]){cur_sample_reaction_progress_step = 3;}
      else if(TIMER_COUNT_DOWN_1.count == sample_react_step[3]){cur_sample_reaction_progress_step = 4;}
      else if(TIMER_COUNT_DOWN_1.count < sample_react_step[4]){cur_sample_reaction_progress_step = 5;}

      if(check_sample_flow_process == END){check_sample_flow_process = START;} //1초 간격 sample 전개 상태 확인 (Update strip channel ratio data)
    }
    else{cur_sample_reaction_progress_step = 0;}

    //Bluetooth 연결 확인
    BLE_Connection_Check();
    
    // 1초마다 배터리 상태 업데이트
    if(!vbat_update){vbat_update = 1;}
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */		
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  BLE_Rx_Enqueue();
    
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/