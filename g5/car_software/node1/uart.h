#define Usart1_tx_ei_en()	( UCSR1B |= (1<<UDRIE1) ) 
#define Usart1_tx_ei_dis()	( UCSR1B &= ~(1<<UDRIE1) ) 

void USART0_Transmit( unsigned char txData );
void USART1_Transmit( unsigned char txData );

void uart_init(void);         /* Initialize UART */
uint8_t uart_get (void);      /* Get a byte from UART Rx */
uint8_t uart_test(void);      /* Check number of data in UART Rx FIFO */
void uart_put_test(uint8_t device, unsigned char data);   /* Transmit a byte*/
void uart_put(unsigned char);   /* Transmit a byte*/
void uart_puts(const char *s);   /* Transmit a string of bytes*/ 
void uart1_put(unsigned char);
