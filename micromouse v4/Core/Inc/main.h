/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h5xx_hal.h"

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
#define ADC_BAT_Pin GPIO_PIN_0
#define ADC_BAT_GPIO_Port GPIOC
#define SOA_L_Pin GPIO_PIN_1
#define SOA_L_GPIO_Port GPIOC
#define SOB_L_Pin GPIO_PIN_2
#define SOB_L_GPIO_Port GPIOC
#define SOC_L_Pin GPIO_PIN_3
#define SOC_L_GPIO_Port GPIOC
#define SOA_R_Pin GPIO_PIN_0
#define SOA_R_GPIO_Port GPIOA
#define SOB_R_Pin GPIO_PIN_1
#define SOB_R_GPIO_Port GPIOA
#define SOC_R_Pin GPIO_PIN_2
#define SOC_R_GPIO_Port GPIOA
#define FAN_PWM_Pin GPIO_PIN_5
#define FAN_PWM_GPIO_Port GPIOA
#define ENC_R1_Pin GPIO_PIN_6
#define ENC_R1_GPIO_Port GPIOA
#define ENC_R2_Pin GPIO_PIN_7
#define ENC_R2_GPIO_Port GPIOA
#define MTR_CS_R_Pin GPIO_PIN_4
#define MTR_CS_R_GPIO_Port GPIOC
#define MTR_CS_L_Pin GPIO_PIN_5
#define MTR_CS_L_GPIO_Port GPIOC
#define MTR_MISO_Pin GPIO_PIN_0
#define MTR_MISO_GPIO_Port GPIOB
#define MTR_SCK_Pin GPIO_PIN_1
#define MTR_SCK_GPIO_Port GPIOB
#define MTR_MOSI_Pin GPIO_PIN_2
#define MTR_MOSI_GPIO_Port GPIOB
#define SNR_SCL_Pin GPIO_PIN_10
#define SNR_SCL_GPIO_Port GPIOB
#define SNR_SDA_Pin GPIO_PIN_12
#define SNR_SDA_GPIO_Port GPIOB
#define IMU_SCK_Pin GPIO_PIN_13
#define IMU_SCK_GPIO_Port GPIOB
#define IMU_MISO_Pin GPIO_PIN_14
#define IMU_MISO_GPIO_Port GPIOB
#define IMU_MOSI_Pin GPIO_PIN_15
#define IMU_MOSI_GPIO_Port GPIOB
#define MTR_U_R_Pin GPIO_PIN_6
#define MTR_U_R_GPIO_Port GPIOC
#define MTR_V_R_Pin GPIO_PIN_7
#define MTR_V_R_GPIO_Port GPIOC
#define MTR_W_R_Pin GPIO_PIN_8
#define MTR_W_R_GPIO_Port GPIOC
#define IMU_CS_Pin GPIO_PIN_9
#define IMU_CS_GPIO_Port GPIOC
#define MTR_U_L_Pin GPIO_PIN_8
#define MTR_U_L_GPIO_Port GPIOA
#define MTR_V_L_Pin GPIO_PIN_9
#define MTR_V_L_GPIO_Port GPIOA
#define MTR_W_L_Pin GPIO_PIN_10
#define MTR_W_L_GPIO_Port GPIOA
#define SNR_MUX_Pin GPIO_PIN_11
#define SNR_MUX_GPIO_Port GPIOA
#define SNR3_Pin GPIO_PIN_12
#define SNR3_GPIO_Port GPIOA
#define SNR2_Pin GPIO_PIN_15
#define SNR2_GPIO_Port GPIOA
#define SNR1_Pin GPIO_PIN_10
#define SNR1_GPIO_Port GPIOC
#define SNR0_Pin GPIO_PIN_11
#define SNR0_GPIO_Port GPIOC
#define LCD_BK_Pin GPIO_PIN_12
#define LCD_BK_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_2
#define LCD_CS_GPIO_Port GPIOD
#define LCD_SCK_Pin GPIO_PIN_3
#define LCD_SCK_GPIO_Port GPIOB
#define LCD_DC_Pin GPIO_PIN_4
#define LCD_DC_GPIO_Port GPIOB
#define LCD_MOSI_Pin GPIO_PIN_5
#define LCD_MOSI_GPIO_Port GPIOB
#define ENC_L1_Pin GPIO_PIN_6
#define ENC_L1_GPIO_Port GPIOB
#define ENC_L2_Pin GPIO_PIN_7
#define ENC_L2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
