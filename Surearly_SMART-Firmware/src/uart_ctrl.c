#include "main.h"

void Uart_Tx_Byte(char ch) 
{
  while(!(BLE_USART->SR & 0x80)){}  // Transmit data register empty
  BLE_USART->DR = ch;
}

void Uart_Tx_String(char* str)
{
  while(*str){ 
    Uart_Tx_Byte(*str++);
  }
  //delay_ms(1);
}

uint32_t String_To_Decimal(char *str, uint8_t len) //String -> Decimal 변환
{
  uint32_t dec = 0;
  int i = 0;

  for(i = 0; i < len; i++)
  {
    if((0x30 <= str[i])&&(str[i] <= 0x39)){
      dec += (str[i]-0x30) * (uint32_t)round(pow(10.0, (double)(len-i-1)));
    }
  }

  return dec;
}

void u_strncpy(char *dest_buf, char *buf, uint16_t size)
{
  strncpy(dest_buf, buf, size);
  dest_buf[size - 1] = '\0';
}