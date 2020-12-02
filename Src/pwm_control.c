#include "pwm_control.h"
#include "adc.h"

uint16_t temp_pwm_value = 120;
uint16_t lcd_pwm_value = 120;
uint16_t target_temp = 10;

uint32_t lcd_adc_sample_list[5] = {0};
uint32_t lcd_adc_average = 0;
uint8_t lcd_adc_index = 0;

float temp_kp = 3;
float temp_ki = 0.1;
float temp_kd = 1;

float last_error = 0;
float curr_error = 0;
float accumulation = 0;

int fridge_temp = 0;
int temp_pwm = 10;

float max_accumulation = 15.0;
float min_accumulation = -15.0;

uint8_t flag_enable_cooling = 1;

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
    if (lcd_adc_index == 5)
    {
        lcd_adc_index = 0;
    }
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000);
    uint32_t adc_value = HAL_ADC_GetValue(&hadc1);
    lcd_adc_sample_list[lcd_adc_index] = adc_value;
    lcd_adc_index++;
}

float temp_get_pid(void)
{
    float pid_result;

    curr_error = target_temp - fridge_temp;
    float temp_p = temp_kp * curr_error;
    accumulation += curr_error;
    if (accumulation < min_accumulation)
    {
        accumulation = min_accumulation;
    }
    else if (accumulation > max_accumulation)
    {
        accumulation = max_accumulation;
    }
    float temp_i = temp_ki * accumulation;
    float temp_d = temp_kd * (curr_error - last_error);
    last_error = curr_error;
    pid_result = (int)(temp_p + temp_i + temp_d);

    return pid_result;
}

void temp_pwm_set_value(int pid_value)
{
    if (flag_enable_cooling)
    {
        temp_pwm -= pid_value;
        if (temp_pwm < TEMP_MIN_PWM_PULSE)
        {
            temp_pwm = TEMP_MIN_PWM_PULSE;
        }
        else if (temp_pwm > TEMP_MAX_PWM_PULSE)
        {
            temp_pwm = TEMP_MAX_PWM_PULSE;
        }
    }
    else
    {
        temp_pwm = 0;
    }

    TIM_OC_InitTypeDef timOcConf;

    timOcConf.OCMode = TIM_OCMODE_PWM1;
    timOcConf.Pulse = temp_pwm;
    timOcConf.OCPolarity = TIM_OCPOLARITY_HIGH;
    timOcConf.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(TEMP_PWM_TIM, &timOcConf, TEMP_PWM_CHANNEL);
    HAL_TIM_PWM_Start(TEMP_PWM_TIM, TEMP_PWM_CHANNEL);
}