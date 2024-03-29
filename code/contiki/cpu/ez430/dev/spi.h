#ifndef _SPI_H_
#define _SPI_H_

extern uint8_t spi_last_read;

void spi_init(void);
#define spi_radio_select()P3OUT &= ~1
#define spi_radio_deselect()  P3OUT |= 1

int spi_write(const uint8_t* data, int len);
void spi_write_single(uint8_t data);
int spi_read(uint8_t* data, int len);
uint8_t spi_read_single(void);

#define spi_read_somi() P3IN & (1<<2)

#endif
