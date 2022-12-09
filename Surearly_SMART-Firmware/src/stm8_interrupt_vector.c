#include "main.h" 

typedef void (INTERRUPT *interrupt_handler_t)(void);
struct interrupt_vector {
    unsigned char interrupt_instruction;
    //unsigned char reserve;
    interrupt_handler_t interrupt_handler;
};
 
struct interrupt_vector isr_handler[32]@".memvectab";

const struct interrupt_vector isr_handler_init[32] = {
    {0x82, 0x00},
    {0x82, TRAP_IRQHandler},
    {0x82, TLI_IRQHandler},
    {0x82, FLASH_IRQHandler},
    {0x82, DMA1_CHANNEL0_1_IRQHandler},
    {0x82, DMA1_CHANNEL2_3_IRQHandler},
    {0x82, RTC_CSSLSE_IRQHandler},
    {0x82, EXTIE_F_PVD_IRQHandler},
    {0x82, EXTIB_G_IRQHandler},
    {0x82, EXTID_H_IRQHandler},
    {0x82, EXTI0_IRQHandler},
    {0x82, EXTI1_IRQHandler},
    {0x82, EXTI2_IRQHandler},
    {0x82, EXTI3_IRQHandler},
    {0x82, EXTI4_IRQHandler},
    {0x82, EXTI5_IRQHandler},
    {0x82, EXTI6_IRQHandler},
    {0x82, EXTI7_IRQHandler},
    {0x82, LCD_AES_IRQHandler},
    {0x82, SWITCH_CSS_BREAK_DAC_IRQHandler},
    {0x82, ADC1_COMP_IRQHandler},
    {0x82, TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler},
    {0x82, TIM2_CC_USART2_RX_IRQHandler},
    {0x82, TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler},
    {0x82, TIM3_CC_USART3_RX_IRQHandler},
    {0x82, TIM1_UPD_OVF_TRG_COM_IRQHandler},
    {0x82, TIM1_CC_IRQHandler},
    {0x82, TIM4_UPD_OVF_TRG_IRQHandler},
    {0x82, SPI1_IRQHandler},
    {0x82, USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler},
    {0x82, USART1_RX_TIM5_CC_IRQHandler},
    {0x82, I2C1_SPI2_IRQHandler},
};

void RAM_Vector_Table_Update_For_Main_App(void)
{
    disableInterrupts();
    uint8_t i = 0;
    for(i = 0; i < 32; i++)
    {
        isr_handler[i].interrupt_instruction = isr_handler_init[i].interrupt_instruction;
        isr_handler[i].interrupt_handler = isr_handler_init[i].interrupt_handler;
    }
}
