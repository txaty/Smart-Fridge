#ifndef _TASK_H_
#define _TASK_H_

#include "cmsis_os.h"
#include "tos_k.h"


// LCD display and touch screen
#define LED_TASK_STK_SIZE 256
#define LVGL_TASK_STK_SIZE 2048
extern k_mutex_t display_touch_locker;

void display_touch_task(void *pdata);

// Wifi connection test
#define WIFI_TASK_SIZE 2048
extern k_task_t k_task_wifi;
extern uint8_t k_wifi_stk[WIFI_TASK_SIZE];
extern int socket_id_0;

void task_wifi(void *pdata);


#endif