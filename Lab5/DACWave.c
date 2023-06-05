/** 
 
**********************************************************************
******** 
 *Name: DACWave.c 
 *Purpose: DAC for MCBSTM32F200 
 * 
 
**********************************************************************
******** 
 */ 
/* Includes -----------------------------------------------------
-------------*/ 
#include "stm32f2xx_hal.h" 
/* Private typedef ----------------------------------------------
-------------*/ 
/* Private define -----------------------------------------------
-------------*/ 
/* Definition for DAC clock resources */ 
#define DACx_CHANNEL1_GPIO_CLK_ENABLE() __GPIOA_CLK_ENABLE() 
#define DMAx_CLK_ENABLE() __DMA1_CLK_ENABLE() 
/* Definition for DACx Channel1 Pin */ 
#define DACx_CHANNEL1_PIN GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT GPIOA 
/* Definition for DACx's Channel1 */ 
#define DACx_CHANNEL1 DAC_CHANNEL_1 
/* Definition for DACx's DMA Channel1 */ 
#define DACx_DMA_CHANNEL1 DMA_CHANNEL_7 
#define DACx_DMA_STREAM1 DMA1_Stream5 
/* Private macro ------------------------------------------------
-------------*/ 
/* Private variables --------------------------------------------
-------------*/ 
DAC_HandleTypeDef DacHandle; 
static DAC_ChannelConfTypeDef sConfig; 

//sine wave look up table, for 10 samples. 
const uint16_t Wave[10] = {2048, 3252, 3996, 3996, 3252, 2048, 844, 100, 100, 844}; 

/* Private function prototypes ----------------------------------
-------------*/ 
static void DAC_Ch1_WaveConfig(void); 
static void TIM6_Config(void); 
static void SystemClock_Config(void); 
/* Private functions --------------------------------------------
-------------*/ 
/** 
 * @brief DAC MSP De-Initialization 
 * This function frees the hardware resources: 
 * - Disable the Peripheral's clock 
 * - Revert GPIO to their default state 
 * @param hadc: DAC handle pointer 
 * @retval None 
 */ 
void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac) 
{ 
 GPIO_InitTypeDef GPIO_InitStruct; 
 static DMA_HandleTypeDef hdma_dac1; 
 /*##-1- Enable peripherals and GPIO Clocks 
#################################*/ 
 /* DAC Periph clock enable */ 
 __DAC_CLK_ENABLE(); 
 /* Enable GPIO clock ****************************************/ 
 DACx_CHANNEL1_GPIO_CLK_ENABLE(); 
 /* DMA1 clock enable */ 
 DMAx_CLK_ENABLE(); 
 /*##-2- Configure peripheral GPIO 
##########################################*/ 
 /* DAC Channel1 GPIO pin configuration */ 
 GPIO_InitStruct.Pin = DACx_CHANNEL1_PIN; 
 GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; 
 GPIO_InitStruct.Pull = GPIO_NOPULL; 
 HAL_GPIO_Init(DACx_CHANNEL1_GPIO_PORT, & GPIO_InitStruct); 
 /*##-3- Configure the DMA streams 
##########################################*/ 
 /* Set the parameters to be configured for Channel1*/ 
 hdma_dac1.Instance = DACx_DMA_STREAM1; 
 hdma_dac1.Init.Channel = DACx_DMA_CHANNEL1; 
 hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH; 
 hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE; 
 hdma_dac1.Init.MemInc = DMA_MINC_ENABLE; 
 hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; 
 hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; 
 hdma_dac1.Init.Mode = DMA_CIRCULAR; 
 hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH; 

 hdma_dac1.Init.FIFOMode = DMA_FIFOMODE_DISABLE; 
 hdma_dac1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL; 
 hdma_dac1.Init.MemBurst = DMA_MBURST_SINGLE; 
 hdma_dac1.Init.PeriphBurst = DMA_PBURST_SINGLE; 
 HAL_DMA_Init(& hdma_dac1); 
 /* Associate the initialized DMA handle to the the DAC handle 
*/ 
 __HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac1); 
} 
/** 
 * @brief TIM MSP Initialization 
 * This function configures the hardware resources: 
 * - Peripheral's clock enable 
 * - Peripheral's GPIO Configuration 
 * @param htim: TIM handle pointer 
 * @retval None 
 */ 
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim) 
{ 
 /* TIM6 Periph clock enable */ 
 __TIM6_CLK_ENABLE(); 
} 
/** 
 * @brief Main program. 
 * @param None 
 * @retval None 
 */ 
int main(void) 
{ 
 /* STM32F2xx HAL library initialization: 
 - Configure the Flash prefetch, instruction and Data 
caches 
 - Configure the Systick to generate an interrupt each 1 
msec 
 - Set NVIC Group Priority to 4 
 - Global MSP (MCU Support Package) initialization 
 */ 
 HAL_Init(); 
 /* Configure the system clock to have a system clock = 120 MHz 
*/ 
 SystemClock_Config(); 
 /*##-1- Configure the DAC peripheral 
#######################################*/ 
 DacHandle.Instance = DAC; 
 /*##-2- Configure the TIM peripheral 
#######################################*/ 
 TIM6_Config(); 

 /* Wave generator -------------------------------------------
*/ 
 DAC_Ch1_WaveConfig(); 
 /* Infinite loop */ 
 while (1) {} 
} 
/** 
 * @brief System Clock Configuration 
 * The system Clock is configured as follow : 
 * System Clock source = PLL (HSE) 
 * SYSCLK(Hz) = 120000000 
 * HCLK(Hz) = 120000000 
 * AHB Prescaler = 1 
 * APB1 Prescaler = 4 
 * APB2 Prescaler = 2 
 * HSE Frequency(Hz) = 25000000 
 * PLL_M = 25 
 * PLL_N = 240
 * PLL_P = 2 
 * PLL_Q = 5 
 * VDD(V) = 3.3
 * Flash Latency(WS) = 3 
 * @param None 
 * @retval None 
 */ 
static void SystemClock_Config(void) 
{ 
 RCC_ClkInitTypeDef RCC_ClkInitStruct; 
 RCC_OscInitTypeDef RCC_OscInitStruct; 
 /* Enable HSE Oscillator and activate PLL with HSE as source */ 
 RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE; 
 RCC_OscInitStruct.HSEState = RCC_HSE_ON; 
 RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON; 
 RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE; 
 RCC_OscInitStruct.PLL.PLLM = 25; 
 RCC_OscInitStruct.PLL.PLLN = 240; 
 RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2; 
 RCC_OscInitStruct.PLL.PLLQ = 5; 
 HAL_RCC_OscConfig(& RCC_OscInitStruct); 
 
 /* Select PLL as system clock source and configure the HCLK, 
PCLK1 and PCLK2 
 clocks dividers */ 
 RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | 
RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2); 
 RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; 
 RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; 
 RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
 RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

 HAL_RCC_ClockConfig(& RCC_ClkInitStruct, FLASH_LATENCY_3); 
} 
static void DAC_Ch1_WaveConfig(void) 
{ 
 /*##-1- Initialize the DAC peripheral 
######################################*/ 
 HAL_DAC_Init(& DacHandle); 
 
 /*##-2- DAC channel1 Configuration 
#########################################*/ 
 sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO; 
 sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE; 
 HAL_DAC_ConfigChannel(& DacHandle, & sConfig, DACx_CHANNEL1); 
 /*##-3- Enable DAC Channel1 and associeted DMA 
#############################*/ 
	//edit here
 HAL_DAC_Start_DMA(& DacHandle, DACx_CHANNEL1, (uint32_t *)Wave, 
10, DAC_ALIGN_12B_R); 
 /*##-4- Enable DAC Channel1 
################################################*/ 
 HAL_DAC_Start(& DacHandle, DACx_CHANNEL1); 
 
 /*##-5- Set DAC channel1 DHR12RD register 
################################################*/
// 
 HAL_DAC_SetValue(& DacHandle, DACx_CHANNEL1, DAC_ALIGN_12B_R, 
4095); 
} 
/** 
 * @brief TIM6 Configuration 
 * @note TIM6 configuration is based on APB1 frequency 
 * @note TIM6 Update event occurs each TIM6CLK/256 
 * @param None 
 * @retval None 
 */ 
void TIM6_Config(void) 
{ 
 static TIM_HandleTypeDef htim; 
 TIM_MasterConfigTypeDef MasterConfig; 
 /*##-1- Configure the TIM peripheral 
#######################################*/ 
	/*Configure TIM6: f_DACout = */
 /* Time base configuration */ 
 htim.Instance = TIM6; 
//set period of TIM to set period of wave
 htim.Init.Period = 0x100; //16ns
 htim.Init.Prescaler = 6250; 
 htim.Init.ClockDivision = 0; 
 htim.Init.CounterMode = TIM_COUNTERMODE_UP; 
 HAL_TIM_Base_Init(& htim); 

 /* TIM6 TRGO selection */ 
 MasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE; 
 MasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE; 
 HAL_TIMEx_MasterConfigSynchronization(& htim, & MasterConfig); 
 /*##-2- Enable TIM peripheral counter 
######################################*/ 
 HAL_TIM_Base_Start(& htim); 
} 
/*********************************** END OF FILE 
*******************************/