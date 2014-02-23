#include <stdint.h>
#include <util/delay.h>

#include <can.h>
#include <usart.h>


static void rx_complete(uint8_t mob);
static void tx_complete(uint8_t mob);
static void can_default(uint8_t mob);


int main(void) {
	set_canit_callback(CANIT_RX_COMPLETED, rx_complete);
	set_canit_callback(CANIT_TX_COMPLETED, tx_complete);
	set_canit_callback(CANIT_DEFAULT, can_default);

	usart1_init();
	CAN_INIT_ALL();								//Can setup

	sei();										//Enable interrupt

	usart1_printf("\n\n\nSTARTING\n");

	// Set MOB 8 to listen for messeges with id 4 and length 7
	can_msg_t rx_msg = {
		.mob = 8,
		.id = 4,
		.dlc = 7,

		.mode = MOB_RECIEVE
	};
	can_setup(&rx_msg);


	usart1_printf("Listning for id %d on mob %d with a msg length %d\n",
		rx_msg.id,
		rx_msg.mob,
		rx_msg.dlc
	);


	while(1){
		// Main work loop
		_delay_ms(250);

		// send a message with id 4 on MOB 10
		can_msg_t tx_msg = {
			.mob = 10,
			.id = 4,
			.data = {'H', 'E', 'L', 'L', 'O', '!', '!'},
			.dlc = 7,
			.mode = MOB_TRANSMIT
		};

		can_send(&tx_msg);
		usart1_printf("CAN Tx\t id %d, mob %d, :: ", tx_msg.id, tx_msg.mob);

		// As tx_msg.data is a byte array we cant treat it as a string
		usart1_putn(sizeof(tx_msg.data)/sizeof(tx_msg.data[0]), tx_msg.data);
		usart1_putc('\n');
	}

    return 0;
}

// Callback to be run when rx comletes on the CAN
static void rx_complete(uint8_t mob) {
	can_msg_t msg = {
		.mob = mob // mob is the MOB that fired the interrupt
	};
	can_receive(&msg); // Fetch the message and fill out the msg struct

	// Print out the received data. Please dont print inside can callbacks
	// in real code as these are run inside the can ISR
	usart1_printf("CAN Rx\t id: %d on mob %d :: ", msg.id, msg.mob);
	usart1_putn(msg.dlc, msg.data); usart1_putc('\n');
}

static void tx_complete(uint8_t mob) {
	// We clear the mob so it can be used again
	MOB_ABORT();					// Freed the MOB
	MOB_CLEAR_INT_STATUS();			// and reset MOB status
	CAN_DISABLE_MOB_INTERRUPT(mob);	// Unset interrupt
}

static void can_default(uint8_t mob) {
	MOB_CLEAR_INT_STATUS(); 		// reset MOB status
}
