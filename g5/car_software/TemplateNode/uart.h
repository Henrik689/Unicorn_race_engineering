#ifndef UART_H
#define UART_H

void uart_init(void);

void uart_txchar(const char c);
void uart_txstring(char *str);

#endif /* UART_H */
