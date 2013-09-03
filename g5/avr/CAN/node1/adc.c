#include "config.h"

// ADC konvertering
unsigned int convertanalog(unsigned channel)	
{
	unsigned int adlow = 0;
	unsigned int adhigh = 0; 
	
    ADMUX=(1<<REFS0)|(channel & 0x0f);
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    ADCSRA|= (1<<ADSC);
    while(bit_is_set(ADCSRA,ADSC)); 
    adlow=ADCL; 
    adhigh=ADCH;
    return((unsigned int)((adhigh<<8)|(adlow & 0xFF)));
}
