#include "task.h"
#include "gpio.h"
#include "lvgl.h"
#include "esp8266.h"
#include "tos_at.h"
#include "sal_module_wrapper.h"

// LCD display and touch screen
k_mutex_t display_touch_locker;

void display_touch_task(void *pdata)
{
  k_err_t err;
  while (K_TRUE)
  {
    err = tos_mutex_pend(&display_touch_locker);
    if (err == K_ERR_NONE)
    {
      lv_task_handler();
      lv_tick_inc(5);
      tos_mutex_post(&display_touch_locker);
    }
    osDelay(1);
  }
}

// Wifi connection test
k_task_t k_task_wifi;
uint8_t k_wifi_stk[WIFI_TASK_SIZE];
int socket_id_0 = -1;

void task_wifi(void *pdata)
{
  // Configure Pin PB8, PB9
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);

  if (esp8266_sal_init(HAL_UART_PORT_3) == 0)
  {
    if (esp8266_join_ap("ASD", "qwertyuiop") != 0)
    {
      printf("AP joining failed\n");
    }
    else
    {
      printf("AP joning success\n");
    }
  }
  socket_id_0 = tos_sal_module_connect("108.160.133.235", "80", TOS_SAL_PROTO_TCP);
  if (socket_id_0 == -1)
  {
    printf("TCP0 connect failed\r\n");
  }
  else
  {
    printf("TCP0 connect success! fd: %d\n", socket_id_0);
  }
}

//SDIO test
k_task_t k_task_sdio;
uint8_t k_sdio_stk[SDIO_TASK_SIZE];

void task_sdio(void *pdata)
{
  char buffer[200];
  int indx = 0;
  Mount_SD("/");
  Format_SD();
  Unmount_SD("/");
  for (int i = 0; i < 10; ++i)
  {
    Mount_SD("/");
    sprintf(buffer, "Hello ---> %d\n", indx);
    Update_File("FILE1.TXT", buffer);
    sprintf(buffer, "world ---> %d\n", indx);
    Update_File("FILE2.TXT", buffer);
    Unmount_SD("/");

    indx++;
    printf("Debug %d\n", indx);
    osDelay(2000);
  }
}