/*********************************************
 * Unicorn main gear kontrol
 *********************************************/

#include <config.h>
#include <extern.h>
#include <prototyper.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

// GPS TIME
int the_time_h = 0;
int the_time_m = 0;
int the_time_s = 0;

// North/South
char N_S_indicator = '0';

// East/West
char E_W_indicator = '0';

// satellites
unsigned short int satellites = 0;

unsigned int speed = 0;

// Pos
double lat = 0.0;
double lon = 0.0;

long int test = 0;

int main(void)
{
	char tempchar[2];

	ioinit();
	uartinit0();

	sendtekst("\n\nStartet\n\n\r");

	uartinit1();

	sei();

	while (1)
	{
		PORTC |= (1<<PC7); //tic

		sendtekst("Time: ");
		itoa(the_time_h,tempchar,10);
		sendtekst(tempchar);
		sendtekst(":");
		itoa(the_time_m,tempchar,10);
		sendtekst(tempchar);
		sendtekst(":");
		itoa(the_time_s,tempchar,10);
		sendtekst(tempchar);
		sendtekst("  ");
		
		test = lat*10000;

		sendtekst("Lat: ");
		ltoa(test,tempchar,10);
		sendtekst(tempchar);
		sendtekst("  ");

		sendtekst("N/S: ");
		while ((UCSR0A & (1 << UDRE0)) == 0) {};
		UDR0 = N_S_indicator;
		sendtekst("  ");

		test = lon*10000;

		sendtekst("Lon: ");
		ltoa(test,tempchar,10);
		sendtekst(tempchar);
		sendtekst("  ");

		sendtekst("E/W: ");
		while ((UCSR0A & (1 << UDRE0)) == 0) {};
		UDR0 = E_W_indicator;
		sendtekst("  ");

		sendtekst("Sat: ");
		itoa(satellites,tempchar,10);
		sendtekst(tempchar);
		sendtekst("  ");

		sendtekst("Speed [km/h]: "); // Ikke testet
		itoa(speed,tempchar,10);
		sendtekst(tempchar);

		sendtekst("\n\r");

		PORTC &=~ (1<<PC7); //toc

		//PORTC^= (1<<PC7);
		_delay_ms(1000);
	}
    return 0;
}
