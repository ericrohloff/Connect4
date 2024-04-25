#include "stm32l476xx.h"
#include "LED.h"
#include "audio.h"

extern volatile int TimingDelay;

volatile uint32_t msec;
volatile uint8_t nes_controls[8];

// ticks: number of ticks between two interrupts
void SysTick_Init(uint32_t ticks) {
	
	//Turn on MSI clock
	RCC->CR |= RCC_CR_MSIRGSEL;
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= RCC_CR_MSIRANGE_7;
	
	RCC->CR |= RCC_CR_MSION;
	while(!(RCC->CR & RCC_CR_MSIRDY));
	
	// Disable SysTick IRQ and SysTick counter
	SysTick->CTRL = 0;
	
	// Set reload register
	SysTick->LOAD = ticks - 1;
	
	// Set interrupt priority of SysTick
	// Make SysTick least urgent (i.e. highest priority number)
	// __NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
	//NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	NVIC_SetPriority(SysTick_IRQn, 1);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);					// Enable EXTI0_1 interrupt in NVIC

	
	// Reset the SysTick counter value
	SysTick->VAL = 0;
	volatile uint32_t msec = 0;
	
	// Select processor clock
	// 1 = processor clock; 0 = external clock
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	//SysTick->CTRL = 0;
	// Enable SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does no assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// SysTick interrupt service routine
void SysTick_Handler(void) {
	
	if (msec / 2 > 0xFF) msec = 0;
	int nes_count = msec / 2;
	
	// handle latch
	if (nes_count == 0) GPIOB->ODR |= GPIO_ODR_ODR_2;
	else GPIOB->ODR &= ~GPIO_ODR_ODR_2;
	
	
	if (msec % 2 == 1) {
		// TOGGLE CLOCK
		GPIOB->ODR |= GPIO_ODR_ODR_3; // TURN ON CLOCK
	} else {
		GPIOB->ODR &= ~GPIO_ODR_ODR_3; // TURN OFF CLOCK
	}
	
	if (nes_controls[0] > 0) Green_LED_On();
	else Green_LED_Off();
	
	uint8_t current_value = GPIOB->IDR & GPIO_IDR_IDR_6;

	if (nes_count < 0x8) {
		nes_controls[nes_count] = current_value;
	}
	
	msec++;
	
	if(TimingDelay>0)TimingDelay--;
	
	// TimeDelay--;
}
