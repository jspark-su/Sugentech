/*****************************************************/
/*사용 순서*/
/*
  1. OP_Amp_Enable()
  2. function select 
    - PTR_Data()
    - PTR_Sensing_All_Channel()
    - PTR_Sensing_Colorline()
    - auto_calibration_all()
  3. OP_Amp_Disable()
*/

#include "main.h"


void ADC_Initialization()
{
  
  //기존 방식
  //Single conversion mode
  CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
  ADC_Init(ADC1, ADC_ConversionMode_Single, ADC_Resolution_12Bit, ADC_Prescaler_2);
  ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_48Cycles); //Ts = 192 usec @ f_sys = 1Mhz (ADC 1번 진행 시)
  
  
  /*
  //Continuous conversion mode
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_192Cycles); //Ts = 192 usec @ f_sys = 1Mhz (ADC 1번 진행 시).
  */
  /*
  //ADC, DMA 미리 START
  //Continuous conversion + DMA mode
    ADC_Cmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
  */
}

void ADC_Deinitialization()
{
  //CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
  ADC_DeInit(ADC1);
}

void ADC_Start(ADC_Channel_TypeDef pChannel)
{
  //기존 방식
  //Single conversion mode
    if((pChannel == ADC_Channel_Vrefint) || (pChannel == ADC_Channel_TempSensor)){
      ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_192Cycles); //Ts = 192 usec @ f_sys = 1Mhz (ADC 1번 진행 시)
    }
    else{
      ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_48Cycles); //Ts = 192 usec @ f_sys = 1Mhz (ADC 1번 진행 시)
    }
    ADC_Cmd(ADC1, ENABLE); //ADC Start
    ADC_ChannelCmd(ADC1, pChannel, ENABLE); //ADC Channel set
  //
  
  
   /*
  //ADC, DMA 매번 START
  //Continuous conversion + DMA mode
    ADC_Cmd(ADC1, ENABLE); //ADC Start
    ADC_ChannelCmd(ADC1, pChannel, ENABLE); //ADC Channel set
    ADC_DMACmd(ADC1, ENABLE); //DMA Start
  */
  
  /*
  //ADC, DMA 미리 START
  //Continuous conversion + DMA mode
  ADC_ChannelCmd(ADC1, pChannel, ENABLE); //ADC Channel set
  */
}

void ADC_End(ADC_Channel_TypeDef pChannel)
{
  //ADC1->CR1 &= (uint8_t)~ADC_CR1_ADON;
  
  
  ADC_Cmd(ADC1, DISABLE);
  ADC_ChannelCmd(ADC1, pChannel, DISABLE);
  
  //Continuous conversion + DMA mode
  //ADC_DMACmd(ADC1, DISABLE);
}

/*
@param  ADC_Channel_TypeDef
  *          This parameter can be one of the following values:
  *            @arg PTRx: 1 ~ 6
*/
uint16_t ADC_Data()
{
  //char i = 0;
  uint16_t ptmpreg = 0;
  
  ADC_SoftwareStartConv(ADC1);
  while (!(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET)){}
  //for(i=0;i<=20;i++); // add little delay
  ptmpreg = ADC_GetConversionValue(ADC1);
  
  ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
  
  return ptmpreg;
}

uint16_t PTR_Data(GPIO_Pin_TypeDef led_num, uint16_t dac_value)
{  
  ADC_Channel_TypeDef ptr_num = PTR_1;
  
  if(led_num == LED_1){
    ptr_num = PTR_1;
  }
  else if(led_num == LED_2){
    ptr_num = PTR_2;
  }
  else if(led_num == LED_3){
    ptr_num = PTR_3;
  }
  else if(led_num == LED_4){
    ptr_num = PTR_4;
  }
  else if(led_num == LED_5){
    ptr_num = PTR_5;
  }
  else if((led_num == LED_6)||(led_num == LED_7)){
    ptr_num = PTR_6;
  }
  
  uint16_t p_data = 0;
  LED_Ctrl(led_num, LED_ON);
  DAC_Start();
  //
  DAC_Data_Set(dac_value);
  
  //__disable_interrupt();
  ADC_Start(ptr_num);
  
  //adc_init_value_erase
  p_data = ADC_Data();
  p_data = 0;
  //
  char i = 0;

  // Oversampling 10
  for(i = 0; i < 10; i++){
    p_data += ADC_Data();
  }
 
  //__enable_interrupt();
  ADC_End(ptr_num);
  LED_Ctrl(led_num, LED_OFF);
  DAC_End();
  return p_data;
}

void PTR_Sensing_All_Channel(uint16_t* ptr_value)
{
  char idx = 0;
  for(idx = 1; idx <= 7; idx++)
  {
    ptr_value[idx] = PTR_Data((GPIO_Pin_TypeDef)led_num[idx], calibration_data[idx]);
  }
}

void PTR_Sensing_Bandline(uint16_t* ptr_value)
{
  char idx = 0;
  for(idx = 1; idx <= 5; idx++)
  {
    ptr_value[idx] = PTR_Data((GPIO_Pin_TypeDef)led_num[idx], calibration_data[idx]);
  }
}

void PTR_Sensing_Colorline(uint16_t* ptr_value)
{
  ptr_value[0] = PTR_Data((GPIO_Pin_TypeDef)led_num[6], calibration_data[6]);
  ptr_value[1] = PTR_Data((GPIO_Pin_TypeDef)led_num[7], calibration_data[7]);
}


void led_settling_time_test(GPIO_Pin_TypeDef led_num, uint16_t dac_value, uint16_t* p_data)
{
/*  
  int idx = 0;
  
  //uint16_t sort_arr[SORT_NUM] = {0};
  //uint8_t i = 0;
  
  OP_Amp_Enable();
  
  ADC_Channel_TypeDef ptr_num;
  if(led_num == LED_1)
    ptr_num = PTR_1;
  else if(led_num == LED_2)
    ptr_num = PTR_2;
  else if(led_num == LED_3)
    ptr_num = PTR_3;
  else if(led_num == LED_4)
    ptr_num = PTR_4;
  else if(led_num == LED_5)
    ptr_num = PTR_5;
  else if((led_num == LED_6)||(led_num == LED_7))
    ptr_num = PTR_6;
  
  //__disable_interrupt();
  
  LED_Ctrl(led_num, LED_ON);
  DAC_Start();
  //
  DAC_Data_Set(dac_value);

  ADC_Start(ptr_num);
  //delay_ms(1000);
  
  uint16_t tmp = ADC_Data();
  
  for(idx = 0; idx < 300; idx++)
    p_data[idx] = ADC_Data();
  
  //__enable_interrupt();
  ADC_End(ptr_num);
  LED_Ctrl(led_num, LED_OFF);
  DAC_End();
  
  OP_Amp_Disable();
*/
}
