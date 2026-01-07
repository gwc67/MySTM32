#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f1xx.h"  // 根据你的型号修改，如 stm32f4xx.h

void delay_init(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif