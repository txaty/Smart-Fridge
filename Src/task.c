#include "task.h"
#include "lvgl.h"

void lv_task(void *pdata)
{
    while (1)
    {
        printf('Hello\n');
        osDelay(1000);
    }
}