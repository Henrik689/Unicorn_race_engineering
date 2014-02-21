#include <stdint.h>
#include <stdio.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#include <can.h>
#include <uart.h>
#include <io.h>

static void rx_complete(uint8_t mob);
static void tx_complete(uint8_t mob);
static void can_default(uint8_t mob);

#define GPS_BAUDRATE (4800)

int main(void){
	set_canit_callback(CANIT_RX_COMPLETED, rx_complete);
	set_canit_callback(CANIT_TX_COMPLETED, tx_complete);
	set_canit_callback(CANIT_DEFAULT, can_default);

	// Enable uart to talk to the GPS
	uart1_enableRX();
	uart1_enableTX();
	uart1_setModeAsync();
	uart1_setNumberOfStopBits(1);
	uart1_setCharSize(UART_CHAR_8BIT);
	uart1_setBaudRate(GPS_BAUDRATE, UART_MODE_ASYNC_NORMAL);
	uart1_enableRXInterrupt();

	CAN_INIT_ALL();								//Can setup

	sei();										//Enable interrupt


	int i=0;
	while(1){
		// Main work loop
		_delay_ms(250);
		can_msg_t tx_msg = {
			.mob = 10,
			.id = 4,
			.data = {'H', 'E', 'Y', ' ', 'J', 'O', 'E'},
			.dlc = 7,
			.mode = MOB_TRANSMIT
		};
		can_send(&tx_msg);
		uart1_printf("HEJ %d\n", i);
	}

    return 0;
}

// This is where we receive GPS data
ISR(USART1_RX_vect){
	char data = UDR1;
}

static void rx_complete(uint8_t mob) {
	can_msg_t msg = {
		.mob = mob
	};
	can_receive(&msg);
	uart1_printf("Received id: %d on mob %d :: ", msg.id, msg.mob);

	int i;
	for (i = 0; i < msg.dlc; ++i){
		uart1_printf("%d ", msg.data[i]);
		//uart1_printf("0x%02X ", msg.data[i]);
	}
	uart1_txchar('\n');
}

static void tx_complete(uint8_t mob) {
	MOB_ABORT();					// Freed the MOB
	MOB_CLEAR_INT_STATUS();			// and reset MOb status
	CAN_DISABLE_MOB_INTERRUPT(mob);	// Unset interrupt
}

static void can_default(uint8_t mob) {
	MOB_CLEAR_INT_STATUS(); 		// and reset MOb status
}
