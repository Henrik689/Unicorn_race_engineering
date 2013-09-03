/*********************************************
 * Press controller
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int pressRef = 0;
float intpressOld = 0;

void presscontroller(unsigned int adc)
{
		int pressBar = 0;
		int PressKP = 50;
		float PressKI =  30.0;	
		float intpress = 0;	
		int err = 0;		
		int out = 0;     

		// ADC to Bar       	
		pressBar = 0.17*adc-18.0;        

		if(pressBar<=0)
		    pressBar = 0;
        
		// Press Error
        err = (pressRef-pressBar);

		// Integral
        intpress = intpressOld + err * 0.011;
		intpressOld = intpress;

		// PI-controller
		out = (int)(err*PressKP + intpress * PressKI);        

		// Anti windup
		if(intpressOld>1000)
		   intpressOld = 1000;
		if(intpressOld<-1000)
		    intpressOld = -1000;

		// Output saturation
		if(out>dutymax)
		    out = dutymax;
		if(out<=0)
		    out = 0;

		// Controller to output
		PWM_duty_cycle_A16_set(out);
}

