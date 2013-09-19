/*********************************************
 * Interrupt-service-rutiner
 *********************************************/

#include "config.h"
#include "prototyper.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Wheel speed
char speed1_ovf = 0;
char speed2_ovf = 0;
unsigned int speed1[AVGLENGTH];
unsigned int speed2[AVGLENGTH];
unsigned int speed1_avg = 0;
unsigned int speed2_avg = 0;
extern int i;
extern int j;

// ADC
unsigned int ADCconv = 0;
unsigned int setChannel = 0;
char channel = 0;
char channel_queue[] = {6,7};
unsigned int ang = 0;
unsigned int press = 0;

// debugging
char tmp[10];

// Timer0 (8-bit) overflow interrupt
ISR(TIMER0_OVF_vect)
{	
	// ADC start new reading
	// Change ADC channel
	ADMUX &= 0xf8;
	ADMUX |= channel_queue[setChannel++];

	// Start ADC-convert
	ADCSRA|=(1<<ADSC);

	if(setChannel>=ADCtotnum)
		setChannel = 0;
    
    speed1_avg = 0;
    speed2_avg = 0;
    int k;
    
    for (k = 0; k < AVGLENGTH; k++) {
        if (speed1[k]<AVGLENGTH) {
            speed1_avg += 1;
        }
        else
        {
            speed1_avg += speed1[k]/AVGLENGTH;
        }
        if (speed2[k]<AVGLENGTH) {
            speed2_avg += 1;
        }
        else
        {
            speed2_avg += speed2[k]/AVGLENGTH;
        }
    }
    
	//sendtekst("Speed1: ");
	ltoa(speed1_avg,tmp,10);
	sendtekst(tmp);	
	sendtekst("\t");
	/*sendtekst("Speed2: ");
	ltoa(speed2_avg,tmp,10);
	sendtekst(tmp);
	sendtekst("\t");
	sendtekst("Ang: ");
	ltoa(ang,tmp,10);
	sendtekst(tmp);		
	sendtekst("\t");
	sendtekst("Press: ");
	ltoa(press,tmp,10);
	sendtekst(tmp);		
	sendtekst("\r\n");
*/
}

// If counter overflow, speed = 0 (counter>2^16)
ISR(TIMER1_OVF_vect)
{
    i++;
    if (i >= AVGLENGTH){i = 0;}
	speed1_ovf = 1;
	speed1[i] = TCNT1;
    TCNT1 = 0;
}
ISR(TIMER3_OVF_vect)
{
    j++;
    if (j >= AVGLENGTH){j = 0;}
	speed2_ovf = 1;
	speed2[j] = TCNT3;
	TCNT3 = 0;
}
ISR(INT6_vect)
{
	if(speed1_ovf == 0)
	{
        i++;
        if (i >= AVGLENGTH){i = 0;}
		speed1[i] = TCNT1;
		TCNT1 = 0;
		speed1_ovf = 0;
	}
	else
	{
        TCNT1 = 0;
		speed1_ovf = 0;
	}
}
ISR(INT7_vect)
{	
	if(speed2_ovf == 0)
	{
        j++;
        if (j >= AVGLENGTH){j = 0;}
        speed2[j] = TCNT3;
		TCNT3 = 0;
		speed2_ovf = 0;
	}
	else
	{
		TCNT3 = 0;
		speed2_ovf = 0;
	}	
}

// ADC convert complete
ISR(ADC_vect,ISR_NOBLOCK)
{
	unsigned int adlow = 0;
	unsigned int adhigh = 0;
		
	// Read ADC channel
	channel = ADMUX & 0x07;

	// Read ADC convertion
   	adlow=ADCL;
    	adhigh=ADCH;
	ADCconv = (unsigned int)((adhigh<<8)|(adlow & 0xFF));
	
	// Channel 0 = ?
	if(channel == 0)
	{
		press = ADCconv;
	}
}
