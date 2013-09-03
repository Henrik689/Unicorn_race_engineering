/*********************************************
 * Kobling controller
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int KoblingPos = 0;
int KoblingPosRef = 0;
float KoblingPosKP = 2;

void KoblingPosController(unsigned int adcIn)
{
	int KoblingPosErr = 0;
	int measPos = 0;
	int outPos = 0;

	measPos = adcIn-KoblingPosMin;

	if(measPos<0)
		measPos = 0;
	if(measPos>KoblingPosMax)
		measPos = KoblingPosMax;

	KoblingPosErr =  KoblingPosRef -measPos;

	outPos = (int)(KoblingPosErr*KoblingPosKP) + muxMiddle;

	if(outPos>255)
		outPos = 255;
	if(outPos<0)
		outPos = 0; 

	PWM_duty_cycle_A_set(outPos);
}
