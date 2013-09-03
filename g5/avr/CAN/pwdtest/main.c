/*********************************************
 * Unicorn main
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	// Init
    uartinit();
    pwm8Init();
    pwm16Init();
	adcInit(0);
	counter0Init();	
    sei();

    while (1){
	}
    return 0;
}
