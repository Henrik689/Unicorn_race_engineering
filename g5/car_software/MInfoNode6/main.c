//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "can_std/can_lib.h"
#include <util/delay.h>
#include "can_new.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "can_logning.h"
#include "adc.h"
#include "gps/gps.h"

#define ID_BASE 0x80
void init(void);

// GPS TIME
int the_time_h = 0;
int the_time_m = 0;
int the_time_s = 0;

// North/South
char N_S_indicator = '0';

// East/West
char E_W_indicator = '0';

// satellites
unsigned char satellites = 1;
unsigned int speed = 0;

// Pos
double lat = 0;
double lon = 0;

int main (void)
{	
	U8 canData=0;
	CLKPR = 0x80;  CLKPR = 0x00;  // Clock prescaler Reset
	uint8_t data_buf[8];
	uint8_t j,i=0;

	_delay_ms(1000);
	enum valueID ID;
	init();				// Init CAN, UART, I/O
	sei(); 				// Init Interrupt
	Can_sei();			// Enable general can interrupt	
	Can_set_tx_int(); 	// Enable can tx interrupt
	AdcReadStart(); 	// Start ADC read process
	uartinit1(); 		// GPS uart init
	
	while (1) {
		data_buf[0] = 59;
		data_buf[1] = satellites;
		can_send_non_blocking(rpm_msgid, data_buf, 2);

		_delay_ms(500);

		data_buf[0] = 60;
		data_buf[1] = (speed>>8)& 0xFF;
		data_buf[2] = (speed & 0xFF);
		can_send_non_blocking(rpm_msgid, data_buf, 3);

		//_delay_ms(200);
		//data_buf[0] = 61;
		//data_buf[1] = (the_time_h>>8)& 0xFF;
		//data_buf[2] = (the_time_h & 0xFF);
		//can_send_non_blocking(rpm_msgid, data_buf, 3);

		//_delay_ms(200);
		//data_buf[0] = 62;
		//data_buf[1] = (the_time_m>>8)& 0xFF;
		//data_buf[2] = (the_time_m & 0xFF);
		//can_send_non_blocking(rpm_msgid, data_buf, 3);

		//_delay_ms(200);
		//data_buf[0] = 63;
		//data_buf[1] = (the_time_s>>8)& 0xFF;
		//data_buf[2] = (the_time_s & 0xFF);
		//can_send_non_blocking(rpm_msgid, data_buf, 3);

		_delay_ms(500);
	}
	return 0;
}

void init(void)
{
    // CAN Init
    can_init(0);

    // IO Init
    DDRA = 0xFF;
}
