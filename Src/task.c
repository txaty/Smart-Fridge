#include "task.h"
#include "gpio.h"

void redLedBlink(void *pdata)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        osDelay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        osDelay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        osDelay(500);
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    }
}