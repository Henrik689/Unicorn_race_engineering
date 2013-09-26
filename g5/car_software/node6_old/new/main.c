/*********************************************
 * Unicorn Node 6 (wheelspeed, gps, adc)
 *********************************************/

#include "config.h"
#include "prototyper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "can_std/can_lib.h"
#include "can_std/can_drv.h"
#include "can.h"

// GPS
int the_time_h = 0;
int the_time_m = 0;
int the_time_s = 0;
char N_S_indicator = '0';
char E_W_indicator = '0';
char satellites = 1;
unsigned int speed = 0;
double lat = 0.0;
double lon = 0.0;
int i = 0;
int j = 0;

int main(void)
{
	ioinit();
	uartinit();
	can_init(0);
	Can_sei();
	Can_set_tx_int();
	Can_set_rx_int();
	counter0Init();
	counter1Init();
	counter3Init();
	extIntInit();
	//adcInit(0);
	sei();

	// CAN
	uint8_t data_response_buffer[8];
	
	st_cmd_t data_msg;

	uint8_t data_tx[8];
	data_tx[0] = 39;
	data_tx[1] = 1;

	while (1)
	{
		_delay_ms(50);
		can_send_non_blocking(133, data_tx, 8);
	}
    return 0;
}
