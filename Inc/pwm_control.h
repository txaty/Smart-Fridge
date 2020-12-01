#ifndef _PWM_CONTROL_H
#define _PWM_CONTROL_H

#include "tim.h"


#define LCD_PWM_TIM &htim4
#define LCD_PWM_CHANNEL TIM_CHANNEL_1

#define LCD_MAX_PWM_PULSE 127
#define LCD_MIN_PWM_PULSE 0

#define TEMP_MAX_PWM_PULSE 63
#define TEMP_MIN_PWM_PULSE 0

#define LCD_ADC_2_PWM_K 0.08f
#define LCD_ADC_2_PWM_B -160

#define TARGET_TEMP_MAX 30
#define TARGET_TEMP_MIN -10

extern uint16_t temp_pwm_value;
extern uint16_t lcd_pwm_value;
extern uint16_t target_temp;

extern uint32_t lcd_adc_sample_list[5];
extern uint32_t lcd_adc_average;
extern uint8_t lcd_adc_index;

uint8_t lcd_pwm_set_value(uint16_t value);
void lcd_adc_read(void);

#endif