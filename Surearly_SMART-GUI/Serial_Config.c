#include "main.h"  
 
SERIAL_CONFIG_TypeDef SERIAL_CONFIG;

void Serial_Port_Find(int panel)
{
	char string[512];
	unsigned int size1,size2,values,i;
	int type;
	char valueName[1024];
	char serial_port_name[100]={0};
	
    RegQueryInfoOnKey (REGKEY_HKLM, "HARDWARE\\DEVICEMAP\\SERIALCOMM", NULL, &values, NULL, NULL,NULL);
	
	for(i=0;i<values;i++) 
    {
        size1 = 1024; 
        size2 = 512;
    
        RegEnumerateValue (REGKEY_HKLM, "HARDWARE\\DEVICEMAP\\SERIALCOMM", i, valueName, &size1, string, &size2,&type);
		
        if( type==_REG_SZ )
        {
            if((valueName[8] == 'V')&&(valueName[9] == 'C')&&(valueName[10] == 'P'))
			{
                sprintf(serial_port_name, " > 사용 가능한 포트 번호 : %s\n",string);
				SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET,serial_port_name);
                    
				sscanf(string, "COM%d", &SERIAL_CONFIG.port_number);
				SetCtrlIndex(panel, SUB_PANEL_sSERIAL_PORT_SEL, SERIAL_CONFIG.port_number-1);
			}
        }
    }
	
}

void Serial_Port_Open(int panel)
{
	int sParity = 0;
	int sStopbits = 1;
	int sDatasize = 8;
	
    GetCtrlVal (panel, SUB_PANEL_sSERIAL_PORT_SEL, &SERIAL_CONFIG.port_number);
    GetCtrlVal (panel, SUB_PANEL_sSERIAL_BAUDRATE_SEL, &SERIAL_CONFIG.baud_rate);

    if(!OpenComConfig (SERIAL_CONFIG.port_number, "",SERIAL_CONFIG.baud_rate, sParity, sDatasize, sStopbits, 5000, 5000) )
	{
        SERIAL_CONFIG.serial_connection_check = SERIAL_PORT_OPENED;
        SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n > 시리얼 포트 열기 완료 !\r\n\r\n");
        SetCtrlVal(panel, SUB_PANEL_sBT_SERIAL_PORT_OPEN, 0);
	}
	else
	{
		SERIAL_CONFIG.serial_connection_check = SERIAL_PORT_CLOSED;
    	SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n > 시리얼 포트 열기 실패.\r\n\r\n");
    	SetCtrlVal(panel, SUB_PANEL_sBT_SERIAL_PORT_OPEN, 1);
	}
}

void Serial_Port_Close(int panel)
{
    GetCtrlVal(panel, SUB_PANEL_sSERIAL_PORT_SEL, &SERIAL_CONFIG.port_number);
    SetCtrlVal(panel, SUB_PANEL_MONITOR_CON_SET, "\r\n > 시리얼 포트 닫힘.\r\n\r\n");
	CloseCom(SERIAL_CONFIG.port_number);
	SERIAL_CONFIG.serial_connection_check = SERIAL_PORT_CLOSED;
}

char Serial_Read_Byte()
{
	if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED){
		return (char)ComRdByte(SERIAL_CONFIG.port_number);
	}
	else{
		return 0xff;	
	}
}

void Serial_Write_Byte(char ch)
{
	if(SERIAL_CONFIG.serial_connection_check == SERIAL_PORT_OPENED){
		ComWrtByte(SERIAL_CONFIG.port_number, (int)ch);
	}
}

void Serial_Write_String(char* str)
{
  ComWrt(SERIAL_CONFIG.port_number, str, strlen(str));  
}
