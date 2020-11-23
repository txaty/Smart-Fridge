#ifndef __SCCB_H
#define __SCCB_H

#include "stm32f1xx.h"
#include "gpio.h"
#include "tools.h"

#define SCL_H         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET) //GPIO_SetBits(GPIOC , GPIO_PIN_6)
#define SCL_L         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET) //GPIO_ResetBits(GPIOC , GPIO_PIN_6) 
   
#define SDA_H         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET) //GPIO_SetBits(GPIOC , GPIO_PIN_7) 
#define SDA_L         HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET) //GPIO_ResetBits(GPIOC , GPIO_PIN_7) 

#define SCL_read      read_input_data_bit(GPIOC, GPIO_PIN_6) //HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) //GPIO_ReadInputDataBit(GPIOC , GPIO_PIN_6) 
#define SDA_read      read_input_data_bit(GPIOC, GPIO_PIN_7) //HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_7) //GPIO_ReadInputDataBit(GPIOC , GPIO_PIN_7) 

#define ADDR_OV7725   0x42

void SCCB_GPIO_Config(void);
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte);
int SCCB_ReadByte(uint8_t* pBuffer,   uint16_t length,   uint8_t ReadAddress);

#endif 
