#include "cmsis_os.h"
#include "debug.h"
#include "usart.h"

#if defined(__CC_ARM) || defined(__ICCARM__)

int fputc(int ch, FILE *f)
{
    if (ch == '\n')
    {
        HAL_UART_Transmit(&huart1, (void *)"\r", 1, 30000);
    }
    HAL_UART_Transmit(&huart1, (unit8_t *)&ch, 1, 0xFFFF);
    return ch;
}

#elif defined(__GNUC__)

int _write(int fd, char *ptr, int len)
{
    (void)HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF);
    return len;
}

#endif