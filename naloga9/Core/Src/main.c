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
#include <stdlib.h>
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
static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram); // prototip
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


	uint32_t find_max(uint32_t* addr, uint32_t size){
		uint32_t max = 0;
		for(uint32_t i = 0; i < size; i++){
			if(addr[i] > max){
				max = addr[i];
			}
		}
		return max;
	}

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

  /* USER CODE BEGIN 1 */

	SCB_EnableICache();
	SCB_EnableDCache();

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/

  //to zakomentiramo
  // MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  GPIO_InitTypeDef gpio_init_structure;
  gpio_init_structure.Mode = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull = GPIO_PULLUP;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;
  gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8| GPIO_PIN_9;
  gpio_init_structure.Pin |= GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);
  gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7| GPIO_PIN_8;
  gpio_init_structure.Pin |= GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  gpio_init_structure.Pin |= GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  gpio_init_structure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2| GPIO_PIN_3;
  gpio_init_structure.Pin |= GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12;
  gpio_init_structure.Pin |= GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  gpio_init_structure.Pin  = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
  gpio_init_structure.Pin |= GPIO_PIN_8 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
  gpio_init_structure.Pin   = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 ;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);


  __HAL_RCC_FMC_CLK_ENABLE();
  FMC_SDRAM_TimingTypeDef  SDRAM_Timing = {0};
  SDRAM_Timing.LoadToActiveDelay    = 2;
  SDRAM_Timing.ExitSelfRefreshDelay = 7;
  SDRAM_Timing.SelfRefreshTime      = 4;
  SDRAM_Timing.RowCycleDelay        = 7;
  SDRAM_Timing.WriteRecoveryTime    = 2;
  SDRAM_Timing.RPDelay              = 2;
  SDRAM_Timing.RCDDelay             = 2;

  SDRAM_HandleTypeDef      hsdram = {0};
  hsdram.Instance = FMC_SDRAM_DEVICE;
  hsdram.Init.SDBank             = FMC_SDRAM_BANK2;
  hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
  hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_3;
  hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_DISABLE;
  HAL_SDRAM_Init(&hsdram, &SDRAM_Timing);

  SDRAM_Initialization_Sequence(&hsdram);

  // DMA za FIFO:
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


  volatile uint32_t* tabela = (volatile uint32_t*) 0xD0000000;
  for(uint32_t i = 0; i < 30000; i++){
	  tabela[i] = i;
  }
  volatile uint32_t* destination = (volatile uint32_t*) 0xC0000000;

  HAL_DMA_Start(&dma1_struct, (uint32_t)tabela, (uint32_t)destination, 30000);
  HAL_DMA_PollForTransfer(&dma1_struct, HAL_DMA_FULL_TRANSFER, HAL_MAX_DELAY);

/*
 * SRAM:
 *
  uint32_t* tabela = malloc(30000 * sizeof(uint32_t));
  for(uint32_t i = 0; i < 30000; i++){
	  tabela[i] = i;
  }
*/

  start_timer();
  find_max(tabela, 30000);
  stop_timer();
  uint32_t cas = get_time();

//  free(tabela);

  /* USER CODE END 2 */

  /****** IZMERJENI CAS ******
   * - SDRAM, brez predpomnilnika in brez FIFO: 40971
   * - SRAM, brez predpomnilnika: 22094
   * - SDRAM, s predpomnilnikom in brez FIFO: 31121
   * - SRAM, s predpomnilnikom: 8967
   * - SDRAM, s predpomnilnikom in FIFO: 31120 -- nekaj narobe s FIFO, moral bi bit ful manj
   *
  */


  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)
#define REFRESH_COUNT                    ((uint32_t)0x0603)

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
  FMC_SDRAM_CommandTypeDef command;
  __IO uint32_t tmpmrd =0;

  // 1. vklopimo uro
  command.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

  // pocakamo 100 us
  HAL_Delay(1);

  // posljemo precharge all ukaz
  command.CommandMode = FMC_SDRAM_CMD_PALL;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

  // posljemo auto refresh ukaz(e)
  command.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  command.AutoRefreshNumber = 8;
  command.ModeRegisterDefinition = 0;
  HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

  // nastavimo mode register SDRAMa
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
					 SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
					 SDRAM_MODEREG_CAS_LATENCY_3           |
					 SDRAM_MODEREG_OPERATING_MODE_STANDARD |
					 SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  command.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  command.AutoRefreshNumber = 1;
  command.ModeRegisterDefinition = tmpmrd;
  HAL_SDRAM_SendCommand(hsdram, &command, SDRAM_TIMEOUT);

  // nastavimo stevec osvezevanja
  HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);
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
