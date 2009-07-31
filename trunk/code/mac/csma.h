#ifndef _CSMA_H_
#define _CSMA_H_

#define CSMA_BROADCAST 0xFFFF
extern uint16_t csma_addr;

void csma_init();

int csma_send(uint16_t dest_addr, uint8_t* data, int len);
int csma_read(uint16_t* from, uint8_t* data, int len);

void csma_set_rx_cb(int (*cb) (void));
void csma_set_tx_cb(int (*cb) (void));

#endif
