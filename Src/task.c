#include "task.h"
#include "lvgl.h"

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