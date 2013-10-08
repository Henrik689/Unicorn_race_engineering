/*********************************************
 * Functions
 *********************************************/

#include "config.h"
#include "extern.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "debug.h"
#include <util/delay.h>

#include "functions.h"

void servoPosition(unsigned int position) {
	count = 0;
	gearCounter = 0;
    gearGotoPosition = position;
    gearActive = 1;
    float dutyCycle = (0.6278260870)*(float)position+(42.63130435);
    OCR3CH = (0xFF) & ((int)dutyCycle>>8);
    OCR3CL = (0xFF) & ((int)dutyCycle);
}//Maximum: 0x01, 0xAE; //Minimum: 0x00, 0x45;

void gearUp(void){
	IgnCutOn;
	_delay_ms(100);	
	servoPosition(UPPOS);
	_delay_ms(GEARWAIT);
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);
    sendtekst("2");
    //gearButActive = 0;
	_delay_ms(100);
	IgnCutOff;
	
	
	}

void gearDown(void){
	IgnCutOn;
	_delay_ms(100);	
	servoPosition(DOWNPOS);
	_delay_ms(GEARWAIT);
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);
    sendtekst("4");
    //gearButActive = 0;
	_delay_ms(100);
	IgnCutOff;
}

void gearNeutral1(void){
	IgnCutOn;
	_delay_ms(100);
	servoPosition(NEUPOS1);
	_delay_ms(GEARWAIT);
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);
    //gearButActive = 0;
	_delay_ms(100);
	IgnCutOff;
}

void gearNeutral2(void){
	IgnCutOn;
	_delay_ms(100);
	servoPosition(NEUPOS2);
	_delay_ms(GEARWAIT);
	servoPosition(MIDPOS);
	_delay_ms(GEARWAIT);
    //gearButActive = 0;
	_delay_ms(100);
	IgnCutOff;
}

void servoCheck(void)
{/*
    if (abs(gearPosition-gearGotoPosition) < 5 && gearCounter > 100) {
        gearActive = 0;
        gearCounter = 0;
        if (gearGotoPosition != MIDPOS) {
            servoPosition(MIDPOS);
        }
    }*/
     if (gearCounter > 100) {
        gearActive = 0;
        gearCounter = 0;
        servoPosition(MIDPOS);
    }
    else if (abs(gearPosition - gearPositionOld) < 5) {
        gearCounter++;
    }
}

/*static void adcStop(void)
{
	ADCSRA &=~(1<<ADEN); // ADC prescaler disable
}*/

void sendtekst(char *tekstarray)
{
	short int i;
	for (i = 0; tekstarray[i] != '\0'; i++)
	{	
		while ((UCSR1A & (1 << UDRE1)) == 0) {};
		UDR1 = tekstarray[i];
	}
}
