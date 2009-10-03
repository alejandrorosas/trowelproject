#ifndef _LIS302_H
#define _LIS302_H

int16_t lis302_init(void);
void lis302_reinit(void);

int8_t lis302_getx(void);
int8_t lis302_gety(void);
int8_t lis302_getz(void);

#endif
