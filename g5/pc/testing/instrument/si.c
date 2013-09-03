#include "si.h"

volatile uint8_t reqCmd[10]={0x12,0x34,0x56,0x78,23,8,0,0,0,0};

ISR(USART_RX_vect){
  uint8_t tmp;
  uint8_t* dirtyPointer;
  
  if(!(UCSR0A&_BV(RXC0))){
    return;
  }
	timeOut=0x04;
  PORTD&=~pDCD;
  switch(siCount){
    case 10:
      dirtyPointer=(uint8_t*)&newOilP;
      dirtyPointer[1]=UDR0;
    case 11:
      dirtyPointer=(uint8_t*)&newOilP;
      dirtyPointer[0]=UDR0;
    case 46:
      dirtyPointer=(uint8_t*)&newCLT;
      dirtyPointer[1]=UDR0;
    case 47:
      dirtyPointer=(uint8_t*)&newCLT;
      dirtyPointer[0]=UDR0;
    case 54:
      dirtyPointer=(uint8_t*)&newRPM;
      dirtyPointer[1]=UDR0;
      break;
    case 55:
      dirtyPointer=(uint8_t*)&newRPM;
      dirtyPointer[0]=UDR0;
      break;
    case 62:
      dirtyPointer=(uint8_t*)&newSpeed;
      dirtyPointer[1]=UDR0;
      break;
    case 63:
      dirtyPointer=(uint8_t*)&newSpeed;
      dirtyPointer[0]=UDR0;
      break;
    case 113:  //OUTBITS
      tmp=UDR0;
      flags.newMeasure=true;
      flags.reply=true;
			flags.timeOut=false;
      break;
    default:
      tmp=UDR0;
      break;
  }
  siCount++;
}

void siInit(void){
/*  UBRR0=12; /*19200baud at 4MHz*/
	UBRR0=11; /*19200baud at 3.6864MHz*/
/*  UBRR0=23; /*19200baud at 7.3728MHz*/
  UCSR0B|=_BV(TXEN0)|_BV(RXEN0)|_BV(RXCIE0);  
}

void reqInfo(void){
  uint8_t c;
  
  siCount=0;
  PORTD|=pDCD;
  for(c=0;c<10;c++){
    while(!(UCSR0A&_BV(UDRE0))){
    }
    UDR0=reqCmd[c];
  }
  
  if(flags.timeOut==true && !(PINC&pModeC)){
    
    while(!(UCSR0A&_BV(UDRE0))){
    }
    _delay_ms(2);
    PORTB|=pConfigB;    /*Disable ECU TX, Enable Radio RX/TX*/
    _delay_ms(2);
    UDR0='a';
    while(!(UCSR0A&_BV(UDRE0))){
    }
    _delay_ms(2);
    PORTB&=~pConfigB;   /*Enable ECU TX, Disable Radio RX/TX*/
  }
  
}
