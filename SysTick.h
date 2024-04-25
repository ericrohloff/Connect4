#ifndef __STM32L476G_DISCOVERY_SYSTICK_H
#define __STM32L476G_DISCOVERY_SYSTICK_H

#include "stm32l476xx.h"


void SysTick_Init(uint32_t ticks);
void SysTick_Handler(void);

#endif /* __STM32L476G_DISCOVERY_SYSTICK_H */