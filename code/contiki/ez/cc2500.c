#include <io.h>
#include <signal.h>

#include "spi.h"
#include "cc2500.h"
//#include "cc2500_smartrf.h"

#define GDO0_PIN 6
#define GDO2_PIN 7

#define READ_BIT  0x80
#define BURST_BIT 0x40

static int (*gdo0_cb)(void);
static int (*gdo2_cb)(void);

static inline void micro_delay(register unsigned int n) {
	__asm__ __volatile__ (
			"1: \n"
			" dec %[n] \n"
			" jne 1b \n"
			: [n] "+r"(n));
}

void cc2500_init(uint8_t* init_cfg, int len) {

	P2SEL &= ~(BV(GDO0_PIN) | BV(GDO2_PIN));
	P2DIR &= ~(BV(GDO0_PIN) | BV(GDO2_PIN));

	/* initialize SPI */
	spi_init();

	/* radio reset sequence */
	spi_radio_select();
	micro_delay(10);
	spi_radio_deselect();

	/* hold CSn high for at least 40 microseconds */
	micro_delay(100);

	/* pull CSn low and wait for SO to go low */
	spi_radio_select();
	while (spi_read_somi())
		;

	/* directly send strobe command - cannot use function as it affects CSn pin */
	spi_write_single(CC2500_STROBE_SRES);

	/* wait for SO to go low again, reset is complete at that point */
	while (spi_read_somi())
		;

	/* return CSn pin to its default high level */
	spi_radio_deselect();

	/* configure radio registers */

	int i;
	if (init_cfg && len > 0) {
		for (i = 0; i < len; i++) {
			cc2500_write_reg(init_cfg[2 * i], init_cfg[2 * i + 1]);
		}
	}
}

uint8_t cc2500_cmd_strobe(uint8_t addr) {
	if (addr < 0x30 || addr > 0x3D) {
		while (1)
			;
	}
	spi_radio_select();
	spi_write_single(addr);
	spi_radio_deselect();

	return spi_last_read;
}

uint8_t cc2500_read_reg(uint8_t addr) {
	uint8_t ret_val = 0;
	if (addr > 0x3B) {
		while (1)
			;
	}

	spi_radio_select();
	spi_write_single(addr | BURST_BIT | READ_BIT);
	ret_val = spi_read_single();
	spi_radio_deselect();

	return ret_val;
}

void cc2500_write_reg(uint8_t addr, uint8_t value) {
	if (addr > 0x2E && addr != 0x3E) {
		while (1)
			;
	}

	spi_radio_select();
	spi_write_single(addr);
	spi_write_single(value);
	spi_radio_deselect();

}

void cc2500_write_fifo(uint8_t * pWriteData, uint8_t len) {
	spi_radio_select();
	spi_write_single(CC2500_REG_TXFIFO | BURST_BIT);
	spi_write(pWriteData, len);
	spi_radio_deselect();
}

void cc2500_read_fifo(uint8_t * pReadData, uint8_t len) {
	int i;
	spi_radio_select();
	spi_write_single(CC2500_REG_RXFIFO | BURST_BIT | READ_BIT);
	for (i = 0; i < len; i++) {
		pReadData[i] = spi_read_single();
	}
	//~ spi_read(pReadData, len);
	spi_radio_deselect();
}

void cc2500_gdo0_int_set_cb(int(*cb)(void)) {
	gdo0_cb = cb;
}
void cc2500_gdo2_int_set_cb(int(*cb)(void)) {
	gdo2_cb = cb;
}

void port2irq(void);
/**
 * Interrupt service routine for PORT2.
 * Used for handling CC2500 interrupts triggered on
 * the GDOx pins.
 */
interrupt(PORT2_VECTOR) port1irq(void) {
	if (P2IFG & BV(GDO0_PIN)) {
		P2IFG &= ~BV(GDO0_PIN);
		if (gdo0_cb != 0x0) {
			if (gdo0_cb()) {
				LPM4_EXIT;
			}
		}
	}

	if (P2IFG & BV(GDO2_PIN)) {
		P2IFG &= ~BV(GDO2_PIN);
		if (gdo2_cb != 0x0) {
			if (gdo2_cb()) {
				LPM4_EXIT;
			}
		}
	}
}
