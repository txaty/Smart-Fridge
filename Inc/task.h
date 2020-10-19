#ifndef _TASK_H_
#define _TASK_H_

#include "cmsis_os.h"
#include "tos_k.h"

#define TASK1_STK_SIZE 512
#define TASK2_STK_SIZE 512
    
void lv_task(void *pdata);

#endif