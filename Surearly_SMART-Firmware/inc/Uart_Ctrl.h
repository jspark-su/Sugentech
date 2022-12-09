#ifndef __UART_CTRL_H
 #define __UART_CTRL_H

#define REMOVE_CR_LF            2
#define STR_BUFFER_SIZE         100

void Uart_Tx_Byte(char ch);
void Uart_Tx_String(char* str);
void string_print(char* str);
//uint8_t hex2ascii(uint8_t code);
//uint8_t ascii_num_check(char *ascii);
uint32_t String_To_Decimal(char *str, uint8_t len); //String -> Decimal 변환
void u_strncpy(char *dest_buf, char *buf, uint16_t size);
#endif /* __UART_CTRL_H*/