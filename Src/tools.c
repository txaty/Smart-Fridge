#include "tools.h"
#include "tim.h"

void delay_us(uint16_t time)
{
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    while ((__HAL_TIM_GET_COUNTER(&htim6)) < time)
        ;
}

void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

GPIO_PinState read_input_data_bit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIO_PinState temp = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
    GPIO_InitTypeDef GPIO_InitStruct1 = {0};
    GPIO_InitStruct1.Pin = GPIO_Pin;
    GPIO_InitStruct1.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct1.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct1);

    GPIO_PinState result = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

    GPIO_InitTypeDef GPIO_InitStruct2 = {0};
    GPIO_InitStruct2.Pin = GPIO_Pin;
    GPIO_InitStruct2.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct2.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct2);
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, temp);

    return result;

}