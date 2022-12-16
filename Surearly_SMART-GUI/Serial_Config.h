#ifndef __SERIAL_CONFIG_H
 #define __SERIAL_CONFIG_H

#define SERIAL_BAUDRATE		(int)38400
#define SERIAL_PORT_CLOSED	0
#define SERIAL_PORT_OPENED	1
typedef struct
{
	char serial_connection_check;
	int port_number;
    int baud_rate;
}SERIAL_CONFIG_TypeDef;

void Serial_Port_Find(int panel);
void Serial_Port_Open(int panel);
void Serial_Port_Close(int panel);

char Serial_Read_Byte();
void Serial_Write_Byte(char ch);
void Serial_Write_String(char* str);


#endif /* __SERIAL_CONFIG_H*/


