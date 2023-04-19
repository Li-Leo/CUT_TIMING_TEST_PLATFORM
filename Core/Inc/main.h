/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define wire_2_Pin GPIO_PIN_14
#define wire_2_GPIO_Port GPIOC
#define wire_1_Pin GPIO_PIN_15
#define wire_1_GPIO_Port GPIOC
#define MAX_Pin GPIO_PIN_0
#define MAX_GPIO_Port GPIOA
#define MIN_Pin GPIO_PIN_1
#define MIN_GPIO_Port GPIOA
#define BUZZ_Pin GPIO_PIN_4
#define BUZZ_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_5
#define LCD_EN_GPIO_Port GPIOA
#define LCD_RW_Pin GPIO_PIN_6
#define LCD_RW_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_7
#define LCD_RS_GPIO_Port GPIOA
#define DB0_Pin GPIO_PIN_0
#define DB0_GPIO_Port GPIOB
#define DB1_Pin GPIO_PIN_1
#define DB1_GPIO_Port GPIOB
#define DB2_Pin GPIO_PIN_2
#define DB2_GPIO_Port GPIOB
#define DS1302_CE_Pin GPIO_PIN_12
#define DS1302_CE_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOB
#define IRD_Pin GPIO_PIN_14
#define IRD_GPIO_Port GPIOB
#define WORK_RST_Pin GPIO_PIN_15
#define WORK_RST_GPIO_Port GPIOB
#define CUT_DELAY_Pin GPIO_PIN_8
#define CUT_DELAY_GPIO_Port GPIOA
#define DB3_Pin GPIO_PIN_3
#define DB3_GPIO_Port GPIOB
#define DB4_Pin GPIO_PIN_4
#define DB4_GPIO_Port GPIOB
#define DB5_Pin GPIO_PIN_5
#define DB5_GPIO_Port GPIOB
#define DB6_Pin GPIO_PIN_6
#define DB6_GPIO_Port GPIOB
#define DB7_Pin GPIO_PIN_7
#define DB7_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
