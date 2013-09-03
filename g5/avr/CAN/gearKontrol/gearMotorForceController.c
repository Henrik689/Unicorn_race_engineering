/*********************************************
 * Gear Torque Controller
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <stdlib.h>

int Kp = 1;
int filterCurrentOld = 0;

unsigned int torqueController(unsigned int current)
{
	int torqueErr = 0;
	int pwm = 0;
	int filterCurrent = 0;

	// Eksperimentielt (for at undga at PWM gAr helt i 0 ved start og bremsestrom)
	if(current>GEARFORCEREF/2)
		current = GEARFORCEREF/2;

	// Lav-pass filter for at undga regulering pa strom spikes
    filterCurrent = (int)((double)FILTERKONSTANT*current+(double)(1-FILTERKONSTANT)*filterCurrentOld);
    filterCurrentOld = filterCurrent;

	torqueErr = GEARFORCEREF-filterCurrent;

	pwm = torqueErr * Kp;

	// Saturation
	if(pwm<0)
		pwm = 0;
	if(pwm>255)
		pwm = 255;

	return (unsigned short int)pwm;
}
