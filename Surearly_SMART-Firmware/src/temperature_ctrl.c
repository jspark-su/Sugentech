#include "main.h"

float mcu_temperature_read()
{
  uint16_t temperature_adc = 0;
  
  //ADC_ChannelCmd(ADC1, ADC_Channel_TempSensor, ENABLE); //ADC1 TS channel set
  ADC_TempSensorCmd(ENABLE);//ADC TSON
  //ADC_Cmd(ADC1, ENABLE); //ADC ADON Start
  ADC_Start(ADC_Channel_TempSensor);
  //__disable_interrupt();
  uint16_t tmp = ADC_Data();
  char i = 0;
  for(i = 0; i  < 10; i++){
    temperature_adc += ADC_Data();
  }

  //__enable_interrupt();
  ADC_End(ADC_Channel_TempSensor);
  ADC_TempSensorCmd(DISABLE);
  
  /* STM8L152C8 Datasheet: page 42, 112
  The TS_Factory_CONV_V90 byte represents the 8 LSB of the result of the V90 12-bit ADC conversion performed in factory.
  The 2 MSB have a fixed value: 0x3.
  Tested in production at VDD = 3 V ±10 mV. The 8 LSB of the V90 ADC conversion result are 
  stored in the TS_Factory_CONV_V90 byte.
  */
  uint16_t TS_FACTORY_CONV_V90 = (uint16_t)FLASH_ReadByte(0x4911) | 0x0300;
  float avg_slope = ((float)TS_FACTORY_CONV_V90 / 4096.0 * 3.0) / (90.0 + 273.15); //Avg_Slop (V/Kelvin)
  
  float temperature_volt = ((float)temperature_adc * MCU_Read_Voltage()) / 4096 / 10.0; // 10회 평균 적용
  /* STM8L152C8 Reference: page 212 */
  float mcu_temperature = (temperature_volt / (avg_slope)) - 273.15;
  
  return mcu_temperature;
}

/*
void temperature_read_test(uint16_t* data, uint16_t size)
{
  uint16_t i = 0;
  ADC_TempSensorCmd(ENABLE);
  ADC_Start(ADC_Channel_TempSensor);
  
  for(i = 0; i < size; i++)
    data[i] = ADC_Data();
  
  ADC_End(ADC_Channel_TempSensor);
  ADC_TempSensorCmd(DISABLE);
}
*/