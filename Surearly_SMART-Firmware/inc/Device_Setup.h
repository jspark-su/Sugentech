#ifndef __DEVICE_SETUP_H
 #define __DEVICE_SETUP_H

//Strip check switch
#define SW_STRIP_PORT                       GPIOC
#define SW_STRIP_PIN                        GPIO_Pin_5
#define SW_STRIP_EXTI_PIN                   EXTI_Pin_5
#define SW_STRIP_EXTI_IT_PIN                EXTI_IT_Pin5

#define STICK_INSERTED                      0
#define STICK_EJECTED                       1

//BLE onoff switch
#define PWR_ONOFF_SW_PORT                   GPIOC
#define PWR_ONOFF_SW_PIN                    GPIO_Pin_6
#define PWR_ONOFF_SW_EXTI_PIN               EXTI_Pin_6
#define PWR_ONOFF_SW_EXTI_IT_PIN            EXTI_IT_Pin6

//nEN OPAMP
#define nEN_OPAMP_PORT                      GPIOD
#define nEN_OPAMP_PIN                       GPIO_Pin_0

//nLED 1 ~ 7
#define nLED_PORT                           GPIOB
#define nLED_1                              GPIO_Pin_0
#define nLED_2                              GPIO_Pin_1
#define nLED_3                              GPIO_Pin_2
#define nLED_4                              GPIO_Pin_3
#define nLED_5                              GPIO_Pin_5
#define nLED_6                              GPIO_Pin_6
#define nLED_7                              GPIO_Pin_7

#define M_LCD_COM0                          GPIO_Pin_4 //GPIOA
#define M_LCD_COM1                          GPIO_Pin_5 //GPIOA
#define M_LCD_COM2                          GPIO_Pin_6 //GPIOA
#define M_LCD_COM3                          GPIO_Pin_1 //GPIOD

#define M_LCD_SEG0                          GPIO_Pin_7 //GPIOA
#define M_LCD_SEG1                          GPIO_Pin_0 //GPIOE
#define M_LCD_SEG2                          GPIO_Pin_1 //GPIOE
#define M_LCD_SEG3                          GPIO_Pin_2 //GPIOE
#define M_LCD_SEG4                          GPIO_Pin_3 //GPIOE


//
#define PWR_SW_SHORT_PRESS_MIN         1 //Sleep 상태에서 짧게 눌러 instant wake-up을 하기 위함. SHORT_PRESS_MIN 값이 클경우 한번 매우 짧게 버튼을 입력할 경우 instant wake-up 후 sleep으로 다시 진행 할 수 없음. SHORT_PRESS_MIN은 무조건 1로 설정해야함
#define PWR_SW_SHORT_PRESS_MAX         1500
#define PWR_SW_LONG_PRESS_MIN          1500
#define PWR_SW_LONG_PRESS_MAX          2000 //시간이 길게 설정 될 경우 instant wake-up 상태에서 long press event 확인 후 wake-up 진행 완료 후 다시 long press event가 실행 되어 very long press event가 실행 되지 않고 long press event가 실행 되어 sleep으로 진행 되므로 very long press event를 실행 할 수 없음.
                                            //Wake-up 진행 동안 누적 카운트가 MAX를 넘어야 다시 long press event를 실행하지 않으므로 LONG_PRESS_MAX 시간을 짧게 설정 해야함. 
#define PWR_SW_VERY_LONG_PRESS_MIN     15000

#define PWR_SW_EVENT_NONE                0
#define PWR_SW_EVENT_SHORT_PRESSED       1
#define PWR_SW_EVENT_LONG_PRESSED        2
#define PWR_SW_EVENT_VERY_LONG_PRESSED   3


#define PWR_WAKE_UP_STATUS                    1
#define PWR_SLEEP_STATUS                      2
#define PWR_INSTANT_WAKE_UP_STATUS            3
#define PWR_INSTANT_SLEEP_STATUS              4
#define PWR_BATTERY_CHARGING_SLEEP_STATUS     5


typedef struct
{
    int timer; 
    char event;
}SWITCH_CTRL_Typedef;

void User_GPIO_Init();
void Device_Wakeup();
void Device_Sleep();
void Device_Instant_Wakeup();
void Device_Instant_Sleep();
void Check_Power_Supply_Test();
void Stick_Insertion_Count();
void Power_Switch_Event_Handle();
void GPIO_EXTI_Callback(EXTI_IT_TypeDef EXTI_IT);
void Interrupt_Priority_Init();
void Factory_Reset_Process();



#endif /* __DEVICE_SETUP_H*/