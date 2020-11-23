/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal.h"

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
#define Temp_Sensor_IO_Pin GPIO_PIN_6
#define Temp_Sensor_IO_GPIO_Port GPIOE
#define Cam_VSYNC_Pin GPIO_PIN_3
#define Cam_VSYNC_GPIO_Port GPIOC
#define Cam_VSYNC_EXTI_IRQn EXTI3_IRQn
#define Cam_FIFO_RRST_Pin GPIO_PIN_2
#define Cam_FIFO_RRST_GPIO_Port GPIOA
#define Cam_FIFO_OE_Pin GPIO_PIN_3
#define Cam_FIFO_OE_GPIO_Port GPIOA
#define Cam_FIFO_WRST_Pin GPIO_PIN_4
#define Cam_FIFO_WRST_GPIO_Port GPIOC
#define Cam_FIFO_RCLK_Pin GPIO_PIN_5
#define Cam_FIFO_RCLK_GPIO_Port GPIOC
#define Cam_Data4_Pin GPIO_PIN_12
#define Cam_Data4_GPIO_Port GPIOB
#define Cam_Data5_Pin GPIO_PIN_13
#define Cam_Data5_GPIO_Port GPIOB
#define Cam_Data6_Pin GPIO_PIN_14
#define Cam_Data6_GPIO_Port GPIOB
#define Cam_Data7_Pin GPIO_PIN_15
#define Cam_Data7_GPIO_Port GPIOB
#define Cam_SCL_Pin GPIO_PIN_6
#define Cam_SCL_GPIO_Port GPIOC
#define Cam_SDA_Pin GPIO_PIN_7
#define Cam_SDA_GPIO_Port GPIOC
#define Cam_FIFO_WEN_Pin GPIO_PIN_3
#define Cam_FIFO_WEN_GPIO_Port GPIOD
#define Cam_Data0_Pin GPIO_PIN_8
#define Cam_Data0_GPIO_Port GPIOB
#define Cam_Data1_Pin GPIO_PIN_9
#define Cam_Data1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
