#ifndef _PWM_H_
#define _PWM_H_

#define PWM_SOURCE_SMCLK 1
#define PWM_SOURCE_ACLK 2

void pwm_period_set(int source, int div, uint16_t period);
void pwm_out1_set(uint16_t high_time);
void pwm_out2_set(uint16_t high_time);
void pwm_stop();

#endif
