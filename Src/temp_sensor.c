#include "tim.h"
#include "gpio.h"
#include "debug.h"
#include "temp_sensor.h"

void delay_us(uint16_t time)
{
    __HAL_TIM_SET_COUNTER(&htim6, 0);
    htim6.Instance->CNT = 0;
    printf("start counting\r\n");
    // while ((__HAL_TIM_GET_COUNTER(&htim6)) < time)
    //     ;
    // while ();
    printf("finished counting\r\n");
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
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

uint8_t DS18B20_Start(void)
{
    uint8_t Response = 0;
    set_pin_output(DS18B20_PORT, DS18B20_PIN);       // set the pin as output
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0); // pull the pin low
    printf("Finished phase 1\r\n");
    delay_us(480);                                   // delay according to datasheet
    // HAL_Delay(480);
    
    set_pin_input(DS18B20_PORT, DS18B20_PIN); // set the pin as input
    delay_us(80);                             // delay according to datasheet
    // HAL_Delay(80);

    if (!(HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)))
        Response = 1; // if the pin is low i.e the presence pulse is detected
    else
        Response = -1;

    delay_us(400); // 480 us delay totally.
    // HAL_Delay(400);

    return Response;
}

void DS18B20_Write(uint8_t data)
{
    set_pin_output(DS18B20_PORT, DS18B20_PIN); // set as output

    for (int i = 0; i < 8; i++)
    {

        if ((data & (1 << i)) != 0) // if the bit is high
        {
            // write 1

            set_pin_output(DS18B20_PORT, DS18B20_PIN);       // set as output
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0); // pull the pin LOW
            // delay_us(1);                                     // wait for 1 us
            HAL_Delay(1);

            set_pin_input(DS18B20_PORT, DS18B20_PIN); // set as input
            // delay_us(50);                             // wait for 60 us
            HAL_Delay(50);
        }

        else // if the bit is low
        {
            // write 0

            set_pin_output(DS18B20_PORT, DS18B20_PIN);
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, 0); // pull the pin LOW
            // delay_us(50);                                    // wait for 60 us
            HAL_Delay(50);

            set_pin_input(DS18B20_PORT, DS18B20_PIN);
        }
    }
}

uint8_t DS18B20_Read(void)
{
    uint8_t value = 0;
    set_pin_input(DS18B20_PORT, DS18B20_PIN);

    for (int i = 0; i < 8; i++)
    {
        set_pin_output(DS18B20_PORT, DS18B20_PIN); // set as output

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0); // pull the data pin LOW
        // delay_us(2);                                // wait for 2 us
        HAL_Delay(2);

        set_pin_input(DS18B20_PORT, DS18B20_PIN); // set as input
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))  // if the pin is HIGH
        {
            value |= 1 << i; // read = 1
        }
        // delay_us(60); // wait for 60 us
        HAL_Delay(60);
    }
    return value;
}