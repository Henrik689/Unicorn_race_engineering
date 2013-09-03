#include "timer.h"

void timerInit(void){
  TCCR2B=0x07; /*prescaler=1024*/
  TIMSK2|=_BV(TOIE2);
  timeDiv=0;
  timeOut=0x0f;
  preScaler=0;
	radioTimeout=0x21;
}

ISR(TIMER2_OVF_vect){

/*
F_CPU=4MHz, Timer2 prescaler=1024 -> 3906Hz
*/

  TCNT2|=0x40;
  
  flags.refresh=true;
  timeDiv++;
  
  if(timeOut>0){
    timeOut--;
  }
  
	if(timeOut==0){
		flags.timeOut=true;
		flags.reqInfo=true;
    timeOut=0x0f;
	}
	
	if(flags.reply==true){
		flags.reply=false;
		flags.reqInfo=true;
    timeOut=0x0f;
		if(flags.forward && flags.online){
      PORTD&=~pConfigD;
      flags.forward=false;
    }else{
      PORTD|=pConfigD;
      flags.forward=true;
    }
	}  //Test
		if(flags.online==false && !(PINC&pModeC)){
			radioTimeout++;
			switch(radioTimeout){
				case 1:
					PORTC&=~pResetC;
					break;
				case 10:
					PORTC|=pResetC;
					break;
				case 20:
					radioInit();
					break;
				case 21:
					radioInit2();
					break;
				case 0xf1:
					radioTimeout=0;
					break;
				default:
					if(PCIFR&_BV(PCIF2)){
						PCIFR|=_BV(PCIF2);
						if(radioTimeout>25){
              flags.online=true;
            }
					}
					break;
			}
		}else if(PCIFR&_BV(PCIF2)){
		  preScaler++;
		  if(preScaler==4){
        flags.online=false;
      }
			PCIFR|=_BV(PCIF2);
			radioTimeout=0x21;
		}else{
      preScaler=0;
    }
//	}
}
