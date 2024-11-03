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

  // PC13 - gumb
  // PD3 - led (push-pull brez pull)
  // PJ2 - led (push-pull brez pull)
  // PI13 - led (open-drain + pull-up)

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  GPIO_InitTypeDef zelena = {0};
  GPIO_InitTypeDef zelena2 = {0};
  GPIO_InitTypeDef rdeca = {0};
  GPIO_InitTypeDef gumb = {0};

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

  // gumb
  gumb.Pin = GPIO_PIN_13;
  gumb.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &gumb);


  // na zacetku oba ugasnemo
  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, ENABLE);
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, ENABLE);

  uint32_t pressCount = 0;
  uint32_t prejStGumba = 0;

  void prizgiRdeco(){
	  uint32_t stanjeGumba= HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	  // rdeča
	  if(stanjeGumba && !prejStGumba){
		  pressCount++;
		  if(pressCount % 2 != 0){
			  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, DISABLE);
		  }else{
			  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, ENABLE);
		  }
	  }

	  prejStGumba = stanjeGumba;
  }


  while (1){

	  // več teh prizgiRdečo, da bolje dela... ?

	  prizgiRdeco();

	  // obe zeleni

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, ENABLE); // on 1
	  HAL_Delay(250);

	  prizgiRdeco();

	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, DISABLE); // off 1
	  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, DISABLE); // on 2
	  HAL_Delay(250);

	  prizgiRdeco();
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, ENABLE); // on 1
	  HAL_Delay(250);

	  prizgiRdeco();
	  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, DISABLE); // off 1
	  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, ENABLE); // off 2
	  HAL_Delay(250);
  }



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
