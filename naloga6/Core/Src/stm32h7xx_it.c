/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32h7xx_it.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/


extern UART_HandleTypeDef uart;

int stanjePrve = 0;
int stanjeDruge = 0;
int stanjeTretje = 0;

void ledFunct(char idLedChar, int st) {
    int idLed = atoi(&idLedChar);
    char buffer[100];

    if (idLed == 1) {
        if (st == 1) {
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, ENABLE); // on 1
            stanjePrve = 1;
        } else {
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, DISABLE); // off 1
            stanjePrve = 0;
        }
    } else if (idLed == 2) {
        if (st == 1) {
            HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, DISABLE); // on 2
            stanjeDruge = 1;
        } else {
            HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2, ENABLE); // off 2
            stanjeDruge = 0;
        }
    } else if (idLed == 3) {
        if (st == 1) {
            HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, DISABLE); // on 3
            stanjeTretje = 1;
        } else {
            HAL_GPIO_WritePin(GPIOI, GPIO_PIN_13, ENABLE); // off 3
            stanjeTretje = 0;
        }
    }


    sprintf(buffer, "LED: %s, %s, %s\r\n",
		(stanjePrve == 1) ? "ON" : "OFF",
		(stanjeDruge == 1) ? "ON" : "OFF",
		(stanjeTretje == 1) ? "ON" : "OFF");

    HAL_UART_Transmit(&uart, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}


void checkCommand(char command[10]){
	int stanje = 0; // 0 - ugasni, 1 - prižgi
	if(command[7] == 'N' || command[7] == 'n'){
		stanje = 1;
	}else{
		stanje = 0;
	}
	ledFunct(command[4], stanje);
}

// tolk časa se pa že dolg nisem j*bov z debuggiranjem moram priznat



int i = 0;
char command[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int idLed = 0;

/* USER CODE BEGIN 1 */
void USART3_IRQHandler() {
  if (__HAL_UART_GET_IT(&uart, UART_IT_RXNE)) {
	command[i] = USART3->RDR;
	if(i == 10 || command[i] == '\r'){
		checkCommand(command);
		i = 0;
	}else{
		i++;
	}
  }
}
/* USER CODE END 1 */
