#ifndef __TEMPERATURE_CTRL_H
 #define __TEMPERATURE_CTRL_H

float mcu_temperature_read();
void temperature_read_test(uint16_t* data, uint16_t size);
#endif /* __TEMPERATURE_CTRL_H*/