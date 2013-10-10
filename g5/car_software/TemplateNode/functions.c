/*********************************************
 * Functions
 *********************************************/

#include "config.h"
#include <stdlib.h>
#include <util/delay.h>
#include "functions.h"


// static void adcStop(void)
// {
// 	ADCSRA &=~(1<<ADEN); // ADC prescaler disable
// }

void sendtekst(char *tekstarray)
{
	short int i;
	for (i = 0; tekstarray[i] != '\0'; i++)
	{	
		while ((UCSR1A & (1 << UDRE1)) == 0) {};
		UDR1 = tekstarray[i];
	}
}
