#ifndef _TASK_H_
#define _TASK_H_

#define TASK1_STK_SIZE 512

#include "cmsis_os.h"
#include "usart.h"

void redLedBlink(void *pdata);

#if defined(__CC_ARM) || defined(__ICCARM__)

int fputc(int ch, FILE *f);

#elif defined (__GNUC__)

int _write(int fd, char *ptr, int len);

#endif

void redLedBlink(void *pdata);

#endif