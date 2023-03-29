#include "stm32f2xx.h"
#define TIME_DELAY_ 10000ul;
//Citation: RM as Reference Manual, PM as Prog Manual
unsigned int j;
unsigned int k;
const int COUNTER = 200ul;
void delay(){
	for (j = 0; j < 100; j++){
	};
}

void PG8(){
	GPIOG->ODR |= 1ul << 8;
}

void PG8off(){
		GPIOG->ODR &= ~(1ul << 8);
} 	
void PG7(){
	GPIOG->ODR |= 1ul << 7;
}

void PG7off(){
		GPIOG->ODR &= ~(1ul << 7);
}
int main(){
	//setup GPIO
	//set PAx and PGx as GPIO
	//RM: 5.3.10
		RCC->AHB1ENR |= (1ul | (1ul<<6));
	//set PA0 as input
	//RM: 8.4.1
		GPIOA->MODER &= ~1ul;
		GPIOA->MODER &= ~(1ul <<1);
	//set PG8 and PG7 as output
	//RM: 8.4.1
		GPIOG->MODER = (GPIOG->MODER | (1ul<<14) | (1ul<<16)) & ~(1ul<<15) & ~(1ul<<17);  
delay();
	//Config the EXTI
	//Enable System clock SYSCFG
		//RM 5.3.14
	RCC->APB2ENR |= (1ul <<14);
	RCC->APB1ENR |= 1ul;
	//unmask 
	EXTI->IMR |= (1ul);
	//set line 0 to rising trigger,RM 8.3.4,4
	EXTI->FTSR &= ~1ul;
	EXTI->RTSR |= 1ul;
	
	//Reset pending bit , RM 8.3.8
	EXTI->PR |= 1ul;
	
	//Set EXTI0 to capture interrupt from PGAx
	//RM 7.2.3
	SYSCFG->EXTICR[0] |= 0x0000;
	delay();
	//Configure TIM
	//enable update interrupt
	//RM 18.4.3
	TIM2->DIER |=1ul;
	//f_TIM8 = f_SYSCLK/2 = 188e8/2 = 84e8
	TIM2->CR1 &= ~(1ul<<3);
	// freq calling interrupt = f/((PSC+1) * (ARR+1)); 
	//set prescaler
	TIM2->PSC = 8400ul-1;
	//set auto-reload value
	TIM2->ARR = 1000ul-1;
	//* -1 bc count from 0. Read RM 18.3.1
		
		//prevent Flash from turning off in stop mode
		//must go together
		//Power down in deepsleep
		PWR->CR &= ~(1ul<<1);
		//Low power deepsleep
		PWR->CR |= 1ul;
		TIM2->CR1 |= 1ul;		
		//NVIC setup
	NVIC_SetPriorityGrouping(4); //2 preempt (bb00)
	//set priority	
	NVIC_SetPriority(EXTI0_IRQn, 2);
	NVIC_SetPriority(TIM2_IRQn, 4);
	//enable EXTI
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);
	
	while (1){
		if ((unsigned int)TIM2->CNT > ((unsigned int)TIM2->ARR >> 1)){
			PG8off();
			PG7();
		}
		else{
			PG7off();
			PG8();
		}
	}
}



//Setting stop mode, not sleep mode.
// wake up by any exti line
//Handling PA0 pressed interrupt, 
void EXTI0_IRQHandler(){
	//clear interrupt flag
	EXTI->PR |= 1ul;
	if (GPIOA->IDR & 1ul){
		//enter sleepmode
		//change to falling trigger
			EXTI->FTSR |= 1ul;
			EXTI->RTSR &= ~1ul;
		//set to sleep deep and sleep on exit
		//PM 4.4.8
			SCB->SCR |= ((1ul <<1) | (1ul<<2));
		//TIM enable
		}
	else {
			SCB->SCR &= ~((1ul <<1) | (1ul<<2));
			EXTI->FTSR &= ~1ul;
			EXTI->RTSR |= 1ul;
	}
}
void TIM2_IRQHandler(){
	TIM2->SR &= 0;
}