/*********************************************
 * Functions
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

void sendtekst(char *tekstarray)
{
	short int i;
	for (i = 0; tekstarray[i] != '\0'; i++)
	{	
		while ((UCSR0A & (1 << UDRE0)) == 0) {};
		UDR0 = tekstarray[i];
	}
}

