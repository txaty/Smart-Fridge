#ifndef _TASK_H_
#define _TASK_H_

#include "cmsis_os.h"
#include "tos_k.h"

#define LED_TASK_STK_SIZE 256
#define LVGL_TASK_STK_SIZE 2048

extern k_mutex_t display_touch_locker;

void display_touch_task(void *pdata);


#endif