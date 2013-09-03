/*********************************************
 * Interrupt-service-rutiner
 *********************************************/

#include <config.h>
#include <extern.h>
#include <prototyper.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

// ADC
unsigned int ADCconv = 0;
char setChannel = 0;

// Temp Var
char count = 0;
char retning = 0;
char tempchar[5];

// ADC convert complete
ISR(ADC_vect)
{
	unsigned int adlow = 0;
	unsigned int adhigh = 0;
	char channel = 0;
		
	// Read ADC channel
	channel = ADMUX & 0x07;

	// Read ADC convertion
    adlow=ADCL;
    adhigh=ADCH;
	ADCconv = (unsigned int)((adhigh<<8)|(adlow & 0xFF));	

	// Channel 0 = Press ADC convert
	if(channel == 0)
	{
		// Press Controller
		presscontroller(ADCconv);	
	}

	// Channel 1 = Kobling pos ADC convert
	if(channel == 1)
	{
		// Kobling Controller
		KoblingPosController(ADCconv);
		debugging();
	}

	// Channel 2 = Gear pos ADC convert
	//if(channel == 2)
	//{
	//}
}

 // UART interrupt
ISR(USART0_RX_vect)
{
	char data;
	char tempchar[5];	
	data = UDR0;	// Lægger data fra seriel bufffer i variabel 

    if(data == '1')
    {
	    KoblingPosRef = 130;
	}

    if(data == '2')
    {
	    KoblingPosRef = 70;
	}

    if(data == '3')
    {
	    KoblingPosRef = 10;
	}

    if(data == 'u')
    {
    KoblingPosRef = KoblingPosRef+10;
    }

    if(data == 'h')
    {
    KoblingPosRef = KoblingPosRef-10;
    }

    if(data == 'z')
    {
    pressRef++;
    }

    if(data == 'x')
    {
    pressRef--;
    }

    if(data == '<')
    {
    pressRef = 110;
    }

    if(data == 'c')
    {
    pressRef = 0;
    }

    if(data == 'p')
    {
    KoblingPosKP = KoblingPosKP + 0.1;
    }

    if(data == 'l')
    {
    KoblingPosKP = KoblingPosKP - 0.1;
    }
	
	if(data == 'a')
    {
        count = 0;
        retning = 0;
        PWM_duty_cycle_A_set(0);
	    PWM_duty_cycle_B_set(0);
    }

    if(data == 'd')
    {
        count = 0;
        retning = 1;
        PWM_duty_cycle_A_set(0);
	    PWM_duty_cycle_B_set(0);
    }
	
	if(data == 'w')
	{
        if(count>=254)
            count = 254;

        count++;

        if(retning)
		    PWM_duty_cycle_A_set(count);
        else
            PWM_duty_cycle_B_set(count);
	}	
	if(data == 's')
	{
        if(count<=1)
            count = 1;            

        count--;
        
        if(retning)        
		    PWM_duty_cycle_A_set(count);
        else
            PWM_duty_cycle_B_set(count);
	}
	if(data == 'q')
	{
        if(count>=254)
            count = 254;

        count = 255;

        if(retning)
		    PWM_duty_cycle_A_set(count);
        else
            PWM_duty_cycle_B_set(count);
	}
	if(data == 'e')
	{
        if(count>=254)
            count = 254;

        count = 0;

        if(retning)
		    PWM_duty_cycle_A_set(count);
        else
            PWM_duty_cycle_B_set(count);
	}	
}

// Timer0 (8-bit) overflow interrupt, ADC convert trigger
ISR(TIMER0_OVF_vect)
{	
	// Change ADC channel
	ADMUX &= 0xf8;
	ADMUX |= setChannel++;
	// Start ADC-convert
	ADCSRA|=(1<<ADSC);
	
	if(setChannel>=ADCtotnum)
		setChannel = 0;
}

// Debugging
void debugging(void)
{
		itoa(pressRef, tempchar,10); 					// 1 Press Ref
		sendtekst(tempchar);
		sendtekst(";");

		itoa((int)(0.17*ADCconv-18.0), tempchar,10); 	// 2 Press Meas
		sendtekst(tempchar);
		sendtekst(";");

		itoa(count, tempchar,10); 						// 3 Mux Out 
		sendtekst(tempchar);
		sendtekst(";");

		itoa(KoblingPosRef, tempchar,10);				// 4 Kobling Pos Ref
		sendtekst(tempchar);
		sendtekst(";");

		itoa(KoblingPos, tempchar,10);					// 5 Kobling Pos Meas
		sendtekst(tempchar);
		sendtekst(";");

		itoa(KoblingPosKP*10, tempchar,10);				// 6 Kobling Controller Kp
		sendtekst(tempchar);
	
		sendtekst("\n\r");
}
