#ifndef _CLOCK_H_
#define _CLOCK_H_

#define CLOCK_SOURCE_DCO 0x1
#define CLOCK_SOURCE_VLO 0x2

void clock_dco_set(uint16_t freq);
void clock_mclk_set(int source, int div);
void clock_smclk_set(int source, int div);
void clock_aclk_set(int div);

#endif
