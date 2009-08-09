#ifndef _UART_H_
#define _UART_H_

void uart_init(void);

int uart_putchar(int c);
void uart_register_cb(int (*cb)(int c));

#endif
