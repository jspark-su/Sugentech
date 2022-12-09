#include "main.h"

__IO uint32_t Timing_Delay = 0;

void delay_ms(__IO uint32_t nTime)
{
  Timing_Delay = nTime;
  while (Timing_Delay != 0){}
}

void TimingDelay_Decrement(void)
{
  if (Timing_Delay != 0x00)
  {
    Timing_Delay--;
  }
}