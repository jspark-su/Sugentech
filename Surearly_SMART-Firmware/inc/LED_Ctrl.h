#ifndef __LED_CTRL_H
 #define __LED_CTRL_H

#define LED_SETTLING_TIME       10 // (ms)

#define LED_PORT        GPIOB

#define LED_1           GPIO_Pin_0
#define LED_2           GPIO_Pin_1
#define LED_3           GPIO_Pin_2
#define LED_4           GPIO_Pin_3
#define LED_5           GPIO_Pin_5
#define LED_6           GPIO_Pin_6
#define LED_7           GPIO_Pin_7

#define LED_OFF         0
#define LED_ON          1

#define TARGET_PTR_RATIO         0.8
#define MAX_TARGET_PTR_VALUE     (double)36000

#define CALIB_STICK_EJCETED     0
#define CALIB_STICK_INSERTED    1

#define CALIBRATION_NONE        0
#define CALIBRATION_FAIL        1

//void LED_Init();
void LED_Ctrl(GPIO_Pin_TypeDef GPIO_Pin, uint8_t state);

void OP_Amp_Enable();
void OP_Amp_Disable();
void DAC_Initialization();
void DAC_Deinitialization();
void DAC_Start();
void DAC_End();
void DAC_Data_Set(uint16_t pDAC_Data);
uint16_t LED_Calibration(GPIO_Pin_TypeDef c_led_num);
double Target_Func(GPIO_Pin_TypeDef c_led_num, double dac);
double Slope(GPIO_Pin_TypeDef c_led_num, double dac, double y);
void Auto_Calibration_Bandline();
void Auto_Calibration_Colorline();
#endif /* __LED_CTRL_H*/