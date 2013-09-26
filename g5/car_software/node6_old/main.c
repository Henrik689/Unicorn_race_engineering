//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "can_std/can_lib.h"
#include <util/delay.h>
#include "can_new.h"
#include "../lib/can_defs.h"
#include "test_vars.h"
#include "adc/adc.h"

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

// Pos
double lat = 0.0;
double lon = 0.0;

long int test = 0;

//_____ D E F I N I T I O N S __________________________________________________
#define ID_BASE 0x80
//_____ D E C L A R A T I O N S ________________________________________________
void init(void);
void sendtekst(char *);

char tempchar[4];

int main (void)
{	
	U8 canData=0;
	CLKPR = 0x80;  CLKPR = 0x00;  // Clock prescaler Reset
	uint8_t data_buf[8];
	uint8_t j,i=0;

	//  Init CAN, UART, I/O
	init();
	uartinit1();

	/* 
	// Reset GPS NMEA
	char i = 0;

	_delay_ms(1000);
	_delay_ms(1000);
	_delay_ms(1000);

	for(i=0;i<999;i++){
		sendtekst("$PSRF100,1,4800,8,1,0*");
		itoa(i,tempchar,16);
		sendtekst(tempchar);
		sendtekst("\n\r")
		_delay_ms(10);
	}
	*/

	sei();		/* Interrupt enable */

	Can_sei();		/* Enable general can interrupt */
	Can_set_tx_int();	/* Enable can tx interrupt */

    	// --- Init variables

	AdcReadStart();
	
	while (1) {
		data_buf[0] = satellites;
		data_buf[1] = the_time_h;
		data_buf[2] = the_time_m;
		data_buf[3] = the_time_s;
		data_buf[4] = N_S_indicator;
		data_buf[5] = E_W_indicator;
		data_buf[6] = 14;
		data_buf[7] = 12;
		can_send_non_blocking(rpm_msgid, data_buf, 8);
		_delay_ms(1000);
		data_buf[0] = data[0];
	       	data_buf[1] = data[1];
		data_buf[2] = data[2];
		can_send_non_blocking(rpm_msgid, data_buf, 3);
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

void sendtekst(char *tekstarray)
{
	short int i;
	for (i = 0; tekstarray[i] != '\0'; i++)
	{	
		while ((UCSR1A & (1 << UDRE1)) == 0) {};
		UDR1 = tekstarray[i];
	}
}
