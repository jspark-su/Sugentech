#ifndef __PTR_CTRL_H
 #define __PTR_CTRL_H

#define OVERSAMPLING_NUM         (uint16_t) 10

#define SORT_NUM                 70 //정렬 할 data sample 개수 => PTR sensing 안정화 위해 오름차순 정렬 후 중간값 취하기,, 

#define PTR_1                    (ADC_Channel_8)
#define PTR_2                    (ADC_Channel_7)
#define PTR_3                    (ADC_Channel_6)
#define PTR_4                    (ADC_Channel_5)
#define PTR_5                    (ADC_Channel_4)
#define PTR_6                    (ADC_Channel_3)


void ADC_Initialization();
void ADC_Deinitialization();
void ADC_Start(ADC_Channel_TypeDef pChannel);
void ADC_End(ADC_Channel_TypeDef pChannel);
uint16_t ADC_Data();
uint16_t PTR_Data(GPIO_Pin_TypeDef led_num, uint16_t dac_value);
void PTR_Sensing_All_Channel(uint16_t* ptr_value);
void PTR_Sensing_Bandline(uint16_t* ptr_value);
void PTR_Sensing_Colorline(uint16_t* ptr_value);
void led_settling_time_test(GPIO_Pin_TypeDef led_num, uint16_t dac_value, uint16_t* p_data);
#endif /* __PTR_CTRL_H*/