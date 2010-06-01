#include <io.h>
#include <signal.h>

#include "spi.h"
#include "uart.h"
#include "i2c.h"

#ifndef NULL
  #define NULL 0x0
#endif

enum usci_mode {
	NONE,
	UART,
	SPI,
	I2C
};

uint8_t spi_last_read;
int16_t spi_io_inited = 0;

static enum usci_mode uca_mode = NONE;
static enum usci_mode ucb_mode = NONE;

#define SCL_LINE 0x2
#define SDA_LINE 0x4

#define ERR_MODE	  -1
#define ERR_BUS_BUSY  -2
#define ERR_I2C_BUSY  -3
#define ERR_ADDR_NACK -4
#define ERR_DATA_NACK -5

void i2c_init(void) {
	// if I2C already configured, return.
	if (ucb_mode == I2C) {
		return;
	}

	// set UCSWRST
	UCB0CTL1 = UCSWRST;
	
	// configure ports
	// cc2500 chis select high
	P3SEL &= ~1;
	P3DIR |= 1;
	P3OUT |= 1;
	
	P3DIR |= 0x0F; // default to OUT
	P3SEL |= SCL_LINE | SDA_LINE; // Select SDA & SCL for I2C
	
	// configure all registers
	UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;
	UCB0CTL1 = UCSSEL_SMCLK | UCSWRST;
	UCB0BR0 = 3; // 1MHz/10 = 100kHz
	UCB0BR1 = 0;
	
	UCB0I2CIE &= ~(0xF); // no interrupt
	IE2 &= ~(0xC); // no interrupt
	
	// clear UCSWRST
	UCB0CTL1 &= ~(UCSWRST);
	
	ucb_mode = I2C;
}

int16_t i2c_write(uint8_t addr, const uint8_t* data, int16_t len) {
	int16_t i;
	
	// check configuration
	if (ucb_mode != I2C) {
		i2c_init();
	}
	
	if ( (UCB0STAT & UCBBUSY) || (UCB0STAT & UCSCLLOW) ) {
		// bus busy
		return ERR_I2C_BUSY;
	}
	
	// write address
	UCB0I2CSA = addr;
	
	// set transmitter and start condition
	UCB0CTL1 |= (UCTR | UCTXSTT);
	
	// wait for TXBUF ready
	while ( (IFG2 & UCB0TXIFG) == 0 ) ;
	
	// write first byte
	UCB0TXBUF = data[0];
	
	// wait for ACK or NACK (end of STT)
	while (UCB0CTL1 & UCTXSTT);
	if (UCB0STAT & UCNACKIFG) {
		// got address NACK
		// set stop condition
		UCB0CTL1 |= UCTXSTP;
		// wait end of STP
		while (UCB0CTL1 & UCTXSTP);
		return ERR_ADDR_NACK;
	}
	
	for (i=1;i<len;i++) {
		// wait for ACK/NACK
		while ( !(IFG2 & UCB0TXIFG) && !(UCB0STAT & UCNACKIFG) );
		if (UCB0STAT & UCNACKIFG) {
			// got data NACK
			// set stop condition
			UCB0CTL1 |= UCTXSTP;
			// wait end of STP
			while (UCB0CTL1 & UCTXSTP);
			return ERR_DATA_NACK;
		}
		// send byte
		UCB0TXBUF = data[i];
	}
	
	// wait for ACK/NACK
	while ( !(IFG2 & UCB0TXIFG) && !(UCB0STAT & UCNACKIFG) );
	if (UCB0STAT & UCNACKIFG) {
		// got data NACK
		// set stop condition
		UCB0CTL1 |= UCTXSTP;
		// wait end of STP
		while (UCB0CTL1 & UCTXSTP);
		return ERR_DATA_NACK;
	}
	
	// last data byte has been written
	// set STP
	UCB0CTL1 |= UCTXSTP;
	
	// wait until STP sent
	while (UCB0CTL1 & UCTXSTP);
	
	return len;
}

int16_t i2c_read (uint8_t addr, uint8_t* data, int16_t len) {
	int16_t i;
	
	// check configuration
	if (ucb_mode != I2C) {
		i2c_init();
	}
	
	if ( (UCB0STAT & UCBBUSY)  || (UCB0STAT & UCSCLLOW) ) {
		// bus busy
		return ERR_I2C_BUSY;
	}
	
	// write address
	UCB0I2CSA = addr;
	
	// set receiver and start condition
	UCB0CTL1 &= ~(UCTR);
	UCB0CTL1 |= UCTXSTT;
	
	// wait for ACK or NACK (end of STT)
	while (UCB0CTL1 & UCTXSTT) ;
	if (UCB0STAT & UCNACKIFG) {
		// got address NACK
		// set stop condition
		UCB0CTL1 |= UCTXSTP;
		// wait end of STP
		while (UCB0CTL1 & UCTXSTP);
		return ERR_ADDR_NACK;
	}
	
	for (i=0; i<len-1; i++) {
		// wait for data
		while (!(IFG2 & UCB0RXIFG));
		// read next byte
		data[i] = UCB0RXBUF;
	}
	
	// reading last byte, set stop condition
	UCB0CTL1 |= UCTXSTP;
	
	// wait for data
	while (!(IFG2 & UCB0RXIFG));
	// read next byte
	data[i] = UCB0RXBUF;
	
	// wait until STP sent
	while (UCB0CTL1 & UCTXSTP);
	
	return 0;
	
}

int16_t i2c_write_read(uint8_t addr, uint8_t data_w, uint8_t* data_r, int16_t len) {
	int16_t i;
	
	// check configuration
	if (ucb_mode != I2C) {
		i2c_init();
	}
	
	if ( (UCB0STAT & UCBBUSY)  || (UCB0STAT & UCSCLLOW) ) {
		// bus busy
		return ERR_I2C_BUSY;
	}
	
	// write address
	UCB0I2CSA = addr;
	// set transmitter and start condition
	UCB0CTL1 |= (UCTR | UCTXSTT);
	
	// wait for TXBUF ready
	while ( (IFG2 & UCB0TXIFG) == 0 ) ;
	
	// write byte
	UCB0TXBUF = data_w;
	
	// wait for ACK or NACK (end of STT)
	while (UCB0CTL1 & UCTXSTT);
	
	if (UCB0STAT & UCNACKIFG) {
		// got address NACK
		// set stop condition
		UCB0CTL1 |= UCTXSTP;
		// wait end of STP
		while (UCB0CTL1 & UCTXSTP);
		return ERR_ADDR_NACK;
	}
	
	// set receiver
	UCB0CTL1 &= ~UCTR;
	// set start condition
	UCB0CTL1 |= UCTXSTT;
	// clear TX IFG
	IFG2 &= ~UCB0TXIFG;
	
	// we should be in receive state, wait for end of STT
	while (UCB0CTL1 & UCTXSTT) ;
	
	if (UCB0STAT & UCNACKIFG) {
		// got address NACK
		// set stop condition
		UCB0CTL1 |= UCTXSTP;
		// wait end of STP
		while (UCB0CTL1&UCTXSTP);
		return ERR_ADDR_NACK;
	}
	
	for (i=0; i<len-1; i++) {
		// wait for data
		while (!(IFG2 & UCB0RXIFG));
		// read next byte
		data_r[i] = UCB0RXBUF;
	}
	
	
	// reading last byte, set stop condition
	UCB0CTL1 |= UCTXSTP;
	
	// wait for RX data ready
	while ((IFG2 & UCB0RXIFG) == 0) ;
	data_r[i] = UCB0RXBUF;
	
	
	// wait until STP sent
	while (UCB0CTL1 & UCTXSTP);
	
	return 0;
	
}

void spi_init_io(void) {
	
	/* configure all SPI related pins */
	P3SEL |= (1<<1) | (1<<2) | (1<<3);
	P3DIR |= (1<<1) | (1<<3);
	P3DIR &= ~(1<<2);

	// configure P3.0 IO for radio CSn
	P3SEL &= ~1; // set IO
	P3DIR |= 1; // set output
	spi_radio_deselect();

	spi_io_inited = 1;
}

void spi_init(void) {
	if (!spi_io_inited) {
		spi_init_io();
	}
	
	// initialize the SPI registers with UCSWRST
	UCB0CTL1 = UCSWRST;
	UCB0CTL1 = UCSSEL_SMCLK | UCSWRST;
	UCB0CTL0 = UCCKPH | UCMSB | UCMST | UCSYNC;
	UCB0BR0  = 0x2;
	UCB0BR1  = 0x0;

	// clear UCSWRST
	UCB0CTL1 &= ~UCSWRST;

	// store active mode
	ucb_mode = SPI;
}

void spi_select_radio() {
	// check configuration
	if (ucb_mode != SPI) {
		spi_init();
	}
	P3OUT &= ~1;
}

void spi_deselect_radio() {
	P3OUT |= 1;
}

int spi_write(const uint8_t* data, int len) {
	int i;
	uint8_t dummy;
	
	// check configuration
	if (ucb_mode != SPI) {
		spi_init();
	}

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
	
	// check configuration
	if (ucb_mode != SPI) {
		spi_init();
	}
	
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
	
	// check configuration
	if (ucb_mode != SPI) {
		spi_init();
	}
	
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
	
	// check configuration
	if (ucb_mode != SPI) {
		spi_init();
	}
	
	if (UCB0STAT & UCBUSY)
		return 0;
	
	UCB0TXBUF = 0;
	// wait for RX ready
	while (!(IFG2 & UCB0RXIFG));
	
	// return RX data
	return UCB0RXBUF;
}

int spi_read_interrupt(uint8_t* data, int len, spi_transfer_done_cb_t cb) {


	return 1;
}


void uart_init(void) {
	P3SEL |= 0x30;							 // P3.4,5 = USCI_A0 TXD/RXD
	UCA0CTL1 |= UCSSEL_2;					 // SMCLK
	UCA0BR0 = 8;							  // 1MHz 115200
	UCA0BR1 = 0;							  // 1MHz 115200
	UCA0MCTL = UCBRS2 + UCBRS0;			   // Modulation UCBRSx = 5
	UCA0CTL1 &= ~UCSWRST;					 // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;						  // Enable USCI_A0 RX interrupt
	eint();
	
	uca_mode = UART;
}

int uart_putchar(int c) {
	
	UCA0TXBUF = (char) c;
	while (!(IFG2&UCA0TXIFG));
	return c;
}

int (*uart_cb)(int c) = 0x0;
void uart_register_cb(int (*cb)(int c)) {
	uart_cb = cb;
}

interrupt (USCIAB0RX_VECTOR) uscirx_irq(void) {
	char dummy;
	if (IFG2 & UCA0RXIFG) {
		// UCA RX ready (uart)
		dummy = UCA0RXBUF;
		if (uart_cb)
			if (uart_cb(dummy))
				LPM4_EXIT;
	}

	if (IFG2 & UCB0RXIFG) {
		// UCB RX ready (spi/i2c)
		dummy = UCB0RXBUF;

		if (ucb_mode == SPI) {
			// handle SPI receive
		} else if (ucb_mode == I2C) {
			// handle I2C receive
		}
	}
}

interrupt (USCIAB0TX_VECTOR) uscitx_irq(void) {
	char dummy;
	if (IFG2 & UCA0TXIFG) {
		// UCA TX ready (uart)
	}

	if (IFG2 & UCB0TXIFG) {
		// UCB TX ready (spi/i2c)
		
		if (ucb_mode == SPI) {
			// handle SPI transmit
		} else if (ucb_mode == I2C) {
			// handle I2C transmit
		}
	}
}
