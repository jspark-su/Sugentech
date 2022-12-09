#ifndef __BATTERY_CTRL_H
 #define __BATTERY_CTRL_H

#define VREF                        (float)1.224

#define VBAT_SENSE                  (ADC_Channel_9)

#define VBAT_LOW_VOLT               (float)2.60 // VLDO + 0.08
#define VBAT_MID_VOLT               (float)2.70 // VBAT_LOW_VOLT + 0.12
#define VBAT_HIGH_VOLT              (float)2.80 // VBAT_MID_VOLT + 0.08

#define VBAT_STATUS_HIGH               4
#define VBAT_STATUS_MID                3
#define VBAT_STATUS_LOW                2
#define VBAT_STATUS_VERY_LOW           1
#define VBAT_STATUS_NONE               0

uint16_t Vbat_Read_ADC();
uint16_t Vref_Read_ADC();
float Vbat_Read_Voltage();
float MCU_Read_Voltage();
void Vbat_Display();

#endif /* __BATTERY_CTRL_H*/