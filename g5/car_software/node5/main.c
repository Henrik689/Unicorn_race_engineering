//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "can_std/can_lib.h"
#include <util/delay.h>
#include "can_new.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "can_logning.h"
#include "adc.h"
#include "test_vars.h"

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
unsigned short int satellites = 1;
unsigned int speed = 0;

int main (void)
{	
	U8 canData=0;
	CLKPR = 0x80;  CLKPR = 0x00;  // Clock prescaler Reset
	uint8_t data_buf[8];
	uint8_t j,i=0;

	enum valueID ID;
	init();				// Init CAN, UART, I/O
	sei(); 				// Init Interrupt
	Can_sei();			// Enable general can interrupt	
	Can_set_tx_int(); 	// Enable can tx interrupt
	AdcReadStart(); 	// Start ADC read process
//	uartinit1(); 		// GPS uart init

	while (1) {
		data_buf[0] = 38;
		data_buf[1] = 25;
		data_buf[2] = 66;
		i++;

		can_send_non_blocking(rpm_msgid, data_buf, 4);
		_delay_ms(1000);
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
