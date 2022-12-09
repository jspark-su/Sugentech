#include "main.h"


char Battery_Low_Power_Check()
{
    float vbat = Vbat_Read_Voltage();
    //Battery 전압이 MCU 구동전압 보다 높은지 확인,

    if(vbat > VBAT_LOW_VOLT){
        return 0;
    }
    else{
        return 1;
    }
}

char Memory_Usage_Check()
{
    memset(&MEM_BANK_1, 0, sizeof(MEM_BANK_1)); //이전 사용 data 초기화
    Load_Memory_Bank(MEMORY_ADDR_1, &MEM_BANK_1); //이전 사용 data load
    if(MEM_BANK_1.ACCESS_ADDR_1 == RESET){// 기기를 한번도 사용한 적 없다면 (New PCB 사용 시)
        return 1;
    }
    else{
        return 0;
    }
}

void Memory_Bank_Initialization()
{
  memset(&MEM_BANK_1, 0, sizeof(MEM_BANK_1));
  
/* EEPROM Reset */  
  Erase_Memory_Bank(MEMORY_ADDR_1);
  
  //Stick 삽입 횟수 0 초기화
  EEPROM_Byte_Write(STICK_INSERT_CNT_ADDR_H, 0);
  EEPROM_Byte_Write(STICK_INSERT_CNT_ADDR_L, 0);
  //
/* *********** */  
  
  MEM_BANK_1.ACCESS_ADDR_1 = SET;

  //memory_save_process = SET;//Factory data로 MEM_BANK_1 영역 업데이트
  Save_Memory_Bank_Block_Program(&MEM_BANK_1);
}

void Device_Variable_Initialization()
{
    //Stick Band + Color DAC, PTR data 전역 변수로 load
    Load_Init_Band_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); // MEM_BANK의 band calibration data + PTR sensing data 전역 변수로 load
    Load_Init_Color_Sensing_Data_In_Memory_Bank(&MEM_BANK_1); // MEM_BANK의 color calibration data + PTR sensing data 전역 변수로 load
    //
    //Stick 삽입 횟수 load
    MEM_BANK_1.STICK_INSERT_CNT = (((uint16_t)EEPROM_Byte_Read(STICK_INSERT_CNT_ADDR_H) << 8) & 0xff00) | ((uint16_t)EEPROM_Byte_Read(STICK_INSERT_CNT_ADDR_L) & 0x00ff);
    //    
    User_Mode_Variable_Init();
    Admin_Variable_Init();
    API_Key_Value_Init(&API_DATA);
    
    Timer_Count_Setup(&TIMER_100ms, 0, TIMER_COUNT_ENABLE);
    Timer_Count_Setup(&TIMER_500ms, 0, TIMER_COUNT_ENABLE);

    //기기 초기 실행 시 BLE, Battery Icon을 먼저 표시 하기 위해 update flag SET
    BLE_FLAG.ble_icon_update = SET;
    vbat_update = SET;
    vbat_status = VBAT_STATUS_NONE;
    //
    
    device_mode_select = DEV_APP_CONNECTION_MODE;
}

char Optical_Unit_Check()
{
    //Check optical unit
    char i = 0;
    char check_optical_unit = 0;
    uint16_t c_ptr_data[8] = {0};
    
    PTR_Sensing_All_Channel(c_ptr_data);
    for(i = 1; i <= 7; i++)
    {
      if(c_ptr_data[i] < 10000){} //Normal
      else //Error
      {
        check_optical_unit = 1;
      }     
    }
    return check_optical_unit;
}