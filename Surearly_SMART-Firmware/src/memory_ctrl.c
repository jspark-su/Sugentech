#include "main.h"

//MEMORY_BANK MEM_BANK_0; //기기 초기 Data/Parameter 저장 메모리
MEMORY_BANK MEM_BANK_1 = {0,}; // Test 과정에서 업데이트된 Data/Parameter 저장 메모리

//char memory_save_process = 0;

void EEPROM_Start()
{
  // Define flash programming Time
  FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
  // Unlock flash data eeprom memory
  FLASH_Unlock(FLASH_MemType_Data);
  // Wait until Data EEPROM area unlocked flag is set
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET){}
}

void EEPROM_End()
{
  FLASH_Lock(FLASH_MemType_Data);
}

void EEPROM_Byte_Write(uint32_t start_addr, uint8_t data)
{
  __disable_interrupt();
  EEPROM_Start();
  FLASH_ProgramByte(start_addr, data);
  while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET){}
  EEPROM_End();
  __enable_interrupt();
}

uint8_t EEPROM_Byte_Read(uint32_t start_addr)
{
  return FLASH_ReadByte(start_addr);
}

void EEPROM_Erase(uint32_t start_addr, uint32_t size)
{
  uint32_t idx = 0;
  
  for(idx = 0; idx < size; idx++)
  {
    //*(uint8_t*)(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + start_addr + idx) = 0;
    FLASH_EraseByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + start_addr + idx);
  }
  while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET){}
}

void EEPROM_Array_Write(uint32_t start_addr, uint8_t* data, uint32_t size)
{
  uint32_t idx = 0;
  
  for(idx = 0; idx < size; idx++)
  {
    //*(uint8_t*)(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + start_addr + idx) = data[idx];
    FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + start_addr + idx, data[idx]);
  }
  while(FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET){}
}

void EEPROM_Array_Read(uint32_t start_addr, uint8_t* data, uint32_t size)
{
  uint32_t idx = 0;
  
  for(idx = 0; idx < size; idx++)
  {
    //data[idx] = (*(uint8_t*)(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + start_addr + idx));
    data[idx] = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + start_addr + idx);
  }
}

void Save_Memory_Bank_Block_Program(MEMORY_BANK *e_data)
{
  //Block programming: 1-block == 128-byte
  EEPROM_Start();
  __disable_interrupt();
  FLASH_ProgramBlock(0, FLASH_MemType_Data, FLASH_ProgramMode_Standard, (uint8_t *)e_data); //Addr offset: 0 (0 ~ 127 bytes program)
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET){}
/*  
  FLASH_ProgramBlock(1, FLASH_MemType_Data, FLASH_ProgramMode_Standard, (uint8_t *)e_data + 0x80); //Addr offset: 128 (128 ~ 255 bytes program)
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET){}
  FLASH_ProgramBlock(2, FLASH_MemType_Data, FLASH_ProgramMode_Standard, (uint8_t *)e_data + 0x100); //Addr offset: 256 (256 ~ 383 bytes program)
  while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET){}
*/  
  __enable_interrupt();
  EEPROM_End();
}

void Save_Memory_Bank_Byte_Program(uint32_t start_addr, MEMORY_BANK *e_data)
{
  __disable_interrupt();
  EEPROM_Start();
  EEPROM_Array_Write(start_addr, (uint8_t *)e_data, sizeof(MEMORY_BANK));
  EEPROM_End();
  __enable_interrupt();
}

void Load_Memory_Bank(uint32_t start_addr, MEMORY_BANK *e_data)
{
  EEPROM_Array_Read(start_addr, (uint8_t *)e_data, sizeof(MEMORY_BANK));
}

void Erase_Memory_Bank(uint32_t start_addr)
{
  EEPROM_Start();
  EEPROM_Erase(start_addr, sizeof(MEMORY_BANK));
  EEPROM_End();
}

void Input_Init_Band_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data) //Memoery bank에 Band calibration + PTR sensing data 대입
{
  char i = 0;
  
  for(i = 1; i <= 5; i++)
  {
    e_data->CALIBRATION_DATA[i] = calibration_data[i];
    e_data->INITIAL_BAND_SENSING_DATA[i] = initial_band_sensing_data[i];
  }
}

void Input_Init_Color_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data) //Memoery bank에 Color calibration + PTR sensing data 대입
{
  //e_data->first_use = USED;

  e_data->CALIBRATION_DATA[6] = calibration_data[6];
  e_data->CALIBRATION_DATA[7] = calibration_data[7];
  
  e_data->INITIAL_COLOR_SENSING_DATA[0] = initial_color_sensing_data[0];
  e_data->INITIAL_COLOR_SENSING_DATA[1] = initial_color_sensing_data[1];
}

void Load_Init_Band_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data) //Band calibration + PTR sensing data를 Memory bank로부터 로드
{
  char i = 0;
  
  for(i = 1; i <= 5; i++) //Calibration data load (1~5 ch)
  {
    calibration_data[i] = e_data->CALIBRATION_DATA[i];
    initial_band_sensing_data[i] = e_data->INITIAL_BAND_SENSING_DATA[i];
  }
}

void Load_Init_Color_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data) //Color calibration + PTR sensing data를 Memory bank로부터 로드
{
  calibration_data[6] = e_data->CALIBRATION_DATA[6]; //Calibration data load (6, 7 ch)
  calibration_data[7] = e_data->CALIBRATION_DATA[7];
  
  initial_color_sensing_data[0] = e_data->INITIAL_COLOR_SENSING_DATA[0]; //PTR sensing data load (6, 7 ch)
  initial_color_sensing_data[1] = e_data->INITIAL_COLOR_SENSING_DATA[1];
}

void MCU_Unique_Id_Setting(char* uid)
{
  uint32_t idx = 0;
  char data = 0;
  for(idx = 0; idx < 12; idx++)
  {
    data = (char)FLASH_ReadByte(0x4926 + idx);
    sprintf(&uid[2*idx],"%X", (data >> 4));
    sprintf(&uid[(2*idx) + 1],"%X", (data & 0x0f));
  }
}

/*
void eeprom_test()
{
  MEMORY_BANK M_WRITE_DATA_1;
  MEMORY_BANK M_READ_DATA_1;
  
  //M_WRITE_DATA_1.first_use = 1;
  //M_WRITE_DATA_1.recent_results = 2;
  Save_Memory_Bank_Byte_Program(MEMORY_ADDR_2, &M_WRITE_DATA_1);
  
  //Erase_Memory_Bank(MEMORY_ADDR_0);
  
  Load_Memory_Bank(MEMORY_ADDR_2, &M_READ_DATA_1);
  
  delay_ms(1);
}
*/
