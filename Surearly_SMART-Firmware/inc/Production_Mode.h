#ifndef __PRODUCTION_MODE_H
 #define __PRODUCTION_MODE_H


#define FW_UPD_PASSWORD                 "sugentech_20111209"
#define FW_UPDATE_MODE_EXECUTION        1

enum //Produntion mode function number
{
  PRODUCT_MODE_NONE,
  PRODUCT_MODE_START,
  PRODUCT_CALIB_START,
  PRODUCT_READ_RAWDATA_START,
  PRODUCT_READ_PTR_START,
  PRODUCT_MODE_END,
};

typedef struct
{
  char calib_stick_state;
  char calib_proc_state;
  uint16_t ch_rawdata[8];
  float ch_ratio_data[8];
}PRODUCT_MODE_VAL_TypeDef;

typedef enum
{
  READ_PTR_NONE = 0,
  READ_PTR_READ_COMPLETE,
  READ_PTR_DISPLAY_COMPLETE,
  READ_PTR_END
}READ_PTR_MODE_STATUS_Typedef;

typedef struct
{
  uint8_t OneTimeProcess;
  uint8_t OpticsChannel;
  uint8_t vDacInterval;
  READ_PTR_MODE_STATUS_Typedef status;
  uint16_t DacData[10];
  uint16_t PtrData[10];
  
}PRODUCT_MODE_READ_PTR_Typedef;

typedef struct
{
  char state;
  char process_number;
  
}PRODUCT_MODE_PROCESS_TypeDef;


void Production_Mode_Process();
void Production_Mode_Next_Process(char process_number, char state);
void Production_Calibration_Stick_Check();
#endif /* __PRODUCTION_MODE_H*/