#include "stm32f2xx_hal.h" 
GPIO_InitTypeDef GPIO_InitStruct; 
TIM_HandleTypeDef htim; 
TIM_OC_InitTypeDef outputChannelInit; 

void InitializeGPIO() 
 { 
	 RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; 
	 /* GPIO base configuration */ 
	 GPIO_InitStruct.Pin = (GPIO_PIN_12|GPIO_PIN_15); 
	 GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;; 
	 GPIO_InitStruct.Speed = GPIO_SPEED_LOW; 
	 GPIO_InitStruct.Alternate = GPIO_AF2_TIM4; 
	 HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 
 } 
 
 void InitializeTimer() 
 //1.6MHz baseclock
	{ 
	 RCC->APB1ENR |= RCC_APB1ENR_TIM4EN ; 
	 /* Time base configuration */ 
	 htim.Instance = TIM4; 
	 htim.Init.Period = 0x500; 
	 htim.Init.Prescaler = 1250ul; 
	 htim.Init.ClockDivision = 0; 
	 htim.Init.RepetitionCounter = 0; 
	 htim.Init.CounterMode = TIM_COUNTERMODE_UP; 
	 HAL_TIM_Base_Init(&htim); 
	}
	
	
void InitializePWMChannel() 
 { 
	 /* Channel base configuration */ 
	 outputChannelInit.OCMode = TIM_OCMODE_PWM1; 
	 outputChannelInit.Pulse = 0x400; 
	 outputChannelInit.OCFastMode = TIM_OCFAST_DISABLE; 
	 outputChannelInit.OCPolarity = TIM_OCPOLARITY_HIGH; 
	 HAL_TIM_OC_Init(&htim); 
	 HAL_TIM_OC_ConfigChannel(&htim, &outputChannelInit, 
	TIM_CHANNEL_1); 
	 /* Enable TIM peripheral counter */ 
	 HAL_TIM_OC_Start(&htim, TIM_CHANNEL_1); 
 } 
void InitializePWMChannel2() 
 { 
 /* Channel 2 base configuration */ 
 outputChannelInit.OCMode = TIM_OCMODE_PWM1; 
 outputChannelInit.Pulse = 0x100; 
 outputChannelInit.OCFastMode = TIM_OCFAST_DISABLE; 
 outputChannelInit.OCPolarity = TIM_OCPOLARITY_HIGH; 
 HAL_TIM_OC_Init(&htim); 
 HAL_TIM_OC_ConfigChannel(&htim, &outputChannelInit, 
TIM_CHANNEL_4); 
 /* Enable TIM peripheral counter */ 
 HAL_TIM_OC_Start(&htim, TIM_CHANNEL_4); 
 } 
 
int main() 
{ 
	 InitializeGPIO(); 
	 InitializeTimer(); 
	 InitializePWMChannel(); 
	InitializePWMChannel2();
	 for (;;) 
	 { 
	 } 
}	
	