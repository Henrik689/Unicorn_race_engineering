/*********************************************
 * Unicorn Node 4
 *********************************************/

#include "config.h"
#include "prototyper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

int main(void)
{
	ioinit();
	uartinit();
	//can_init(0);
	//Can_sei();
	//Can_set_tx_int();
	//Can_set_rx_int();
	counter0Init();
	counter1Init();
	counter3Init();
	extIntInit();
	adcInit(0);
	sei();

	while (1)
	{
	}
    return 0;
}
