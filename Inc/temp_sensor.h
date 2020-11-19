#ifndef _TEMP_SENSOR_H
#define _TEMP_SENSOR_H

#define DS18B20_PORT GPIOC
#define DS18B20_PIN GPIO_PIN_1

#include "stdint.h"

uint8_t DS18B20_Start(void);
void DS18B20_Write(uint8_t data);
uint8_t DS18B20_Read(void);

#endif