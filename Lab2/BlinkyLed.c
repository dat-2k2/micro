#include "stm32f2xx.h" // Device header
void delay () {
	unsigned long i; 
	i=0; 
	for(; i<200000; i++){} // Delay 
} 
// Main function  
int main () { 
	RCC->AHB1ENR |= 1ul<<6; // Enable port G clocking 
	GPIOG->MODER = (GPIOG->MODER & ~1ul<<15) | 1ul<<14; 
	for (;;) 
	{
	GPIOG->ODR |= 1ul<<7; 
	delay (); 
	GPIOG->ODR &= ~1ul<<7; 
	delay (); 
	} 
}