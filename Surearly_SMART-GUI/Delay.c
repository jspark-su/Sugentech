#include "main.h"

int Timing_Delay_ms = 0;

void delay_ms(int nTime)
{
  Timing_Delay_ms = nTime;
  //while (Timing_Delay_ms != 0){}
  for(;;)
  {
	if(Timing_Delay_ms == 0){
		break;
	}
  }
}

void TimingDelay_ms_Decrement(void)
{
  if (Timing_Delay_ms != 0)
  {
    Timing_Delay_ms--;
  }
}
