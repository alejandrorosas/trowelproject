#ifndef _ADC_H_
#define _ADC_H_
typedef int (*adc_cb_t)(uint16_t value);

void adc_init(void);
void adc_start(void);
void adc_stop(void);
void adc_register_callback(adc_cb_t cb);

#endif
