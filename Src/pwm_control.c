#include "pwm_control.h"

volatile uint16_t pwm_value = 16;

uint8_t PWM_SetValue(uint16_t value)
{
    if (value >= MIN_PWM_PULSE && value <= MAX_PWM_PULSE)
    {
        TIM_OC_InitTypeDef timOcConf;

        timOcConf.OCMode = TIM_OCMODE_PWM1;
        timOcConf.Pulse = value;
        timOcConf.OCPolarity = TIM_OCPOLARITY_HIGH;
        timOcConf.OCFastMode = TIM_OCFAST_DISABLE;
        HAL_TIM_PWM_ConfigChannel(PWM_TIM, &timOcConf, PWM_CHANNEL);
        HAL_TIM_PWM_Start(PWM_TIM, PWM_CHANNEL);

        return 0;
    }
    return 1;
}