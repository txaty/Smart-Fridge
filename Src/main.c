/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "fatfs.h"
#include "rtc.h"
#include "sdio.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "debug.h"
#include "task.h"
#include "esp8266.h"
#include "sal_module_wrapper.h"
#include "tos_at.h"
#include "stm32f1xx_it.h"
#include "file_handling.h"
#include "temp_sensor.h"
#include "lcd_tft.h"
#include "bsp_ov7725.h"
#include "pwm_control.h"
#include "sntp.h"
#include "display_content.h"

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
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

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
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_TIM6_Init();
  MX_TIM4_Init();
  MX_RTC_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim6);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_ADCEx_Calibration_Start(&hadc1);

  LCD_Init();
  lv_init();
  XPT2046_Init();
  lv_disp_buf_t disp_buf;
  lv_color_t buf[LV_HOR_RES_MAX * LV_VER_RES_MAX / 10];                         /*Declare a buffer for 1/10 screen size*/
  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * LV_VER_RES_MAX / 10); /*Initialize the display buffer*/

  lv_disp_drv_t disp_drv;      /*Descriptor of a display driver*/
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/

  disp_drv.flush_cb = my_disp_flush; /*Set your driver function*/
  disp_drv.buffer = &disp_buf;       /*Assign the buffer to the display*/
  lv_disp_drv_register(&disp_drv);   /*Finally register the driver*/

  lv_indev_drv_t indev_drv;               /*Descriptor of a input device driver*/
  lv_indev_drv_init(&indev_drv);          /*Basic initialization*/
  indev_drv.type = LV_INDEV_TYPE_POINTER; /*Touch pad is a pointer-like device*/
  indev_drv.read_cb = my_touchpad_read;   /*Set your driver function*/
  lv_indev_drv_register(&indev_drv);      /*Finally register the driver*/

  tos_knl_init();
  // Mutex creation
  tos_mutex_create(&pb8_pb9_mutex);
  tos_mutex_create(&display_touch_locker);
  tos_mutex_create(&rtc_update_locker);
  tos_mutex_create(&temp_update_locker);
  // Completion creation
  tos_completion_create(&wifi_connect_success);
  tos_completion_create(&sntp_success);
  tos_task_create_dyn(&k_init_image, "init_image", task_init_image, NULL,
                      3, INIT_IMAGE_SIZE, 0);
  tos_task_create_dyn(&k_wifi_connect, "wifi_connect", task_wifi_connect, NULL,
                      4, WIFI_TEST_CONNECT_SIZE, 0);
  tos_task_create(&k_console_printf_debug, "console_printf_debug", task_console_printf_debug, NULL,
                  7, k_console_printf_debug_stk, CONSOLE_PRINTF_DEBUG_SIZE, 0);
  tos_task_create(&k_rtc_update, "rtc_update", task_rtc_update, NULL,
                  4, k_rtc_update_stk, RTC_UPDATE_SIZE, 0);
  tos_task_create(&k_temp_update, "temp_update", task_temp_update, NULL,
                  2, k_temp_update_stk, TEMP_UPDATE_SIZE, 0);
  tos_knl_start();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint16_t lcd_timer_tick = 0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    lv_tick_inc(1);

    lcd_timer_tick++;
    if (lcd_timer_tick >= 1000)
    {
      lcd_adc_read();
      printf("value read\r\n");
      if (lcd_adc_index == 10)
      {
        lcd_adc_index = 0;
        lcd_adc_average = 0;
        int i = 0;
        for (int i = 0; i < 10; ++i)
        {
          lcd_adc_average += lcd_adc_sample_list[i];
        }
        lcd_adc_average /= 10;
        printf("ADC avg: %d\r\n", lcd_adc_average);
        int lcd_pwm_result = (int)(LCD_ADC_2_PWM_K*lcd_adc_average + LCD_ADC_2_PWM_B);
        if (lcd_pwm_value > LCD_MAX_PWM_PULSE) {
          lcd_pwm_result = LCD_MAX_PWM_PULSE;
        } else if (lcd_pwm_result < LCD_MIN_PWM_PULSE) {
          lcd_pwm_result = LCD_MIN_PWM_PULSE;
        }
        printf("PWM result: %d\r\n", lcd_pwm_result);
        lcd_pwm_set_value(lcd_pwm_result);
      }

      lcd_timer_tick = 0;
    }
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
