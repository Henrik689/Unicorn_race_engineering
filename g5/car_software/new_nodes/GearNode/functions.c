/**
* @file functions.c
* @brief
*	Implementation of gear functions
*	
*/


#include "config.h"
#include "extern.h"
#include "prototyper.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
//#include "debug.h"
#include <util/delay.h>

void servoPosition(unsigned int position)
{

	gearCounter = 0;
    gearGotoPosition = position;
    gearActive = 1;
    
    float dutyCycle = (0.6278260870)*(float)position+(42.63130435);			// Formula to find the correct servo position
    
    OCR3CH = (0xFF) & ((int)dutyCycle>>8);
    OCR3CL = (0xFF) & ((int)dutyCycle);
}

void gearUp()			
{

	IgnCutOn();			// In order to change gears
	_delay_ms(100);		// Should be tested to get optimum time
	servoPosition(UPPOS);	// Check all POS in config.h
	_delay_ms(GEARWAIT);
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);	
	_delay_ms(100);		
	IgnCutOff();
}

void gearDown()							
{
	
	IgnCutOn();
	_delay_ms(100);
	servoPosition(DOWNPOS);
	_delay_ms(GEARWAIT);			
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);
	_delay_ms(100);
	IgnCutOff();

}

void gearNeutral1(void){			// Neutral position from 1st gear
	IgnCutOn;
	_delay_ms(100);
	servoPosition(NEUPOS1);
	_delay_ms(GEARWAIT);
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);
	_delay_ms(100);
	IgnCutOff;
}

void gearNeutral2(void){			// Neutral position from 2nd gear
	IgnCutOn;
	_delay_ms(100);
	servoPosition(NEUPOS2);
	_delay_ms(GEARWAIT);
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);
	_delay_ms(100);
	IgnCutOff;
}

