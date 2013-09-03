/*********************************************
 * Interrupt-service-rutiner
 *********************************************/

#include <config.h>
#include <extern.h>
#include <prototyper.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// ADC
unsigned int ADCconv = 0;
unsigned int pos = 0;
unsigned int posOld = 0;
unsigned int current = 0;
unsigned int overCurrentCounter = 0;
char setChannel = 0;
char channel = 0;
unsigned short int maxTilNeutral = 0; 
unsigned short int minTilNeutral = 0;

// PWM
unsigned short int pwmValue = 0;

// Gear
volatile unsigned short int gearRetning = 0;
volatile unsigned int gearPosTargetUp = GEARPOSMIDDLE;
volatile unsigned int gearPosTargetDown = GEARPOSMIDDLE;
unsigned int gearStock = 0;

// UART transmit fifoRingBufffer
unsigned short int uartFifoBuff[UARTQUEUESIZE];
unsigned short int uartQueueNumber = 0;
unsigned short int uartQueueEnd = 0;
unsigned short int uartQueueStart = 0;

// Temp Var
char tempchar[5];
unsigned int debug = 0;

// ADC convert complete
ISR(ADC_vect)
{
	unsigned int adlow = 0;
	unsigned int adhigh = 0;
		
	// Read ADC channel
	channel = ADMUX & 0x07;

	// Read ADC convertion
    adlow=ADCL;
    adhigh=ADCH;
	ADCconv = (unsigned int)((adhigh<<8)|(adlow & 0xFF));

	// Channel 0 = Pos ADC convert
	if(channel == 0)
	{
		// Pos lav pass filter (for at undga at stoj oedelaegger skiftet)
		pos = (int)((double)FILTERKONSTANTPOS*ADCconv+(double)(1-FILTERKONSTANTPOS)*posOld);
		posOld = pos;
	}
	// Channel 1 = Current ADC convert
	if(channel == 1)
	{
		current = ADCconv;
	}
}

 // UART0 interrupt
ISR(USART0_RX_vect)
{
	char data;
	data = UDR0;

	if(data == 'w')
	{
		motorControl(CW, 150, 748);
		_delay_ms(100);
		AOFF;
		BOFF;
	}

	if(data == 's')
	{
		motorControl(CCW, 150, 748);
		_delay_ms(100);
		AOFF;
		BOFF;
	}

	//if(data == 't')
	//	uartTransmitQueue(testTransmitt,&uartQueueNumber,&uartQueueEnd,uartFifoBuff);
}
/*
ISR(USART0_TX_vect)
{
	// Transmit Complete Interrupt, eksekveres nAr TXCn flaget saettes
	// Dette flag cleares automatisk af int rutinen

	if(test == 0)
	{
		UDR0 = uartFifoBuff[uartNextSlot];
		test++;
	}
}
*/
// Timer0 (8-bit) overflow interrupt
ISR(TIMER0_OVF_vect)
{	
	//______________________ADC_______________________________________________
	// Change ADC channel
	ADMUX &= 0xf8;
	ADMUX |= setChannel++;
	// Start ADC-convert
	ADCSRA|=(1<<ADSC);
	
	if(setChannel>=ADCtotnum)
		setChannel = 0;

	//______________________Ign_Cut___________________________________________
	if((pos>(GEARPOSMIDDLE+GearMiddleDeadZone+IGNCUT)) || (pos<(GEARPOSMIDDLE-GearMiddleDeadZone-IGNCUT)))
	{
		IgnCutOn;
		LEDYellowOn;
	}
	else
	{
		IgnCutOff;
		LEDYellowOff;
	}

	//______________________Aktuator_Moment_Regulering________________________

	// Constant torque controller
	if(channel==1)
	{
		pwmValue = torqueController(current);
	}

	//______________________Retur_fra_min/max_________________________________
	// Til Neutral fra max
	if((maxTilNeutral == 1) && (pos>(GEARPOSMIDDLE+GearMiddleDeadZone)))
	{
		motorControl(CCW, pwmValue, pos);
	}
	else if((maxTilNeutral == 1) && (pos<=(GEARPOSMIDDLE+GearMiddleDeadZone)))
	{
		motorControl(0,0,0);
		minTilNeutral = 0;
		maxTilNeutral = 0;
	}

	// Til Neutral fra min
	if((minTilNeutral == 1) && (pos<(GEARPOSMIDDLE-GearMiddleDeadZone)))
	{
		motorControl(CW, pwmValue, pos);
	}
	else if((minTilNeutral == 1) && (pos>=(GEARPOSMIDDLE-GearMiddleDeadZone)))
	{
		motorControl(0,0,0);
		minTilNeutral = 0;
		maxTilNeutral = 0;
	}

	//______________________Stop_hvis_over_min/max,_set_retur_________________
	if((pos > gearPosTargetUp) && ((minTilNeutral+maxTilNeutral)==0))
	{
		AOFF;
		maxTilNeutral = 1;
		minTilNeutral = 0;
	}
	else if((pos < gearPosTargetDown) && ((minTilNeutral+maxTilNeutral)==0))
	{
		BOFF;
		minTilNeutral = 1;
		maxTilNeutral = 0;
	}

	//______________________Gear_stock_moitor_________________________________

	if((pos<(GEARPOSMIDDLE-GearMiddleDeadZone-2)) || (pos>(GEARPOSMIDDLE+GearMiddleDeadZone+2)))
		gearStock++;
	else
		gearStock = 0;

	if((gearStock>GEARSTOCKTIMEOUT1) && (pos>(GEARPOSMIDDLE+GearMiddleDeadZone+2)))
	{	
		sendtekst("\n\r SimiStock ! \n\r");
		minTilNeutral = 0;
		maxTilNeutral = 1;
	}
	if((gearStock>GEARSTOCKTIMEOUT1) && (pos<(GEARPOSMIDDLE-GearMiddleDeadZone-2)))
	{		
		sendtekst("\n\r SimiStock ! \n\r");
		minTilNeutral = 1;
		maxTilNeutral = 0;
	}
	if(gearStock>GEARSTOCKTIMEOUT2)
	{
		cli();
		hbroEnable(0);
		LEDRed1On;
		gearStock = 0; // For at undga int overflow
	}

	//______________________OverCurrent sensor (force)________________________
	
	if(current>GEARFORCECRITICALMAX)
		overCurrentCounter++;

	if((current <= GEARFORCECRITICALMAX) && (overCurrentCounter>0))
		overCurrentCounter--;

	if((overCurrentCounter>GEARFORCEMAXTIMEOUT1) && (pos>(GEARPOSMIDDLE+GearMiddleDeadZone)))
	{	
		sendtekst("\n\r SimiCurrent ! \n\r");
		minTilNeutral = 0;
		maxTilNeutral = 1;
	}
	if((overCurrentCounter>GEARFORCEMAXTIMEOUT1) && (pos<(GEARPOSMIDDLE-GearMiddleDeadZone)))
	{		
		sendtekst("\n\r SimiCurrent ! \n\r");
		minTilNeutral = 1;
		maxTilNeutral = 0;
	}

	if(overCurrentCounter>GEARFORCEMAXTIMEOUT2) // 70 = ~ 0.5 sec
	{
		hbroEnable(0);
		LEDRed2On;
	}

	// Debugging
	debug++;
	if((debug % 20 == 0))
	{
		itoa(pos, tempchar, 10); 
		sendtekst(tempchar);
		sendtekst(";");
		itoa(current, tempchar, 10); 
		sendtekst(tempchar);
		sendtekst(";");
		itoa(pwmValue, tempchar, 10); 
		sendtekst(tempchar);
		sendtekst("\n\r");
		debug = 0;
	}
}

ISR(PCINT2_vect)
{
	// Gear op kontakt (tjekker at armen ikke er pa vej tilbage)
	if((gearRetning == GEAROP) && ((minTilNeutral+maxTilNeutral)==0))
	{
		gearRetning = 0;
		motorControl(CW, pwmValue, pos);
	}
	// Gear ned kontakt
	if((gearRetning == GEARNED) && ((minTilNeutral+maxTilNeutral)==0))
	{
		gearRetning = 0;
		motorControl(CCW, pwmValue, pos);
	}

	gearRetning = 0;
}
