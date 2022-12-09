#ifndef __TIME_CTRL_H
 #define __TIME_CTRL_H

#ifdef __ENABLE_TIME_CTRL

typedef struct TIME_DATA_struct
{
    uint8_t YEAR_UPPER;
    uint8_t YEAR_LOWER;
    uint8_t MONTH;
    uint8_t DATE;
    uint8_t HOURS;
    uint8_t MINUTES;
    uint8_t SECONDS;
    uint8_t NATION[4];
}TIME_DATA_TypeDef;

void Time_Initialization();
void Time_Deinitialization();
void Calendar_Setup(TIME_DATA_TypeDef* time_data);
void Calendar_Read(RTC_DateTypeDef *RTC_Date, RTC_TimeTypeDef *RTC_Time);

#endif /* __ENABLE_TIME_CTRL*/

#endif  /* __TIME_CTRL_H*/