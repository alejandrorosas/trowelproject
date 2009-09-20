#ifndef _HMC5843_H
#define _HMC5843_H

/**
 * \return 0 if ok, <0 if error
 */
int16_t hmc5843_init(void);

/**
 * \return 0 if ok, <0 if error
 */
int16_t hmc5843_getXYZ(int16_t* x, int16_t* y, int16_t* z);

#endif
