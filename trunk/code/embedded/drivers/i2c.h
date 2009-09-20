#ifndef _I2C_H_
#define _I2C_H_

void i2c_init(void);

int16_t i2c_write(uint8_t addr, const uint8_t* data, int16_t len);
int16_t i2c_write_read(uint8_t addr, uint8_t data_w, uint8_t* data_r, int16_t len);
int16_t i2c_read (uint8_t addr, uint8_t* data, int16_t len);

#endif

