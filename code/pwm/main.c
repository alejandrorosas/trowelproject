#include <io.h>
#include "clock.h"
#include "pwm.h"
#include "leds.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  
  clock_dco_set(1); // DCO @ 1MHz
  clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK @ 1MHz
  clock_smclk_set(CLOCK_SOURCE_DCO, 1); // SMCLK @ 125kHz
  clock_aclk_set(8); // ACLK @ 1500Hz
  
  pwm_period_set(PWM_SOURCE_SMCLK, 1, 15625); // timer @ 187.5Hz, PWM period = 1s
  pwm_out1_set(5625);
  pwm_out2_set(10625);
  
  LPM0;
  
}

