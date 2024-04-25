#include "audio.h"

void System_Clock_Init(void);
void DAC_Init(void);
void TIM4_Init(void);
void TIM4_IRQHandler(void);
uint16_t sine(uint16_t angle_in_degrees);
void delay(int ms);
void playsong(struct song song);

volatile int TimingDelay;		//Global variable for delay

double v=0;					//Global variables for DAC
double angle=0;
int volume=5;

//void System_Clock_Init(void){
//	
//	// Enable High Speed Internal Clock (HSI = 16 MHz)
//  RCC->CR |= ((uint32_t)RCC_CR_HSION);
//	
//  // wait until HSI is ready
//  while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );
//	
//  // Select HSI as system clock source 
//  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
//  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;  //01: HSI16 oscillator used as system clock

//  // Wait till HSI is used as system clock source 
//  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );
//	
//}

void DAC_Init(void){
	//Enable clock to GPIO Port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//Set PA5 as analog
	GPIOA->MODER |= GPIO_MODER_MODER5;
	
	//Configure TIM4 as Master Trigger
	TIM4_Init();
	
	//Enable DAC clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	
	//Enable DAC output buffer
	DAC->MCR &= ~DAC_MCR_MODE2;
	//DAC->MCR |= DAC_MCR_MODE2_0;
	
	//Select TIM4 TRGO as trigger for output
	DAC->CR &= ~DAC_CR_TSEL2;
	DAC->CR |= DAC_CR_TSEL2_2;
	DAC->CR |= DAC_CR_TSEL2_0;
	
	DAC->CR |= DAC_CR_TEN2;
	
	//Enable DAC2
	DAC->CR |= DAC_CR_EN2;
	
}

void TIM4_Init(void){
	//Enable TIM4 Clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	//Edge-aligned mode
	TIM4->CR1 &= ~TIM_CR1_CMS;
	
	//Counting direction: Up counting
	TIM4->CR1 &= ~TIM_CR1_DIR;
	
	//Master mode selection
	TIM4->CR2 &= ~TIM_CR2_MMS;	// Master mode selection
	TIM4->CR2 |= TIM_CR2_MMS_2;	// 100 = OC1REF as TRG0
	
	//Trigger interrupt enable
	TIM4->DIER |= TIM_DIER_TIE;
	
	//Update interrupt enable
	TIM4->DIER |= TIM_DIER_UIE;
	
	//OC1M: Output Compare 1 mode
	TIM4->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;		// 0110 = PWM mode 1
	
	//The counter clock frequency (CK_CNT) = fCK_PSC / (PSC[15:0] +1)
	//Timer driving frequency = 16 MHz/(1 + PSC) = 16 MHz/(1+9) = 1.6 MHz
	TIM4->PSC = 9;		//max 65535
	
	//Trigger frequency = 16 MHz/ (1 + ARR) = 16 MHz/100 = 16 KHz
	TIM4->ARR = 99;		// max 65535
	
	TIM4->CCR1 = 50;		// Duty ratio 50%
	
	TIM4->CCER |= TIM_CCER_CC1E;	//OC1 signal is output on the corresponding output pin
	
	//Enable timer
	TIM4->CR1 |= TIM_CR1_CEN;
	
	//Set priority to 1
	NVIC_SetPriority(TIM4_IRQn, 1);

	//Enable TIM4 interrupt in NVIC
	NVIC_EnableIRQ(TIM4_IRQn);

}

void TIM4_IRQHandler(void){
	if((TIM4->SR & TIM_SR_CC1IF) != 0){
	//Calculate sine of v and multiply by volume
		double result = sine(v)*volume;
		//Sent result to DAC
		DAC->DHR12R2 = (int)result;
		//Increment v by desired angle	
		v+=angle;
		//If v reaches 360 degress, reset to 0		
		if(v >= 360) v = 0;
		//Clear flag		
		TIM4->SR &= ~TIM_SR_CC1IF;
	}
	//Clear flag
	TIM4->SR &= ~TIM_SR_UIF;
}

void delay(int ms){
	//Wait for number of ms
	TimingDelay= ms;
	while(TimingDelay!=0);
}

void playsong(struct song song){
	int counter;
	for(counter=0; counter<song.notes; counter++){
		volume=song.play[counter].volume;
		angle = song.play[counter].angle;
		delay(song.play[counter].duration);
		
		volume=0;
		angle=0;
		delay(10);
	}
}