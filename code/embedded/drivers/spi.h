#ifndef _SPI_H_
#define _SPI_H_

typedef int (*spi_transfer_done_cb_t)(); 

extern uint8_t spi_last_read;

void spi_init(void);
void spi_select_radio();
void spi_deselect_radio();

int spi_write(const uint8_t* data, int len);
void spi_write_single(uint8_t data);
int spi_write_interrupt(const uint8_t* data, int len, spi_transfer_done_cb_t cb);

int spi_read(uint8_t* data, int len);
uint8_t spi_read_single(void);
int spi_read_interrupt(uint8_t* data, int len, spi_transfer_done_cb_t cb);

#define SPI_SOMI (P3IN & BV(2))

#endif
