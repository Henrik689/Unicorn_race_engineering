#include <avr/interrupt.h>
#include "config.h"
#include <util/delay.h>
#include "can_std/can_lib.h"
#include "can_comm.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "init.h"
#include "uart.h"


int main(void)
{
	//Initialise the Gear node
	ioinit();									//Port setup
	uart_init();								//Serial communication
	can_init(0);								//Can setup
    pwm16Init2();								//Setup PWM controller
	adcInit(1);									//Setup ADC for pot-meter or Amp meter
	counter0Init();								//Init interrupt counter to overflow with 168Hz

	sei();										//Enable interrupt

	//Enable CAN
	Can_sei();									//Enable all interrupts
	Can_set_tx_int();							//Enable interrupt on transmit message complete on CAN bus
	Can_set_rx_int();							//Enable interrupt on receive message complete on CAN bus

	while(1){
		// Main work loop
	}

    return 0;
   
}
