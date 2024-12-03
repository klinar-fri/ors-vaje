/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

// funkcija za merjenje casa
void start_timer() {
  __HAL_RCC_TIM2_CLK_ENABLE();
  TIM_HandleTypeDef timer = {0};
  timer.Instance = TIM2;
  timer.Init.CounterMode = TIM_COUNTERMODE_UP;
  timer.Init.Period = 100000000;
  timer.Init.Prescaler = 64 -1;
  HAL_TIM_Base_Init(&timer);

  HAL_TIM_Base_Start(&timer);
  __HAL_TIM_SetCounter(&timer, 0);
}

void stop_timer() {
  TIM_HandleTypeDef timer = {0};
  timer.Instance = TIM2;
  HAL_TIM_Base_Stop(&timer);
}

uint32_t get_time() {
  TIM_HandleTypeDef timer = {0};
  timer.Instance = TIM2;
  return __HAL_TIM_GetCounter(&timer);
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SIZE 100
uint32_t source[SIZE];
uint32_t destination[SIZE];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  // ko se vsak loop zgodi se to pretvori v:
  // lw (load word - preberemo podatek iz pomnilnika v register CPE)
  // sw (store word - shranimo podatek iz registra CPE v pomnilnik)
  // to je neučinkovita raba CPE

  // zato bomo danes uporabljali direktni dostop do pomnilnika s pomočjo DMA naprave

  for(int i = 0; i < SIZE; i++){
	  destination[i] = i;
  }

  /* USER CODE END SysInit */

  // uporaba teh funkcij:
  start_timer();
  HAL_Delay(200);
  stop_timer();
  // izmeriti bi morali okoli 200 000 mikrosekund
  // (zaradi nenatancosti HAL_Delay na nivoju mikrosekund jih boste izmerili nekaj vec)
  uint32_t izmerjen_cas_v_us = get_time();

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */

  __HAL_RCC_DMA1_CLK_ENABLE();

  DMA_HandleTypeDef dma1_struct = {0};

  dma1_struct.Instance = DMA1_Stream0;
  dma1_struct.Init.Request = DMA_REQUEST_MEM2MEM;
  dma1_struct.Init.Direction = DMA_MEMORY_TO_MEMORY;
  dma1_struct.Init.PeriphInc = DMA_PINC_ENABLE;
  dma1_struct.Init.MemInc = DMA_MINC_ENABLE;
  dma1_struct.Init.PeriphDataAlignment = DMA_MDATAALIGN_WORD;
  dma1_struct.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  dma1_struct.Init.Mode = DMA_NORMAL;
  dma1_struct.Init.Priority = DMA_PRIORITY_LOW;
  dma1_struct.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  dma1_struct.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
  HAL_DMA_Init(&dma1_struct);

  // source in destination definiramo kot polji dolzine SIZE
  // uint32_t source[SIZE];
  // uint32_t destination[SIZE];
  // SIZE * 4, ker je dolžina prenosa podana v bytih

  // start
  HAL_DMA_Start(&dma1_struct, (uint32_t)source, (uint32_t)destination, SIZE * 4);
  // čakamo dokler se prenos ne konča
  HAL_DMA_PollForTransfer(&dma1_struct, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);
  // stop

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
