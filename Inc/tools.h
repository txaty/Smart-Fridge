#ifndef _TOOLS_H
#define _TOOLS_H

#include "stdint.h"
#include "gpio.h"

void delay_us(uint16_t time);
void set_pin_output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void set_pin_input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif