#ifndef __SCCB_H
#define __SCCB_H

#include "stm32f1xx.h"
#include "gpio.h"
#include "tools.h"

#define SCL_H         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET)
#define SCL_L         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET)
   
#define SDA_H         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET)
#define SDA_L         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET)

#define SCL_read      GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_6)
#define SDA_read      GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_7)

#define ADDR_OV7725   0x42

void SCCB_GPIO_Config(void);
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte);
int SCCB_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint8_t ReadAddress);

#endif 
