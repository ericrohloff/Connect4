#ifndef __AUDIO_H
#define __AUDIO_H

#include "stm32l476xx.h"

struct note{
	int duration;
	double angle;
	int volume;
};

struct song{
	int notes;
	struct note play[70];
};

void SysTick_Init(uint32_t ticks);
void SysTick_Handler(void);



void System_Clock_Init(void);
void DAC_Init(void);
void TIM4_Init(void);
void TIM4_IRQHandler(void);
uint16_t sine(uint16_t angle_in_degrees);
void delay(int ms);
void playsong(struct song song);



#endif