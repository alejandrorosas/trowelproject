#ifndef _SPI_H_
#define _SPI_H_

#define SPI_RADIO 0x1
#define SPI_ACCEL 0x2

extern uint8_t spi_last_read;
extern int spi_last_config;

void spi_init(void);
#define spi_radio_select()    do { \
  spi_accel_deselect(); \
  if (spi_last_config != SPI_RADIO) { \
    spi_last_config = SPI_RADIO; \
    UCB0CTL1 = UCSSEL_SMCLK | UCSWRST; \
    UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC; \
    UCB0CTL1 &= ~UCSWRST; \
  } \
  P3OUT &= ~1; \
} while (0)

#define spi_radio_deselect()  P3OUT |= 1
#define spi_accel_select()    do { \
  spi_radio_deselect(); \
  if (spi_last_config != SPI_ACCEL) { \
    spi_last_config = SPI_ACCEL; \
    UCB0CTL1 = UCSSEL_SMCLK | UCSWRST; \
    UCB0CTL0 = UCCKPL | UCMSB | UCMST | UCSYNC; \
    UCB0CTL1 &= ~UCSWRST; \
  } \
  P4OUT &= ~BV(6); \
} while (0)

#define spi_accel_deselect()  P4OUT |= BV(6)

int spi_write(const uint8_t* data, int len);
void spi_write_single(uint8_t data);
int spi_read(uint8_t* data, int len);
uint8_t spi_read_single(void);

#define spi_read_somi() P3IN & (1<<2)

#endif
