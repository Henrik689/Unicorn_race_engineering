#include "instrument.h"

void init(void){
  DDRB=0xFF;  /*1=output; 0=input*/
  PORTB=0x00;
  
  DDRC=0xFF&~(pNeutralIN|pModeC);
  PORTC=0x00|pNeutralIN|pModeC;
  
  DDRD=0xFF&~(1<<0|pUpD|pDownD|1<<5); //RX|INT0|INT1|PCINT21 as inputs
  PORTD=0x00|(1<<0|pUpD|pDownD|1<<5); //RX|INT0|INT1|PCINT21 with pullup

  timeDiv=2;

  display(180,4,3);

/*  OSCCAL=0xb4;*/
  
  siInit();
  timerInit();
  gearInit();
/*  dectInit();*/
  
  CLKPR=_BV(CLKPCE);  /*Allow prescaler change*/
  CLKPR=0x01;         /*Prescale by two (CLK=3.6864MHz)*/
  
  PORTC&=~pResetC;
  _delay_ms(200);
  PORTC|=pResetC;
  _delay_ms(200);
  
  flags.online=false;
  radioInit();
  radioInit2();
  PORTD&=~pConfigD;
  _delay_ms(200);
  if(PCIFR&_BV(PCIF2)){
		PCIFR|=_BV(PCIF2);
	}
  sei();
}

int main(void){
  uint8_t gear, rpm, warnings;
	uint16_t clt, oilP, speed;
  
  init();
  
  gear=0;
  rpm=0;
  speed=0;
  warnings=0;
	clt=0;
	oilP=0;
  
  while(1){
    gear=0;
    if(flags.newMeasure){
      flags.newMeasure=false;
			clt=newCLT;
			oilP=newOilP;
      rpm=(newRPM>>6);
      speed=newSpeed;
      
    }
	  
    if(flags.refresh){
      flags.refresh=false;
      
    	gear=calcGear(speed,rpm);
			
    	warnings=0;
      if(clt>cltCold){
        warnings|=0x01*((timeDiv&0x20)!=0);
      }else if(clt<cltStop){
        warnings|=0x01*((timeDiv&0x02)!=0);
      }else if(clt<cltWarn){
        warnings|=0x01;
      }else{
        warnings&=(~0x01);
      }
//      if(oilP<40){
			if(flags.timeOut==true){
        warnings|=0x02;
      }else{
        warnings&=(~0x02);
      }
      
      if(PINC&pModeC){
        flags.online=false;
        PORTD&=~pConfigD;
        warnings=0x01*((timeDiv&0x04)!=0);
        warnings|=0x02*((timeDiv&0x04)==0);
      }
      
      display(rpm,gear,warnings);
    }
    
    if(flags.reqInfo){
      flags.reqInfo=false;
      reqInfo();
    }
  
  }
  
}
