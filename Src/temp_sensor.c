#include "tim.h"
#include "gpio.h"
#include "debug.h"
#include "stdbool.h"
#include "temp_sensor.h"
#include "tools.h"

uint8_t DS18B20_Start(void)
{
    uint8_t Response = 0;
    set_pin_output(DS18B20_PORT, DS18B20_PIN);                    // set the pin as output
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the pin low

    delay_us(480); // delay according to datasheet

    set_pin_input(DS18B20_PORT, DS18B20_PIN); // set the pin as input
    delay_us(80);                             // delay according to datasheet

    if (!(HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)))
        Response = 1; // if the pin is low i.e the presence pulse is detected
    else
        Response = -1;

    delay_us(400); // 480 us delay totally.

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

            set_pin_output(DS18B20_PORT, DS18B20_PIN);                    // set as output
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the pin LOW
            delay_us(1);                                                  // wait for 1 us

            set_pin_input(DS18B20_PORT, DS18B20_PIN); // set as input
            delay_us(50);                             // wait for 60 us
        }

        else // if the bit is low
        {
            // write 0

            set_pin_output(DS18B20_PORT, DS18B20_PIN);
            HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the pin LOW
            delay_us(50);                                                 // wait for 60 us

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

        HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the data pin LOW
        delay_us(2);                                                  // wait for 2 us

        set_pin_input(DS18B20_PORT, DS18B20_PIN);        // set as input
        if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)) // if the pin is HIGH
        {
            value |= 1 << i; // read = 1
        }
        delay_us(60); // wait for 60 us
    }
    return value;
}

int DS18B20_GetCelsiusTemp(void)
{
    uint8_t lsb;
    uint8_t msb;
    bool temp_sign;
    uint16_t temp_int;
    int temp_decimal;
    // float temp_result;

    DS18B20_Start();
    HAL_Delay(1);
    DS18B20_Write(0xCC); // skip ROM
    DS18B20_Write(0x44); // convert t
    HAL_Delay(800);

    DS18B20_Start();
    HAL_Delay(1);
    DS18B20_Write(0xCC); // skip ROM
    DS18B20_Write(0xBE); // Read Scratch-pad

    lsb = DS18B20_Read();
    msb = DS18B20_Read();

    // if (msb > 7)
    // {
    //     msb = ~msb;
    //     lsb = ~msb;
    //     temp_sign = false;
    // }
    // else
    // {
    //     temp_sign = true;
    // }
    // temp_int = msb;
    // temp_int <<= 8;
    // temp_int += lsb;
    // if (!temp_sign)
    // {
    //     temp_int = -temp_int;
    // }
    temp_int = ((uint16_t)msb << 8) | lsb;
    int negative;
    negative = (temp_int & (1 << 15)) != 0;
    if (negative)
    {
        temp_decimal = temp_int | ~((1 << 16) - 1);
    }
    else
    {
        temp_decimal = temp_int;
    }
    // temp_result = temp_decimal / 16;
    temp_decimal /= 16;
    return temp_decimal;
}