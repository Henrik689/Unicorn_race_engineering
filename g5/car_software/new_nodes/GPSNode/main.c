#include <stdint.h>
#include <stdio.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#include <can.h>
#include <usart.h>
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
	USART1_SET_1_STOP_BIT();
	USART1_SET_CHAR_SIZE(UART_CHAR_8BIT);
	usart1_setBaudrate(GPS_BAUDRATE, UART_MODE_ASYNC_NORMAL);
	USART1_ENABLE_RX();
	USART1_ENABLE_TX();
	USART1_ENABLE_RX_INTERRUPT();

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
		usart1_printf("HEJ %d\n", i);
	}

    return 0;
}

static void rx_complete(uint8_t mob) {
	can_msg_t msg = {
		.mob = mob
	};
	can_receive(&msg);
	usart1_printf("Received id: %d on mob %d :: ", msg.id, msg.mob);

	int i;
	for (i = 0; i < msg.dlc; ++i){
		usart1_printf("%d ", msg.data[i]);
		//uart1_printf("0x%02X ", msg.data[i]);
	}
	usart1_putc('\n');
}

static void tx_complete(uint8_t mob) {
	MOB_ABORT();					// Freed the MOB
	MOB_CLEAR_INT_STATUS();			// and reset MOb status
	CAN_DISABLE_MOB_INTERRUPT(mob);	// Unset interrupt
}

static void can_default(uint8_t mob) {
	MOB_CLEAR_INT_STATUS(); 		// and reset MOb status
}
