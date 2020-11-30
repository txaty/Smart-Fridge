#include "pwm_control.h"
#include "adc.h"

uint16_t temp_pwm_value = 120;
uint16_t lcd_pwm_value = 120;
uint16_t target_temp = 10;

uint32_t lcd_adc_sample_list[5] = {0};
uint32_t lcd_adc_average = 0;
uint8_t lcd_adc_index = 0;

uint8_t lcd_pwm_set_value(uint16_t value)
{
    if (value >= LCD_MIN_PWM_PULSE && value <= LCD_MAX_PWM_PULSE)
    {
        TIM_OC_InitTypeDef timOcConf;

        timOcConf.OCMode = TIM_OCMODE_PWM1;
        timOcConf.Pulse = value;
        timOcConf.OCPolarity = TIM_OCPOLARITY_HIGH;
        timOcConf.OCFastMode = TIM_OCFAST_DISABLE;
        HAL_TIM_PWM_ConfigChannel(LCD_PWM_TIM, &timOcConf, LCD_PWM_CHANNEL);
        HAL_TIM_PWM_Start(LCD_PWM_TIM, LCD_PWM_CHANNEL);

        return 0;
    }
    return 1;
}

void lcd_adc_read(void)
{
    if (lcd_adc_index == 5) {
        lcd_adc_index = 0;
    }
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    uint32_t adc_value = HAL_ADC_GetValue(&hadc1);
    lcd_adc_sample_list[lcd_adc_index] = adc_value;
    lcd_adc_index ++;
}