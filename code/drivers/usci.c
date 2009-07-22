#include <io.h>
#include <signal.h>

#include "spi.h"
#include "uart.h"

#ifndef NULL
  #define NULL 0x0
#endif

#define RADIO_SELECT() P3OUT &= ~1
#define RADIO_DESELECT() P3OUT |= 1

uint8_t spi_last_read;

void spi_init(void) {
    // configure P3.0 IO for radio chip select
    P3SEL &= ~1; // set IO
    P3DIR &= ~1; // set output
    RADIO_DESELECT(); // default deselect;
    
    // configure P3.1 P3.2 P3.3 for SPI
    P3SEL |= (1<<1) | (1<<2) | (1<<3);
    
    // set UCSWRST (reset)
    UCB0CTL1 = UCSWRST;
    
    // initialize all registers
    UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC;
    UCB0CTL1 = UCSSEL_SMCLK | UCSWRST;
    UCB0BR0 = 1;
    UCB0BR1 = 0;
    UCB0STAT = UCLISTEN;
    
    // disable interrupts
    IE2 &= ~(UCB0TXIE | UCB0RXIE);
    
    // clear UCSWRST
    UCB0CTL1 &= ~UCSWRST;
}

void spi_select(int on) {
    // this is the radio chip select
    if (on)
        RADIO_SELECT();
    else
        RADIO_DESELECT();
}

int spi_write(const uint8_t* data, int len, int (*done)(void)) {
    int i;
    
    if (done==NULL) {
        if ( UCB0STAT & UCBUSY )
            return 0;

        for (i=0; i<len; i++) {
            UCB0TXBUF = data[i];
            while (!(IFG2 & UCB0TXIFG)) ;
        }

        while (UCB0STAT & UCBUSY);

        return i;
    } else {
        // not implemented
        return 0;
    }
    return 0;
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

int spi_read(uint8_t* data, int len, int (*done)(void)) {
    int i;
    
    if (done==NULL) {
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
    } else {
        // not implemented
        return 0;
    }
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
