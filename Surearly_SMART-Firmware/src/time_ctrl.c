#include "main.h"

#ifdef __ENABLE_TIME_CTRL


RTC_InitTypeDef   RTC_Init_Set;
RTC_TimeTypeDef   RTC_Time_Set;
RTC_DateTypeDef   RTC_Date_Set;
//RTC_AlarmTypeDef  RTC_Alarm_Set;

RTC_TimeTypeDef   RTC_Time_Read;
RTC_DateTypeDef   RTC_Date_Read;

TIME_DATA_TypeDef   TIME_DATA;

void Time_Initialization()
{
  //CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);
  //CLK_RTCClockConfig(CLK_RTCCLKSource_HSI, CLK_RTCCLKDiv_64);
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);
  RTC_Init_Set.RTC_HourFormat = RTC_HourFormat_24;
  //Reference manual 426 page
  // 1 Hz = LSI_clock / (RTC_AsynchPrediv + 1)*(RTC_SynchPrediv + 2)
  //LSI
  /*
  //38 kHz
  RTC_Init_Set.RTC_AsynchPrediv = 0x7C; //(124)
  RTC_Init_Set.RTC_SynchPrediv = 0x012F; //(303)
  */
  /*
  //37 kHz
  RTC_Init_Set.RTC_AsynchPrediv = 0x7C; //(124)
  RTC_Init_Set.RTC_SynchPrediv = 0x0127; //(295)
  */
  //36.545 kHz
  RTC_Init_Set.RTC_AsynchPrediv = 0x03; //(3)
  RTC_Init_Set.RTC_SynchPrediv = 0x2317; //(8983)
  
  /* //HSI
  RTC_Init_Set.RTC_AsynchPrediv = 0x7C; //124 (7C) + 1
  RTC_Init_Set.RTC_SynchPrediv = 0x07CF;//1999 (7CF) +1
  */
  RTC_Init(&RTC_Init_Set);
}

void Time_Deinitialization()
{
  RTC_DeInit();
  CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE);
  CLK_RTCClockConfig(CLK_RTCCLKSource_Off, CLK_RTCCLKDiv_1);
  CLK_LSICmd(DISABLE);
}

void Calendar_Setup(TIME_DATA_TypeDef* time_data)
{
  uint8_t month = 0;

  if(time_data->MONTH == 0x0A){
    month = 0x10;
  }
  else if(time_data->MONTH == 0x0B){
    month = 0x11;
  }
  else if(time_data->MONTH == 0x0C){
    month = 0x12; 
  }
  else{
    month = time_data->MONTH;
  }
    

  RTC_DateStructInit(&RTC_Date_Set);
  RTC_Date_Set.RTC_WeekDay = RTC_Weekday_Wednesday;
  RTC_Date_Set.RTC_Date = time_data->DATE;
  RTC_Date_Set.RTC_Month = (RTC_Month_TypeDef)month;
  RTC_Date_Set.RTC_Year = time_data->YEAR_LOWER;
  RTC_SetDate(RTC_Format_BIN, &RTC_Date_Set);

  RTC_TimeStructInit(&RTC_Time_Set);
  RTC_Time_Set.RTC_Hours   = time_data->HOURS;
  RTC_Time_Set.RTC_Minutes = time_data->MINUTES;
  RTC_Time_Set.RTC_Seconds = time_data->SECONDS;
  RTC_SetTime(RTC_Format_BIN, &RTC_Time_Set);

  /*
  RTC_AlarmStructInit(&RTC_Alarm_Set);
  RTC_Alarm_Set.RTC_AlarmTime.RTC_Hours   = 01;
  RTC_Alarm_Set.RTC_AlarmTime.RTC_Minutes = 01;
  RTC_Alarm_Set.RTC_AlarmTime.RTC_Seconds = 00;
  RTC_Alarm_Set.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
  RTC_SetAlarm(RTC_Format_BIN, &RTC_Alarm_Set);

  RTC_ITConfig(RTC_IT_ALRA, ENABLE);
  RTC_AlarmCmd(ENABLE);
  */
}

void calendar_set()
{
  RTC_DateStructInit(&RTC_Date_Set);
  RTC_Date_Set.RTC_WeekDay = RTC_Weekday_Wednesday;
  RTC_Date_Set.RTC_Date = 25;
  RTC_Date_Set.RTC_Month = RTC_Month_September;
  RTC_Date_Set.RTC_Year = 19;
  RTC_SetDate(RTC_Format_BIN, &RTC_Date_Set);

  RTC_TimeStructInit(&RTC_Time_Set);
  RTC_Time_Set.RTC_Hours   = 12;
  RTC_Time_Set.RTC_Minutes = 15;
  RTC_Time_Set.RTC_Seconds = 00;
  RTC_SetTime(RTC_Format_BIN, &RTC_Time_Set);
}


void Calendar_Read(RTC_DateTypeDef *RTC_Date, RTC_TimeTypeDef *RTC_Time)
{
  RTC_GetTime(RTC_Format_BIN, RTC_Time);
  RTC_GetDate(RTC_Format_BIN, RTC_Date);
}


#endif