#include <stdint.h>
#include <stdio.h>

#include <avr/interrupt.h>
#include <util/delay.h>

#include <can.h>
#include <uart.h>
#include <adc.h>


#include "init.h"
#include <io.h>

static void rx_complete(uint8_t mob);
static void tx_complete(uint8_t mob);
static void can_default(uint8_t mob);


int main(void)
{
	set_canit_callback(CANIT_RX_COMPLETED, rx_complete);
	set_canit_callback(CANIT_TX_COMPLETED, tx_complete);
	set_canit_callback(CANIT_DEFAULT, can_default);

	//Initialise the Gear node
	ioinit();									//Port setup
	uart0_init();
	uart1_init();								//Serial communication
	CAN_INIT_ALL();								//Can setup
    pwm16Init2();								//Setup PWM controller
	counter0Init();								//Init interrupt counter to overflow with 168Hz

	sei();										//Enable interrupt

	adc_setChannel(1);
	adc_setVref(INTERNAL);
	ADC_ENABLE();
	adc_setPrescaler(ADC_PRESCALAR_128);

	uart1_printf("\n\n\nSTARTING\n");


	can_msg_t rx_msg = {
		.mob = 8,
		.id = 4,
		.dlc = 7,

		.mode = MOB_RECIEVE
	};
	can_setup(&rx_msg);
	uart1_printf("1\n");
	
	SET_PIN_MODE(PORTF, PIN0, OUTPUT);
	SET_PIN_MODE(PORTF, PIN1, OUTPUT);
	SET_PIN_MODE(PORTF, PIN2, OUTPUT);
	SET_PIN_MODE(PORTF, PIN3, OUTPUT);
	SET_PIN_MODE(PORTF, PIN4, OUTPUT);
	SET_PIN_MODE(PORTF, PIN5, OUTPUT);
	SET_PIN_MODE(PORTF, PIN6, OUTPUT);
	SET_PIN_MODE(PORTF, PIN7, OUTPUT);

	DIGITAL_WRITE(PORTF, PIN0, HIGH); 	// takes 6 instruktions
	IO_SET_HIGH(PORTF, PIN1); 			// takes 1 instruktion
	DIGITAL_WRITE(PORTF, PIN2, HIGH);
	DIGITAL_WRITE(PORTF, PIN3, HIGH);
	DIGITAL_WRITE(PORTF, PIN4, HIGH);
	DIGITAL_WRITE(PORTF, PIN5, HIGH);
	DIGITAL_WRITE(PORTF, PIN6, HIGH);
	IO_SET_HIGH(PORTF, PIN7);
	
	int i=0;
	while(1){
		// Main work loop
		_delay_ms(250);
		//uart1_printf("1.5\n");
		can_msg_t tx_msg = {
			.mob = 10,
			.id = 4,
			.data = {'H', 'E', 'Y', ' ', 'J', 'O', 'E'},
			.dlc = 7,
			.mode = MOB_TRANSMIT
		};
		//uart1_printf("2\n");
		can_send(&tx_msg);
		uart1_printf("HEJ %d\n", i);


		//uint16_t res = 3;adc_readChannel(i);
		//uart1_printf("ADC channel %d = %d \n", i, res);

		DIGITAL_TOGGLE(PORTF, i); // This expands to 6 instruktions

		if(++i == 8){
			i = 0;
			uart1_txstring("\n");
		}
	}

    return 0;
}

static void rx_complete(uint8_t mob) {
	can_msg_t msg = {
		.mob = mob
	};
	can_receive(&msg);
	uart1_printf("Received id: %d on mob %d :: ", msg.id, msg.mob);
	uart1_txarr(msg.data, msg.dlc); uart1_txchar('\n');
}

static void tx_complete(uint8_t mob) {
	MOB_ABORT();					// Freed the MOB
	MOB_CLEAR_INT_STATUS();			// and reset MOb status
	CAN_DISABLE_MOB_INTERRUPT(mob);	// Unset interrupt
}

static void can_default(uint8_t mob) {
	MOB_CLEAR_INT_STATUS(); 		// and reset MOb status
}
