#ifndef _TASK_H_
#define _TASK_H_

#include "cmsis_os.h"
#include "tos_k.h"


// LCD display and touch screen
#define LED_TASK_STK_SIZE 256
#define LVGL_TASK_STK_SIZE 1024
extern k_mutex_t display_touch_locker;

void display_touch_task(void *pdata);

// Wifi connection test
#define WIFI_TASK_SIZE 512
extern k_task_t k_task_wifi;
extern uint8_t k_wifi_stk[WIFI_TASK_SIZE];
extern int socket_id_0;

void task_wifi(void *pdata);

// SDIO test
#define SDIO_TASK_SIZE 512
extern k_task_t k_task_sdio;
extern uint8_t k_sdio_stk[SDIO_TASK_SIZE];

void task_sdio(void *pdata);

#endif