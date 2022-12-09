#ifndef __LCD_CTRL_H
 #define __LCD_CTRL_H


/*LCD DUTY LIST
  LCD_Duty_Static < Static duty
  LCD_Duty_1_2    < 1/2 duty
  LCD_Duty_1_3    < 1/3 duty
  LCD_Duty_1_4    < 1/4 duty
  LCD_Duty_1_8    < 1/8 duty
*/
#define LCD_DUTY                LCD_Duty_1_4

/*LCD BIAS LIST
  LCD_Bias_1_4    < 1/4 bias
  LCD_Bias_1_3    < 1/3 bias
  LCD_Bias_1_2    < 1/2 bias
*/    
#define LCD_BIAS                LCD_Bias_1_3

/*LCD PORT MASK LIST
  LCD_PortMaskRegister_0 //SEG0 ~ SEG7
  LCD_PortMaskRegister_1 //SEG8 ~ SEG15
  LCD_PortMaskRegister_2 //SEG16 ~ SEG23
  LCD_PortMaskRegister_3 //SEG24 ~ SEG31
  LCD_PortMaskRegister_4 //SEG32 ~ SEG39
  LCD_PortMaskRegister_5 //SEG40 ~ SEG43
*/
#define LCD_PORT_MASK           LCD_PortMaskRegister_0
#define LCD_PORT_MASK_DATA      0x1F //Select SEG0 ~ 4

/*LCD PAGE SELECTION
  - LCD_PageSelection_FirstPage
      The S0[i] LCD RAM bit corresponds to SEG[i] of COM0
      The S1[i] LCD RAM bit corresponds to SEG[i] of COM1
      The S2[i] LCD RAM bit corresponds to SEG[i] of COM2
      The S3[i] LCD RAM bit corresponds to SEG[i] of COM3
  - LCD_PageSelection_SecondPage
      The S0[i] LCD RAM bit corresponds to SEG[i] of COM4
      The S1[i] LCD RAM bit corresponds to SEG[i] of COM5
      The S2[i] LCD RAM bit corresponds to SEG[i] of COM6
      The S3[i] LCD RAM bit corresponds to SEG[i] of COM7
*/
#define LCD_PAGE_COM            LCD_PageSelection_FirstPage

/*LCD CONTRAST LIST
  LCD_Contrast_Level_0  < Medium Density / High Density Maximum Voltage = 2.60V / 2.60V
  LCD_Contrast_Level_1  < Medium Density / High Density Maximum Voltage = 2.70V / 2.73V
  LCD_Contrast_Level_2  < Medium Density / High Density Maximum Voltage = 2.80V / 2.86V
  LCD_Contrast_Level_3  < Medium Density / High Density Maximum Voltage = 2.90V / 2.99V
  LCD_Contrast_Level_4  < Medium Density / High Density Maximum Voltage = 3.00V / 3.12V
  LCD_Contrast_Level_5  < Medium Density / High Density Maximum Voltage = 3.10V / 3.25V
  LCD_Contrast_Level_6  < Medium Density / High Density Maximum Voltage = 3.20V / 3.38V
  LCD_Contrast_Level_7  < Medium Density / High Density Maximum Voltage = 3.30V / 3.51V
*/    
#define LCD_CONTRAST            LCD_Contrast_Level_4


/*icon*/
/*DATA = Numbering+SEG_bit*/
#define BLE                     0x0101
#define NFC                     0x0280
#define BATTERY_LOW             0x0340
#define BATTERY_MID             0x0410
#define BATTERY_HIGH            0x0520
#define EJECT                   0x0610
#define SQUARE_1                0x0708
#define SQUARE_2                0x0804
#define SQUARE_3                0x0902
#define SQUARE_4                0x0A01
#define PLUS_1                  0x0B80
#define PLUS_2                  0x0C40
#define PLUS_3                  0x0D20
#define MINUS                   0x0E10
#define INSERT                  0x0F04
#define DROP                    0x1002
#define BOOK                    0x1101

/*option*/
#define ICON_OFF                0
#define ICON_ON                 1        
#define BLINK_OFF               2        
#define BLINK_ON                3


#define BLINK_STATE_ON          0
#define BLINK_STATE_OFF         1

#define MINUS_ICON              1
#define PLUS_1_ICON             2
#define PLUS_2_ICON             3
#define PLUS_3_ICON             4
#define ERROR_ICON              9

#define ICON_TOTAL_NUM          16

#define LCD_STATUS_SETUP_COMPLETE   0
#define LCD_STATUS_SETUP_BUSY       1

typedef struct
{
  uint8_t seg;
  uint8_t blink_onoff;
  uint8_t blink_state;
  uint8_t blink_data;
}LCD_COM;


void LCD_Initialization();
void LCD_Deinitialization();
void LCD_Display(uint16_t icon, uint8_t option);
void LCD_Blink_Timer();
void LCD_All_Off();
void LCD_All_On();
void LCD_Power_On_Action_1();
void LCD_Power_Off_Action_1();
void LCD_Event_All_Icon_Blink(uint16_t num, uint32_t interval_time_ms);
void LCD_Result_Icon_Display(char level);

void LCD_Result_Display_Off();
void LCD_Lower_Icon_Off();
void BLE_Icon_Display();

//void LCD_Result_Display(char T_line);
//void lcd_on_off_test();
//void lcd_blink_test();
#endif /* __LCD_CTRL_H*/