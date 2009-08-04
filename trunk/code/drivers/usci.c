#include <io.h>
#include <signal.h>

#include "spi.h"
#include "uart.h"

#ifndef NULL
  #define NULL 0x0
#endif

uint8_t spi_last_read;

void spi_init(void) {
  
  /* configure all SPI related pins */
  P3DIR |= (1<<1) | (1<<3);
  P3DIR &= ~(1<<2);

  // configure P3.0 IO for radio CSn
  P3SEL &= ~1; // set IO
  P3DIR |= 1; // set output
  spi_radio_deselect();
  
  /* initialize the SPI registers */
  UCB0CTL1 = UCSWRST;
  UCB0CTL1 = UCSSEL_SMCLK | UCSWRST;
  UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;
  UCB0BR0  = 0x2;
  UCB0BR1  = 0x0;
  
  P3SEL |= (1<<1) | (1<<2) | (1<<3);
  
  UCB0CTL1 &= ~UCSWRST;
  
  // disable interrupts
  IE2 &= ~(UCB0TXIE | UCB0RXIE);
}

int spi_write(const uint8_t* data, int len) {
  int i;
  uint8_t dummy;
  
  if ( UCB0STAT & UCBUSY )
      return 0;

  for (i=0; i<len; i++) {
      UCB0TXBUF = data[i];
      while (!(IFG2 & UCB0RXIFG)) ;
      dummy = UCB0RXBUF;
  }

  while (UCB0STAT & UCBUSY);

  return i;
}

void spi_write_single(uint8_t data) {
    
    if ( UCB0STAT & UCBUSY )
        return;
    
    UCB0TXBUF = data;
    
    // wait for RX ready
    while (!(IFG2 & UCB0RXIFG));
    
    // store RX data
    spi_last_read = UCB0RXBUF;
    
    while (UCB0STAT & UCBUSY);
}

int spi_read(uint8_t* data, int len) {
    int i;
    
    if (UCB0STAT & UCBUSY)
        return 0;
    
    for (i=0; i<len; i++) {
        // wait for TX ready
        while (!(IFG2 & UCB0TXIFG));
        // start sending
        UCB0TXBUF = 0;
        // wait for RX ready
        while (!(IFG2 & UCB0RXIFG));
        // store RX data
        data[i] = UCB0RXBUF;
    }
        
        return i;
}

uint8_t spi_read_single(void) {
    
    if (UCB0STAT & UCBUSY)
        return 0;
    
    UCB0TXBUF = 0;
    // wait for RX ready
    while (!(IFG2 & UCB0RXIFG));
    
    // return RX data
    return UCB0RXBUF;
}

void uart_init(void) {
    ;
}
