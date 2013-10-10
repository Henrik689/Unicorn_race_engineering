/*********************************************
 * Interrupt-service-rutiner
 *
 * Max er helt ude
 * Min er helt inde
 *
 *********************************************/

#include <stdlib.h>
#include "config.h" // F_CPU is defined here and must be defined before util/delay.h is included 
#include "extern.h"
#include "functions.h"

// ADC
unsigned int ADCconv = 0;

// Debugging
char tempchar[10];
unsigned short int count = 0;

// ADC convert complete
ISR(ADC_vect,ISR_NOBLOCK)
{
	unsigned int adlow = 0;
	unsigned int adhigh = 0;

    gearPositionOld = gearPosition;
    
	// Read ADC convertion
    adlow=ADCL;
    adhigh=ADCH;
	ADCconv = (unsigned int)((adhigh<<8)|(adlow & 0xFF));
    //ADCconv = (ADCconv-1023)*-1; //Hvis det skal vendes
    gearPosition = ADCconv;
}

// Timer0 (8-bit) overflow interrupt (168 Hz)
ISR(TIMER0_OVF_vect)
{
    gearButActive = 0;
    //servoCheck();
    gearBut = gearButCAN;
    gearButNeuMeas = GEARNEUTRALMEAS;
    
    if((gearBut == GEARDOWNBUT) && (gearButNeuMeas == 0)){
        if(GearEst_val < 6){
            GearEst_val++;
        }
    }
    else if((gearBut == GEARUPBUT) && (gearButNeuMeas == 0)){
        if(GearEst_val>1){
            GearEst_val--;
        }
    }
    else if((gearBut == GEARUPBUT) && (gearButNeuMeas == 1)){
        GearEst_val = 1;
    }
    else if((gearBut == GEARDOWNBUT) && (gearButNeuMeas == 1)){
        GearEst_val = 2;
    }
    
    
    if(gearButActive == 0 && (gearBut == GEARNEUBUT1)){
        gearButActive = 1;
        gearNeutral1();
        GearEst_val = 0;
    }
    else if(gearButActive == 0 && (gearBut == GEARNEUBUT2)){
        gearButActive = 1;
        gearNeutral2();
        GearEst_val = 0;
    }
    else if(gearButActive == 0 && gearBut == GEARUPBUT){
        gearButActive = 1;
        sendtekst("1");
        gearDown();
    }
    else if(gearButActive == 0 && gearBut == GEARDOWNBUT){
        gearButActive = 1;
        sendtekst("3");
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
