/*********************************************
 * Interrupt-service-rutiner
 *
 * Max er helt ude
 * Min er helt inde
 *
 *********************************************/

#include "config.h"
#include "extern.h"
#include "prototyper.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// ADC
unsigned int ADCconv = 0;
unsigned int pos = GEARPOSMID;
unsigned int posOld = 0;
unsigned int current = 0;
unsigned int currentOld = 0;
char setChannel = 0;
char channel = 0;

// PWM
unsigned int pwmOut = DUTYMAX;

// Gear
unsigned short int gearBut = 0;
unsigned short int gearButCAN = 0;
unsigned short int gearRetning = 0;
unsigned short int gearActive = 0;
unsigned short int released = 1;
unsigned short int maxTilNeutral = 0; 
unsigned short int minTilNeutral = 0;
unsigned int gearPosTargetMax = GEARPOSMAX;
unsigned int gearPosTargetMin = GEARPOSMIN;
unsigned short int gearNeutralMeas = 0;

// Gear estimation
unsigned int roadSpeed_val = 0;
unsigned int rpm_val = 0;
unsigned int gearratio = 0;
unsigned int GearEst_val = 0;
unsigned int gearup_trig = 0;
unsigned int geardown_trig = 0;
unsigned int gear_meas_neutral = 0; // Gear forbi neutral ved skifte ?
unsigned int gear_locked = 0; // Godt gear est fundet ?

// Gear protection
unsigned int overCurrentCounter = 0;
unsigned int errormsg = 0;

// Debugging
char tempchar[10];
unsigned short int i = 0;

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

	// Channel 0 = Current sense	
	if(channel == 0)
	{
		// Current lavpass filter
		current = (int)((double)FILTERKONSTANTCURRENT*ADCconv+(double)(1-FILTERKONSTANTCURRENT)*currentOld);
		currentOld = current;
	}
	// Channel 1 = Pos ADC convert
	if(channel == 1)
	{
		// Giver 0 helt inde og 1024 helt ude (vendes med polaritet på gear pot)
		ADCconv = (ADCconv-1023)*-1;
		pos = ADCconv;
	}
}
// Timer0 (8-bit) overflow interrupt (168 Hz)
ISR(TIMER0_OVF_vect)
{	
	// ADC start new reading
	// Change ADC channel
	ADMUX &= 0xf8;
	ADMUX |= setChannel++;
	//ADMUX |= 1;
	// Start ADC-convert
	ADCSRA|=(1<<ADSC);

	if(setChannel>=ADCtotnum)
		setChannel = 0;

	// PWM
	PWM_duty_cycle_A16_set(pwmOut);

	// Measure gear neutral (1 = i neutral)
	if((gearBut & GEARNEUBUT)==GEARNEUBUT)
		gearNeutralMeas = GEARNEUTRALMEAS;
	else
		gearNeutralMeas = 0;

	// Buttons from CAN
	gearBut = gearButCAN;

	// Gear knapperne skal slippes før der kan skiftes gear igen
	if((gearBut == 0) || (gearBut == GEARNEUBUT))
		released = 1;

	// Paabegynder skifte, hvis der trykkes paa en gearknap (sker ikke hvis et skifter er igang, eller knappen 		ikke har væres sluppet siden sidste skift)
	if( (((gearBut&GEARDOWNBUT)==GEARDOWNBUT) || ((gearBut&GEARUPBUT)==GEARUPBUT)) && (gearActive == 0) && (released == 1))
	{	
		// Gear op (gear arm ud)
		if(((gearBut&GEARUPBUT)==GEARUPBUT) && (maxTilNeutral==0) && (minTilNeutral==0))
		{
			gearActive = 1;
			motorControl(IND, pwmOut);
            IgnCutOn; // Fors¿g med nyt IgnCut
            
			gearPosTargetMax = GEARPOSMAX;
			gearPosTargetMin = GEARPOSMIN;
		}
		// Gear ned (gear arm ind)
		if(((gearBut&GEARDOWNBUT)==GEARDOWNBUT) && (maxTilNeutral==0) && (minTilNeutral==0))
		{
			gearActive = 1;
			motorControl(UD, pwmOut);
            IgnCutOn; // Fors¿g med nyt IgnCut

			gearPosTargetMax = GEARPOSMAX;
			gearPosTargetMin = GEARPOSMIN;
		}
	}
    /* Gammelt IgnCut, cutter alt tid hvor pos er ude af GEARPOSMID
	// Ign_Cut, cut ign via motorstyring, holder outpen ben lavt.
	if((pos>(GEARPOSMID+GearMiddleDeadZone+IGNCUT)) || (pos<(GEARPOSMID-GearMiddleDeadZone-IGNCUT)))
		IgnCutOn;
	else
		IgnCutOff;
    */
    
	// Stop_hvis_over_min/max, eller hvis der maaler neutralgear, set_retur
	if(((pos > gearPosTargetMax) && ((minTilNeutral+maxTilNeutral)==0)) ||
       ((gearNeutralMeas) && ((minTilNeutral+maxTilNeutral)==0)))
	{
		motorControl(0,0);
		maxTilNeutral = 1;
		minTilNeutral = 0;
		gearup_trig = 1;
        IgnCutOff; // Fors¿g med nyt IgnCut
	}
	else if(((pos < gearPosTargetMin) && ((minTilNeutral+maxTilNeutral)==0)) ||
			(gearNeutralMeas && ((minTilNeutral+maxTilNeutral)==0)))
	{
		motorControl(0,0);
		minTilNeutral = 1;
		maxTilNeutral = 0;
		geardown_trig = 1;
        IgnCutOff; // Fors¿g med nyt IgnCut
	}
	// Overcurrent protection
	if(current>GEARFORCECRITICALMAX)
		overCurrentCounter++;

	if((current <= GEARFORCECRITICALMAX) && (overCurrentCounter>0))
		overCurrentCounter--;

	// Overcurrent, vend om
	if((overCurrentCounter>GEARFORCEMAXTIMEOUT1) && (pos>(GEARPOSMID+GearMiddleDeadZone)))
	{	
		errormsg = ERRORMSG_OVERCURRENT;
		minTilNeutral = 0;
		maxTilNeutral = 1;		
	}
	if((overCurrentCounter>GEARFORCEMAXTIMEOUT1) && (pos<(GEARPOSMID-GearMiddleDeadZone)))
	{		
		errormsg = ERRORMSG_OVERCURRENT;
		minTilNeutral = 1;
		maxTilNeutral = 0;
	}
	// Sidder helt fast, vendt 3 sek, og proev igen
	if(overCurrentCounter>GEARFORCEMAXTIMEOUT2)
	{
		errormsg = ERRORMSG_GEARSTUCK;
		hbroEnable(0);
		_delay_ms(3000);
		overCurrentCounter = 0;
		hbroEnable(1);
	}
	// Til Neutral fra max
	if((maxTilNeutral == 1) && (pos>(GEARPOSMID+GearMiddleDeadZone)))
	{
		motorControl(IND, pwmOut);
        IgnCutOn; // Fors¿g med nyt IgnCut
	}
	// Er kommert hjem, reset alt
	else if((maxTilNeutral == 1) && (pos<=(GEARPOSMID+GearMiddleDeadZone)))
	{
		motorControl(0,0);
		minTilNeutral = 0;
		maxTilNeutral = 0;
		gearActive = 0;
		gearButCAN = 0;
		errormsg = ERRORMSG_NOERROR;
        IgnCutOff; // Fors¿g med nyt IgnCut
	}
	// Til Neutral fra min
	if((minTilNeutral == 1) && (pos<(GEARPOSMID-GearMiddleDeadZone)))
	{
		motorControl(UD, pwmOut);
        IgnCutOn; // Fors¿g med nyt IgnCut
	}
	// Er kommert hjem, reset alt
	else if((minTilNeutral == 1) && (pos>=(GEARPOSMID-GearMiddleDeadZone)))
	{
		motorControl(0,0);
		minTilNeutral = 0;
		maxTilNeutral = 0;
		gearActive = 0;
		gearButCAN = 0;
		errormsg = ERRORMSG_NOERROR;
        IgnCutOff; // Fors¿g med nyt IgnCut
	}

	// Gaet paa gear ud fra rpm / roadspeed og skifte fra knapper
	// Gearratio = roadSpeed_val / rpm_val;	
	gear_meas_neutral = GEARNEUTRALMEAS;

	if((gearup_trig == 1) && (gear_meas_neutral == 0))
	{
		gearup_trig = 0;
		GearEst_val++;
		if(GearEst_val>6)
			GearEst_val = 6;
	}
	else if((geardown_trig == 1) && (gear_meas_neutral == 0))
	{
		geardown_trig = 0;
		GearEst_val--;
		if(GearEst_val<1)
			GearEst_val = 1;
	}
	else if((gearup_trig == 1) && (gear_meas_neutral == 1))
	{
		gearup_trig = 0;
		GearEst_val = 1;
		gear_locked = 1; // Vi ved nu hvilket gear bilen er i med rimelig sikkerhed
	}
	else if((geardown_trig == 1) && (gear_meas_neutral == 1))
	{
		geardown_trig = 0;
		GearEst_val = 2;
		gear_locked = 1; // Vi ved nu hvilket gear bilen er i med rimelig sikkerhed
	}
	if(gear_locked == 0)
		GearEst_val = 255; // Vi ved ikke hvilket gear 

	// Update gear gaet med speed / rmp ratio estimat
	//if((gearratio == 100) || 1)
	//	GearEst_val = 1;

	// UART Debugging
	
	if((i%15)==0)
	{	
		sendtekst("GearButCAN: ");
		itoa(gearButCAN,tempchar,2);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("GearBut: ");
		itoa(gearBut,tempchar,2);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("POS: ");
		itoa(pos,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("CS: ");
		itoa(current,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("TAGMAX: ");
		itoa(gearPosTargetMax,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("TAGMIN: ");
		itoa(gearPosTargetMin,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("NEUTRAL: ");
		itoa(gearNeutralMeas,tempchar,2);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("ERRMSG: ");
		itoa(errormsg,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\t");

		sendtekst("Gearratio: ");
		itoa(roadSpeed_val,tempchar,10);
		sendtekst(tempchar);
		sendtekst("/");
		itoa(rpm_val,tempchar,10);
		sendtekst(tempchar);
		sendtekst(" = ");
		itoa(gearratio,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\n\r");

		i = 0;
	}
	i++;
}
