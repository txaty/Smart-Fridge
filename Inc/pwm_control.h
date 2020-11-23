#ifndef _PWM_CONTROL_H
#define _PWM_CONTROL_H

#include "tim.h"

#define PWM_TIM &htim4
#define PWM_CHANNEL TIM_CHANNEL_1

#define MAX_PWM_PULSE 63
#define MIN_PWM_PULSE 0

extern volatile uint16_t pwm_value;

uint8_t PWM_SetValue(uint16_t value);

#endif