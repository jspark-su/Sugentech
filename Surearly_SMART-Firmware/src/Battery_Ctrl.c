#include "main.h"

uint8_t vbat_update = 0;
uint8_t vbat_status = 0;

uint16_t Vbat_Read_ADC()
{
  uint16_t vbat_adc = 0;
  //__disable_interrupt();
  ADC_Start(VBAT_SENSE);
  uint16_t tmp = ADC_Data();

  char i = 0;
  for(i = 0; i < 10; i++){
    vbat_adc += ADC_Data();
  }
  //__enable_interrupt();
  ADC_End(VBAT_SENSE);
  return vbat_adc;
}

uint16_t Vref_Read_ADC()
{
  uint16_t vref_adc = 0;
  ADC_VrefintCmd(ENABLE);
  delay_ms(5); //Vref (ADC) turn on settling time
  //__disable_interrupt();
  ADC_Start(ADC_Channel_Vrefint);
  uint16_t tmp = ADC_Data();

  char i = 0;
  for(i = 0; i < 10; i++){
    vref_adc += ADC_Data();
  }
  //__enable_interrupt();
  ADC_End(ADC_Channel_Vrefint);
  ADC_VrefintCmd(DISABLE);
  return vref_adc;
}

float Vbat_Read_Voltage()
{
  float vbat_volt = 0;

  vbat_volt = ((VREF * (float)Vbat_Read_ADC()) / (float)Vref_Read_ADC()) * 2.0; //전압 센싱 게인 = 0.5(R1: 100K, R2: 100K) => 2.0
  
  return vbat_volt;
}

float MCU_Read_Voltage()
{
  float vdd = 0;
  vdd = ((VREF * 4096.0) / (float)Vref_Read_ADC()) * 10.0; // 10: vref_adc 10회 평균 적용
  return vdd;
}

void Vbat_Display()
{
  if(vbat_update) //Battery display
  {
    vbat_update = 0;  
    float vbat = Vbat_Read_Voltage();
    
    if(vbat < VBAT_LOW_VOLT) //VERY_LOW
    {
      if(vbat_status != VBAT_STATUS_VERY_LOW)
      {
        vbat_status = VBAT_STATUS_VERY_LOW;
        LCD_Display(BATTERY_HIGH, ICON_OFF);
        LCD_Display(BATTERY_MID, ICON_OFF);
        LCD_Display(BATTERY_LOW, ICON_OFF);
        LCD_Display(BATTERY_LOW, BLINK_ON);  
      }
    }
    else if((VBAT_LOW_VOLT <= vbat)&&(vbat < VBAT_MID_VOLT)) //LOW
    {
      if(vbat_status != VBAT_STATUS_LOW)
      {
        vbat_status = VBAT_STATUS_LOW;
        LCD_Display(BATTERY_HIGH, ICON_OFF);
        LCD_Display(BATTERY_MID, ICON_OFF);
        LCD_Display(BATTERY_LOW, BLINK_OFF);
        LCD_Display(BATTERY_LOW, ICON_ON);
      }
    }
    else if((VBAT_MID_VOLT <= vbat)&&(vbat < VBAT_HIGH_VOLT)) //MID
    {
      if(vbat_status != VBAT_STATUS_MID)
      {
        vbat_status = VBAT_STATUS_MID;
        LCD_Display(BATTERY_HIGH, ICON_OFF);
        LCD_Display(BATTERY_MID, ICON_ON);
        LCD_Display(BATTERY_LOW, BLINK_OFF);
        LCD_Display(BATTERY_LOW, ICON_ON);
      }
    }
    else if(vbat >= VBAT_HIGH_VOLT) //HIGH
    {
      if(vbat_status != VBAT_STATUS_HIGH)
      {
        vbat_status = VBAT_STATUS_HIGH;
        LCD_Display(BATTERY_HIGH, ICON_ON);
        LCD_Display(BATTERY_MID, ICON_ON);
        LCD_Display(BATTERY_LOW, BLINK_OFF);
        LCD_Display(BATTERY_LOW, ICON_ON);
      }
    }
    else{}
  }
}
