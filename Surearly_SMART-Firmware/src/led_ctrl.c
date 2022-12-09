#include "main.h"

double y_target = 2300;
const double x_delta = 10;

const uint8_t led_num[8] = {0, LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7};
const uint16_t ptr_num[7] = {0, PTR_1, PTR_2, PTR_3, PTR_4, PTR_5, PTR_6};

uint16_t calibration_data[8] = {0};
uint8_t calibration_check = CALIBRATION_NONE;

uint16_t initial_band_sensing_data[6]= {0};
uint16_t recent_band_sensing_data[6]= {0};
uint16_t initial_color_sensing_data[2]= {0}; //초기값 설정 필요!!!
uint16_t recent_color_sensing_data[2]= {0};

/*
void LED_Init()
{
  //GPIOB 0~3, 5~7 Output, High level, Pull-Up
  // Output mode
  // High level
  LED_PORT->ODR |= 0xEF;
  // Set Output mode
  LED_PORT->DDR |= 0xEF;
  // Pull-Up or Push-Pull
  LED_PORT->CR1 |= 0xEF;
}
*/

void LED_Ctrl(GPIO_Pin_TypeDef GPIO_Pin, uint8_t state)
{
  if(state == LED_OFF){
    LED_PORT->ODR |= GPIO_Pin;
  }
  else if(state == LED_ON){
    LED_PORT->ODR &= (uint8_t)(~GPIO_Pin);
  }
  else{}
}

void OP_Amp_Enable()
{
  //Enable opamp
  GPIO_WriteBit(nEN_OPAMP_PORT, nEN_OPAMP_PIN, RESET);
  delay_ms(5); //OPAMP VDD settling time
  
  /*
  //ADC 초기값 버리기 => 안정화 위함.
  uint16_t i = 0;
  uint16_t tmp = 0;
  ADC_Start(VBAT_SENSE);
  for(i = 0; i < 200; i++)
    tmp = ADC_Data();
  //
  ADC_End(VBAT_SENSE);
  */
}

void OP_Amp_Disable()
{
  //disable opamp
  GPIO_WriteBit(nEN_OPAMP_PORT, nEN_OPAMP_PIN, SET);
}

void DAC_Initialization()
{
  CLK_PeripheralClockConfig(CLK_Peripheral_DAC, ENABLE);
  DAC_Init(DAC_Channel_1, DAC_Trigger_None, DAC_OutputBuffer_Disable);
  //DAC_Init(DAC_Channel_1, DAC_Trigger_None, DAC_OutputBuffer_Enable);
  
  //disable opamp
  GPIO_WriteBit(nEN_OPAMP_PORT, nEN_OPAMP_PIN, SET);
  //
  
}

void DAC_Deinitialization()
{
  //CLK_PeripheralClockConfig(CLK_Peripheral_DAC, DISABLE);
  DAC_DeInit();
  //Disable opamp
  //GPIO_WriteBit(nEN_OPAMP_PORT, nEN_OPAMP_PIN, SET);
  //
}

void DAC_Start()
{
  /*
  //Enable opamp
  GPIO_WriteBit(nEN_OPAMP_PORT, nEN_OPAMP_PIN, RESET);
  delay_ms(5); //OPAMP VDD settling time
  */
  //
  DAC_Cmd(DAC_Channel_1, ENABLE);
  //DAC_Data_Set(1000);
  delay_ms(1);//DAC init 안정화 시간
}

void DAC_End()
{
  DAC_Data_Set(0); // DAC 완전 종료
  DAC_Cmd(DAC_Channel_1, DISABLE);
  /*
  //disable opamp
  GPIO_WriteBit(nEN_OPAMP_PORT, nEN_OPAMP_PIN, SET);
  //
  */
}

void DAC_Data_Set(uint16_t pDAC_Data)
{
  
  DAC_SetChannel1Data(DAC_Align_12b_R, pDAC_Data);
  
  delay_ms(LED_SETTLING_TIME);      //led settling time, C20 = 18pF 일때 5 ms, C20 = 100nF 일때 20 ms
  
  /* DAC_Align: DAC_Align_12b_L*/
  /*
  *(uint8_t*)((uint16_t)(DAC_BASE + CH1RDHRH_Offset + DAC_Align_12b_L )) = (uint8_t)(((uint16_t)pDAC_Data) >> 8);
  *(uint8_t*)((uint16_t)(DAC_BASE + CH1RDHRH_Offset + 1 + DAC_Align_12b_L )) = (uint8_t)pDAC_Data;
*/
}

double Target_Func(GPIO_Pin_TypeDef c_led_num, double dac)   //대상 함수
{
   
   double y_val = 0; 
   // DAC set -> LED on -> ADC value
   y_val = (double)PTR_Data(c_led_num, (uint16_t)dac);
   
   return y_val;
   
}

double Slope(GPIO_Pin_TypeDef c_led_num, double dac, double y)   //대상 함수의 미분 값
{
  
  double x_tmp = 0;
  double y_tmp = 0;
  double slope = 0;
  
  if(y_target < y)
  {
    x_tmp = dac - x_delta;
  }
  else
  {
    x_tmp = dac + x_delta;
  }
  
  y_tmp = Target_Func(c_led_num, x_tmp);
  slope = (y -y_tmp) / (dac-x_tmp);
  
  
  return slope;
}

uint16_t LED_Calibration(GPIO_Pin_TypeDef c_led_num)
{
  //y0, y_target 10배된 값
   int i = 0;
   int N = 0;
   double x = 0;
   double x0 = 0;
   double tol = 0; //해, 초기값, 오차
   double y0 = 0;
   N = 20; //루프제한 20회
   x0 = 700; //초기 값 (DAC)
   tol = 200; //오차 (PTR data 10배된 값)
   i = 0;   //카운터 초기화
   int j = 0;
   while (i < N) //루프
   {
     // y0 값 검사
     // y0가 5000 <= y0 <= 33000 내 없다면 x0를 +-20% 적용하여 이동
      while(j < N)
      {
        y0 = Target_Func(c_led_num, x0);
        
        if(y0 > 33000)
          x0 = x0 * 0.8;
        else if(y0 < 5000) 
          x0 = x0 * 1.2;
        else
        {
          j = 0;
          break;
        }
        j++;  
      }
      x = x0 - (y0 - y_target) / Slope(c_led_num, x0, y0); //접선의 방정식의 해//////
      //x값 DAC(12-Bit) 범위로 제한
      if(x > 4095){
        x = 4095;
      }
      else if(x <= 250.0){
        x = 250.0;
      }
      //
      //printf("i = %d, x0 = %f, y0 = %f, x = %f\r\n", i, x0, y0,x);
      if (fabs(y0 - y_target) < tol) //정지 조건
        break; // success
      i++;   //카운터 증가
      x0 = x;   //초기값 갱신
   }
   if(i == N)
   { // failure
     calibration_check = CALIBRATION_FAIL;
     return (uint16_t)(x + 0.5);
   }
   else
   { // success
     return (uint16_t)(x + 0.5);
   }
}

void Auto_Calibration_Bandline()
{
  int i = 0;
  double target_ptr_value = 0;
  
  for(i = 1; i <= 5; i++) //LED 채널 별 목표 ptr data 구하기 (ptr max 값의 80%를 목표 값으로 설정)
  {
    //target_ptr_value = (double)PTR_Data((GPIO_Pin_TypeDef)led_num[i], 4095) * 0.8;
    target_ptr_value = (double)32000.0;
    if(target_ptr_value > MAX_TARGET_PTR_VALUE){
      target_ptr_value = MAX_TARGET_PTR_VALUE;
    }
    y_target = target_ptr_value;
    calibration_data[i] = LED_Calibration((GPIO_Pin_TypeDef)led_num[i]);
  }
}

void Auto_Calibration_Colorline()
{
  int i = 0;
  double target_ptr_value = 0;
  
  for(i = 6; i <= 7; i++) //LED 채널 별 목표 ptr data 구하기 (ptr max 값의 80%를 목표 값으로 설정)
  {
    //target_ptr_value = (double)PTR_Data((GPIO_Pin_TypeDef)led_num[i], 4095) * 0.8;
    target_ptr_value = (double)30000.0;
    if(target_ptr_value > MAX_TARGET_PTR_VALUE){
      target_ptr_value = MAX_TARGET_PTR_VALUE;
    }
    y_target = target_ptr_value;
    calibration_data[i] = LED_Calibration((GPIO_Pin_TypeDef)led_num[i]);
  }
}