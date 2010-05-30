#ifndef _MAC_H_
#define _MAC_H_

#define MAC_BROADCAST 0xFF
extern uint8_t mac_addr;

void mac_init();

int16_t mac_send(uint8_t* data, int16_t len, uint8_t to);

void mac_set_rx_cb(int16_t (*cb) (uint8_t* data, int16_t len, uint8_t from));
void mac_set_tx_cb(int16_t (*cb) (int16_t success));

#endif
