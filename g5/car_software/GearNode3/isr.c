/*********************************************
 * Interrupt service rutine
 *
 * Max is furthest away
 * Min is closest
 *
 *********************************************/

#include "config.h"
#include "extern.h"
#include "prototyper.h"
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "debug.h"

unsigned int ADCconv = 0;                                       // ADC value

// Debugging
char tempchar[10];
unsigned short int count = 0;

// ADC convert complete interrupt
ISR(ADC_vect,ISR_NOBLOCK)
{
	unsigned int adlow = 0;
	unsigned int adhigh = 0;

    gearPositionOld = gearPosition;
    
	// Read the ADC data register (By default the ADC is RIGHT AJUSTED)
    adlow = ADCL;                                                       // ADCL is the least significant byte in the ADC data register
    adhigh = ADCH;                                                      // ADSH is the most significant byte in the ADC data register
	ADCconv = (unsigned int)((adhigh<<8)|(adlow & 0xFF));               // Assemble the two ADC data registers to a 10 bit value (ADCconv = ADCH + ADCL)
    
    gearPosition = ADCconv;
}

/*
    - Timer0 (8-bit) is an overflow interrupt (168 Hz)
    - GEARDOWNBUT and GEARUPBUT is switched for now (Date: 08-10-2013)
    - Main purpose is to control the gear position and check for different states
    - WARNING: The ISR contains sleeping functions (gearNeutral1() and gearNeutral2())(Date: 08-10-2013) 
*/
ISR(TIMER0_OVF_vect)
{
    gearButActive = 0;
    //servoCheck();
    gearBut = gearButCAN;
    gearButNeuMeas = GEARNEUTRALMEAS;
    
    // Conditions for when to set the estimated gear value up or down
    if((gearBut == GEARDOWNBUT) && (gearButNeuMeas == 0)){
        if(GearEst_val < 6){
            GearEst_val++;
        }
    }
    else if((gearBut == GEARUPBUT) && (gearButNeuMeas == 0)){
        if(GearEst_val > 1){
            GearEst_val--;
        }
    }
    // Conditions for when to gear down to 1 or up to gear 2 from neutral gear
    else if((gearBut == GEARUPBUT) && (gearButNeuMeas == 1)){
        GearEst_val = 1;
    }
    else if((gearBut == GEARDOWNBUT) && (gearButNeuMeas == 1)){
        GearEst_val = 2;
    }
    
    //  Check if the gear should gear up half a gear from 1 to neutral
    if(gearButActive == 0 && (gearBut == GEARNEUBUT1)){
        gearButActive = 1;
        gearNeutral1();
        GearEst_val = 0;
    }
    //  Check if the gear should gear down half a gear from 2 to neutral
    else if(gearButActive == 0 && (gearBut == GEARNEUBUT2)){
        gearButActive = 1;
        gearNeutral2();
        GearEst_val = 0;
    }
    //  Check if the gear it should gear down
    else if(gearButActive == 0 && gearBut == GEARUPBUT){
        gearButActive = 1;
        //sendtekst("1");
        gearDown();
    }
    //  Check if the gear it should gear up
    else if(gearButActive == 0 && gearBut == GEARDOWNBUT){
        gearButActive = 1;
        //sendtekst("3");
        gearUp();
    }
    
   
    
	if((count%50)==0)
	{

        /* Data til leg med gear knapper */
        /*
        sendtekst("gearBut2: ");
		itoa(gearBut,tempchar,2);
		sendtekst(tempchar);
		sendtekst("\t");
        
        sendtekst("gearBut10: ");
		itoa(gearBut,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\t");
        */
        
        sendtekst("gearNeutralMeas: ");
		itoa(GEARNEUTRALMEAS,tempchar,10);
		sendtekst(tempchar);
		sendtekst("\t");
            
        sendtekst("gearButCAN: ");
		itoa(gearButCAN,tempchar,2);
		sendtekst(tempchar);
		sendtekst("\t");
        
        sendtekst("gearButActive: ");
		itoa(gearButActive,tempchar,2);
		sendtekst(tempchar);
		sendtekst("\r\n");
        
        /* Data til leg med gear positioner */
		//sendtekst("Pos: ");
		//itoa(gearPosition,tempchar,10);
		//sendtekst(tempchar);
		//sendtekst("\t");
        
        //sendtekst("PosOld: ");
		//itoa(gearPositionOld,tempchar,10);
		//sendtekst(tempchar);
		//sendtekst("\t");
        //sendtekst("/");
        //sendtekst("GotoPos: ");
		//itoa(gearGotoPosition,tempchar,10);
		//sendtekst(tempchar);
		//sendtekst("\t");
        
        //sendtekst("GearActive: ");
		//itoa(gearActive,tempchar,10);
		//sendtekst(tempchar);
		//sendtekst("\t");
        //sendtekst(" - ");
        //sendtekst("GearCounter: ");
		//itoa(gearCounter,tempchar,10);
		//sendtekst(tempchar);
		//sendtekst("\r\n");
		//servoPosition(MIDPOS);

		count = 0;
	}
	count++;
    
    // Is only down here because we want to print
    gearButCAN = 0;
}
