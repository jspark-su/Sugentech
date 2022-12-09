#ifndef __MEMORY_CTRL_H
 #define __MEMORY_H

#define MEMORY_ADDR_1    0 // User data area (측정 후 Data/Parameter 저장 공간)
#define MEMORY_ADDR_2    MEMORY_ADDR_1 + sizeof(MEMORY_BANK)
#define MEMORY_ADDR_3    MEMORY_ADDR_2 + sizeof(MEMORY_BANK) //Backup data area 

#define STICK_INSERT_CNT_ADDR_H             (uint32_t)0x17A0
#define STICK_INSERT_CNT_ADDR_L             (uint32_t)0x17A1
#define BLE_BAUDRATE_SETUP_COMPLETE_ADDR    (uint32_t)0x17A2
#define BLE_NAME_SETUP_COMPLETE_ADDR        (uint32_t)0x17A3
#define FW_UPDATE_MODE_EXECUTION_ADDR       (uint32_t)0x17A4

#define UNUSED          0
#define USED            1

typedef struct
{
  //char first_use; //Device 사용 여부
  char ACCESS_ADDR_1; //ADDR_1 영역 사용 기록 확인 (Main test mode에서 기기 측정 완료 시 SET)

  // 기기 측정 완료 횟수 카운트
  uint16_t DEV_USAGE_CNT;

  uint16_t CALIBRATION_DATA[8]; //Calibration data (DAC value)
  uint16_t INITIAL_BAND_SENSING_DATA[6]; // Unused stick band line PTR sensing data
  uint16_t INITIAL_COLOR_SENSING_DATA[2]; // Unused stick color line PTR sensing data
  uint16_t AGING_BAND_SENSING_DATA[6]; //Calibration 진행 후 PTR sensing data 누적
  
  //Stick 삽입 횟수 count
  uint16_t STICK_INSERT_CNT;
}MEMORY_BANK;
// Memory bank total size: 49-byte

void EEPROM_Start();
void EEPROM_End();

void EEPROM_Byte_Write(uint32_t start_addr, uint8_t data);
uint8_t EEPROM_Byte_Read(uint32_t start_addr);
void EEPROM_Erase(uint32_t start_addr, uint32_t size);

void EEPROM_Array_Write(uint32_t start_addr, uint8_t* data, uint32_t size);
void EEPROM_Array_Read(uint32_t start_addr, uint8_t* data, uint32_t size);

void Save_Memory_Bank_Block_Program(MEMORY_BANK *e_data);//1-block = 128-byte
void Save_Memory_Bank_Byte_Program(uint32_t start_addr, MEMORY_BANK *e_data);
void Load_Memory_Bank(uint32_t start_addr, MEMORY_BANK *e_data);
void Erase_Memory_Bank(uint32_t start_addr);
void Input_Init_Band_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data);
void Input_Init_Color_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data);
void Load_Init_Band_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data);
void Load_Init_Color_Sensing_Data_In_Memory_Bank(MEMORY_BANK *e_data);
void MCU_Unique_Id_Setting(char* uid);
//void eeprom_test();



#endif /* __MEMORY*/