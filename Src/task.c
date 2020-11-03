#include "task.h"
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
        if (err == K_ERR_NONE) {
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
    if (socket_id_0 == -1) {
        printf("TCP0 connect failed\r\n");
    } else {
        printf("TCP0 connect success! fd: %d\n", socket_id_0);
    }
}