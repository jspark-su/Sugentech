#ifndef __READ_PTR_MODE_H
 #define __READ_PTR_MODE_H

typedef enum
{
  READ_PTR_NONE = 0,
  READ_PTR_READ_COMPLETE,
  READ_PTR_DISPLAY_COMPLETE,
  READ_PTR_END
}READ_PTR_MODE_STATUS_Typedef;


typedef struct
{
  int OpticsChannel;
  int vDacInterval;
  int status;
//  int StickState;
  int DacData[10];
  int PtrData[10];
  
}PRODUCT_MODE_READ_PTR_Typedef;

extern void RP_Variable_Init(void);

#endif /* __READ_PTR_MODE_H */