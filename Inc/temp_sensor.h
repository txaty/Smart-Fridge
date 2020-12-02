#ifndef _TEMP_SENSOR_H
#define _TEMP_SENSOR_H

#define DS18B20_PORT GPIOE
#define DS18B20_PIN GPIO_PIN_6

#include "stdint.h"
#include "gpio.h"

uint8_t DS18B20_Start(void);
void DS18B20_Write(uint8_t data);
uint8_t DS18B20_Read(void);
int DS18B20_GetCelsiusTemp(void);

#endif