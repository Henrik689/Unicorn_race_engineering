/*********************************************
 * Interrupt-service-rutiner
 *********************************************/

#include <stdlib.h>
 #include <avr/interrupt.h>
#include "config.h" // F_CPU is defined here and must be defined before util/delay.h is included 

// ADC convert complete
ISR(ADC_vect,ISR_NOBLOCK)
{
 //    unsigned int ADCconv = 0;
	// unsigned int adlow = 0;
	// unsigned int adhigh = 0;
    
	// // Read ADC convertion
 //    adlow = ADCL;
 //    adhigh = ADCH;
	// ADCconv = (unsigned int)((adhigh<<8)|(adlow & 0xFF));
    //ADCconv = (ADCconv-1023)*-1; //Hvis det skal vendes
}

// Timer0 (8-bit) overflow interrupt (168 Hz)
ISR(TIMER0_OVF_vect)
{
    
}
