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



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define SIZE 100
uint32_t source[SIZE];
uint32_t destination[SIZE];

DMA_HandleTypeDef dma1_struct = {0};
UART_HandleTypeDef uart;

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


/* Declare a global flag to track DMA completion */
volatile uint8_t dma_transfer_complete = 0;

/* DMA Complete Callback */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        dma_transfer_complete = 1; // Mark transfer as complete
    }
}

/* IRQ Handlers */
void DMA1_Stream0_IRQHandler(void) {
    HAL_DMA_IRQHandler(&dma1_struct);
}

void USART3_IRQHandler(void) {
    HAL_UART_IRQHandler(&uart);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  MPU_Config();
  HAL_Init();
  SystemClock_Config();


  /* LED INIT */

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	GPIO_InitTypeDef zelena = {0};
	GPIO_InitTypeDef zelena2 = {0};
	GPIO_InitTypeDef rdeca = {0};


	// zelena
	zelena.Pin = GPIO_PIN_3;
	zelena.Mode = GPIO_MODE_OUTPUT_PP;
	zelena.Pull = GPIO_NOPULL;
	zelena.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &zelena);

	// zelena2
	zelena2.Pin = GPIO_PIN_2;
	zelena2.Mode = GPIO_MODE_OUTPUT_PP;
	zelena2.Pull = GPIO_NOPULL;
	zelena2.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOJ, &zelena2);

	// rdeca
	rdeca.Pin = GPIO_PIN_13;
	rdeca.Mode = GPIO_MODE_OUTPUT_OD;
	rdeca.Pull = GPIO_PULLUP;
	rdeca.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOI, &rdeca);

    // na zacetku oba ugasnemo
    HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, ENABLE);
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, ENABLE);


  __HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef init_structure;
  init_structure.Pin = GPIO_PIN_10 | GPIO_PIN_11;
  init_structure.Mode = GPIO_MODE_AF_PP;
  init_structure.Pull = GPIO_NOPULL;
  init_structure.Speed = GPIO_SPEED_FREQ_LOW;
  init_structure.Alternate = GPIO_AF7_USART3;
  HAL_GPIO_Init(GPIOB, &init_structure);

  __HAL_RCC_USART3_CLK_ENABLE();
  uart.Instance = USART3;
  uart.Init.BaudRate = 115200;
  uart.Init.WordLength = UART_WORDLENGTH_8B;
  uart.Init.StopBits = UART_STOPBITS_1;
  uart.Init.Parity = UART_PARITY_NONE;
  uart.Init.Mode = UART_MODE_TX_RX;
  uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HAL_UART_Init(&uart);


  __HAL_RCC_DMA1_CLK_ENABLE();

  dma1_struct.Instance = DMA1_Stream0;
  dma1_struct.Init.Request = DMA_REQUEST_USART3_TX;
  dma1_struct.Init.Direction = DMA_MEMORY_TO_PERIPH;
  dma1_struct.Init.PeriphInc = DMA_PINC_DISABLE;
  dma1_struct.Init.MemInc = DMA_MINC_ENABLE;
  dma1_struct.Init.PeriphDataAlignment = DMA_MDATAALIGN_BYTE;
  dma1_struct.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  dma1_struct.Init.Mode = DMA_NORMAL;
  dma1_struct.Init.Priority = DMA_PRIORITY_LOW;
  dma1_struct.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
  dma1_struct.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_1QUARTERFULL;
  HAL_DMA_Init(&dma1_struct);


  __HAL_LINKDMA(&uart, hdmatx, dma1_struct);


  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);

  HAL_NVIC_SetPriority(USART3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);



  /*
  ******************* IZMERJENA CASA: ****************
   * HAL_UART_TRANSMIT: 3475016µ ~ 3.5 sekund		*
   * HAL_UART_TRANSMIT_DMA: 3474854µs ~ 3.5 sekund	*
  ****************************************************
 */

  uint8_t txt[40000];
  for(int i = 0; i < 40000; i++){
	  txt[i] = 'K';
  }


  /* CHAT GPT - pomoč, moja rešitev ne deluje ?
   - ni mi cist jasn kako nardit to brez da bi prekinu prenos,
   in brez prekinitev

   *
   *
   // start
    start_timer();
    HAL_UART_Transmit_DMA(&uart, txt, sizeof(txt));

    // gre v loop sam če je HAL_MAX_DELAY = 0, potem pa prenese samo 3 znake od 40000 ?

    while (HAL_DMA_PollForTransfer(&dma1_struct, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY) != HAL_OK) {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, ENABLE); // on 1
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, DISABLE); // on 2
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, DISABLE); // on 3
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, DISABLE); // off 1
		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, ENABLE); // off 2
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, ENABLE); // off 3
		HAL_Delay(100);
    }

    // Stop Timer
    stop_timer();

    uint32_t izmerjen_cas_v_us = get_time();
   *
  */

  // Start
  start_timer();
  dma_transfer_complete = 0;
  //HAL_UART_Transmit(&uart, txt, sizeof(txt), HAL_MAX_DELAY);
  HAL_UART_Transmit_DMA(&uart, txt, sizeof(txt));

  // utripaj
  while(!dma_transfer_complete){
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, ENABLE); // on 1
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, DISABLE); // on 2
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, DISABLE); // on 3
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, DISABLE); // off 1
	HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, ENABLE); // off 2
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, ENABLE); // off 3
	HAL_Delay(100);
  }

  stop_timer();

  uint32_t izmerjen_cas_v_us = get_time();

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
