#ifndef _SPI_H_
#define _SPI_H_

extern uint8_t spi_last_read;

void spi_init(void);
void spi_radio_select(int on);

int spi_write(const uint8_t* data, int len, int (*done)(void));
void spi_write_single(uint8_t data);
int spi_read(uint8_t* data, int len, int (*done)(void));
uint8_t spi_read_single(void);

uint8_t spi_read_somi(void);

#endif
