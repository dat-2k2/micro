//WAKEUP -> PG6, USER ->PG8
#include "stm32f2xx.h"
#include "stm32f2xx_hal_gpio.h"
#define TIME_DELAY 200000
unsigned int j;
void delay(){
	for (j = 0 ; j < 10; j++){};
}
void PG7(){
	while (1){
		GPIOG->ODR |= (1ul << 7);
		for (j = 0; j < TIME_DELAY; j++) {};
		GPIOG->ODR &= ~(1ul << 7);
		for (j = 0; j < TIME_DELAY; j++) {};
	}
}
void PG7off(){
			GPIOG->ODR &= ~(1ul << 7);
}
void PG6off(){
			GPIOG->ODR &= ~(1ul << 6);
}
void PG8off(){
			GPIOG->ODR &= ~(1ul << 8);
}
void PG6(){
	while(1){
		GPIOG->ODR |= (1ul << 6);
		for (j = 0; j < TIME_DELAY; j++) {};
		GPIOG->ODR &= ~(1ul << 6);
		for (j = 0; j < TIME_DELAY; j++) {};
		}
}

void PG8(){
	while (1){
		GPIOG->ODR |= (1ul << 8);
		for (j = 0; j < TIME_DELAY; j++) {};
		GPIOG->ODR &= ~(1ul << 8);
		for (j = 0; j < TIME_DELAY; j++) {};
		}
}


int main()
	{

	int i;
		i=0;	
		PG7off(); PG6off(); PG8off();
	RCC->AHB1ENR |= 1ul<<6;
	RCC->AHB1ENR |= 1ul;
	RCC->AHB1ENR |= 1ul <<2;
		delay();
	//PG7 out
	GPIOG->MODER = (GPIOG->MODER & ~(1ul<<15)) | 1ul<<14; 
	GPIOG->OSPEEDR = (GPIOG->OSPEEDR | (1ul<<15)) | (1ul<<14);
	//PG8 out
	GPIOG->MODER = (GPIOG->MODER & ~(1ul<<17)) | 1ul<<16;
	GPIOG->OSPEEDR = (GPIOG->OSPEEDR | (1ul<<17)) | (1ul<<16);
	//PG6 out
	GPIOG->MODER = (GPIOG->MODER & ~(1ul<<13)) | 1ul<<12;
	GPIOG->OSPEEDR = (GPIOG->OSPEEDR | (1ul<<13)) | (1ul<<12);
		delay();

	//PA0 in
	GPIOA->MODER = (GPIOA->MODER & ~(1ul<<1)) & ~1ul;
	//PG15 in
	GPIOG->MODER = (GPIOG->MODER & ~(1ul<<31)) & ~(1ul<<30);

	//SPEED
	//delay 4
		delay();
	//tac, 14-th bit, enable SYSCFG
	RCC->APB2ENR = RCC->APB2ENR | (1ul<<14);
	//setup the EXTI Wakeup event for bit G15 and A0
	//unmask
	EXTI->IMR |= 1ul;
	EXTI->IMR |= (1ul<<15);
	//falling trigger for input PG15 /PC13
	EXTI->RTSR = EXTI->RTSR & ~(1ul<<15);
	EXTI->FTSR = EXTI->FTSR | (1ul<<15);
	//rising trigger for input PA0
	EXTI->FTSR = EXTI->FTSR & ~1ul;
	EXTI->RTSR = EXTI->RTSR | 1ul ;
	//pending reg
	EXTI->PR |= 1ul;
	EXTI->PR |= (1ul<<15);
	//config external interrupt
	//EXTI15, port G
	SYSCFG->EXTICR[3] |= 0x6000 ;
	//EXTI0, port A
	SYSCFG->EXTICR[0] |= 0x0000;
		for (i = 0; i < 10; i++) {};

	//NVIC setup
	NVIC_SetPriorityGrouping(4); //2 preempt (bb00)
	//set priority	
	NVIC_SetPriority(EXTI15_10_IRQn,4);
	NVIC_SetPriority(EXTI0_IRQn, 2);
	//here the PA0 can preempt PG15
	// Event handler interrupts blinking process of PG7
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
//PG7();
	for(;;){
	PG7();
	}
}
	
void EXTI0_IRQHandler(){
	//
	PG8off();
	PG6();
	// reset event
	EXTI->PR |= (1ul); 
}

void EXTI15_10_IRQHandler(){
	PG6off();
	PG8();
	//reset event
	EXTI->PR |= (1ul<<15); 
}