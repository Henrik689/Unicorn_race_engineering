#include "display.h"

void display(uint8_t rpm, uint8_t gear, uint8_t flags){
  display_t dispData;
  int8_t c;
  uint8_t gearData[7]={0,2,3,7,15,13,12};
  uint8_t rpmScale[6]={6000/64,7000/64,8000/64,9000/64,10000/64,11000/64};
  
  dispData.rpm=0;
  
  for(c=0;c<6;c++){
    if(rpm>rpmScale[c]){
      dispData.rpm|=_BV(c);
    }
  }
  
  if(rpm>(11000/65)){
    dispData.rpm=dispData.rpm*((timeDiv&0x02)!=0);
  }
  
  if(gear==7){
    dispData.gear=gearData[4]*((timeDiv&0x04)!=0);
  }else{
    dispData.gear=gearData[gear];
  }
  
  dispData.flags=(uint8_t)~flags;
  
  PORTB&=~pStr;               //disable strobe
  _delay_us(displayDelay);

/*  for(c=4;c>0;c--){
    //SPARE OUTPUT
    _delay_us(displayDelay);
    PORTB|=pClock;
    _delay_us(displayDelay);
    PORTB&=~pClock;
  }*/

  for(c=4;c>0;c--){
    if(dispData.gear&_BV(c-1)){
      PORTB|=pData;
    }else{
      PORTB&=~pData;
    }
    _delay_us(displayDelay);
    PORTB|=pClock;
    _delay_us(displayDelay);
    PORTB&=~pClock;
  }

  for(c=2;c>0;c--){
    if(dispData.flags&_BV(c-1)){
      PORTB&=~pData;
    }else{
      PORTB|=pData;
    }
    _delay_us(displayDelay);
    PORTB|=pClock;
    _delay_us(displayDelay);
    PORTB&=~pClock;
  }
  
  for(c=6;c>0;c--){
    if(dispData.rpm&_BV(c-1)){
      PORTB|=pData;
    }else{
      PORTB&=~pData;
    }
    _delay_us(displayDelay);
    PORTB|=pClock;
    _delay_us(displayDelay);
    PORTB&=~pClock;
  }

  _delay_us(displayDelay);
  PORTB|=pStr;                //enable strobe
  
}
