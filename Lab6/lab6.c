#include "stm32f2xx_hal.h" 
GPIO_InitTypeDef GPIO_InitStruct; 
TIM_HandleTypeDef htim; 
unsigned int i;
void InitializeLED() 
 { 
 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; 
 /* GPIO base configuration */ 
 GPIO_InitStruct.Pin = (GPIO_PIN_7 | GPIO_PIN_8); 
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
 GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 
 HAL_GPIO_Init(GPIOG, &GPIO_InitStruct); 
 HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7 | GPIO_PIN_8, 
GPIO_PIN_RESET); 
 } 
 
 void InitializeTimer() 
 { 
	 RCC->APB1ENR |= RCC_APB1ENR_TIM2EN ; 
	 /* Time base configuration */ 
	 htim.Instance = TIM2; 
	 htim.Init.Period = 0x500; //
	 htim.Init.Prescaler = 4000ul; 
	 htim.Init.ClockDivision = 0; 
	 htim.Init.RepetitionCounter = 0; 
	 htim.Init.CounterMode = TIM_COUNTERMODE_UP; 
	 HAL_TIM_Base_Init(&htim); 
	 /* Enable TIM peripheral counter */ 
	 HAL_TIM_Base_Start(&htim); 
	//(htim.Instance)->DIER |=1ul;
 } 
 
// void EnableTimerInterrupt() 
// { 
// NVIC_SetPriorityGrouping(4); 
// NVIC_SetPriority(TIM2_IRQn,0); 
// NVIC_EnableIRQ(TIM2_IRQn); 
// } 
//void TIM2_IRQHandler() 
// { 
//	 {
//		 TIM2->SR &= 0; //Reset Pending Bit 
//		 HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_7); 
//	 } 
// } 
unsigned int t;
 unsigned int timerMax, riseValue;
 unsigned int riseValues[5] , fallValues[5];
 int main() 
	{ 
		
	InitializeLED(); 
	 InitializeTimer(); 
	//EnableTimerInterrupt();
		
		
		timerMax = (htim.Instance)->ARR;
	 	riseValue = timerMax * (100 - 20)/100;
	 //init rise and fall values
	 riseValues[0] = riseValue;
	 fallValues[0] = riseValue + (timerMax - riseValue)/10;
	 	for (t = 1; t < 5; t++){
	 		riseValues[t] = riseValues[t-1] + (timerMax - riseValue)/5;
	 		fallValues[t] = fallValues[t-1] + (timerMax - riseValue)/5;
	 	}

	 for (;;)
		{ 
			 int timerValue = TIM2->CNT;
//			// for single wave
//	 		if (timerValue == riseValue)
//				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
//			else if (timerValue == timerMax)
//				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
	//
			// for multi triggers
			if (timerValue == riseValues[0] ||timerValue == riseValues[1]||timerValue == riseValues[2]||timerValue == riseValues[3]||timerValue == riseValues[4]) 
				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
			else if (timerValue == fallValues[0] ||timerValue == fallValues[1]||timerValue == fallValues[2]||timerValue == fallValues[3]||timerValue == fallValues[4]) 
				HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);	 
	//		for (i = 0; i < 1000000; i++) 
	//			__nop(); 
	//			HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_8); 


		} 
	}
	// Measure on oscillator before this
	

 //Conclusion: The programm counter "for" independent from peripheral 