#include "gear.h"

void gearInit(void){
  EICRA=0x0a; //0b00001010 = falling edge
}

uint8_t calcGear(uint16_t speed, uint8_t rpm){

  uint8_t ratio, shift;
  static uint8_t gear, gearTmp, tOffset, shiftUp, shiftDown, clutchCount;
  
  ratio=0;
  
  if(((timeDiv-tOffset)&0x03)==0){
  
    if(rpm>0){
      ratio=speed/rpm;
    }
   
    if(ratio>48 && ratio<53){
      if(gearTmp==1){
        gear=1;
      }
      gearTmp=1;
    }else if(ratio>67 && ratio<72){
      if(gearTmp==2){
        gear=2;
      }
      gearTmp=2;
    }else if(ratio>82 && ratio<86){
      if(gearTmp==3){
        gear=3;
      }
      gearTmp=3;
    }else if(ratio>93 && ratio<97){
      if(gearTmp==4){
        gear=4;
      }
      gearTmp=4;
    }else if(ratio>100 && ratio<104){
      if(gearTmp==5){
        gear=5;
      }
      gearTmp=5;
    }else if(ratio>109 && ratio<113){
      if(gearTmp==6){
        gear=6;
      }
      gearTmp=6;
    }else{
      ratio=0;
      if(gear!=7){
        clutchCount++;
      }
      if(clutchCount>5){
        clutchCount=6;
        gear=0;
      }
      gearTmp=0;
    }
    if(ratio!=0){
      clutchCount=0;
    }
  }
  
  if(ratio==0 && gear!=0){
    switch(PIND&(pUpD|pDownD)){
    case 0x00:
      shiftDown=0;
      shiftUp=0;
      break;
    case pDownD:
      shiftUp=0;
      if(shiftDown<11){
        shiftDown++;
      }
      if(shiftDown==2){
        tOffset=timeDiv;
        clutchCount=0;
        if(gear==7){
          gear=1;
        }else if(gear>1){
          gear--;
        }
      }
      break;
    case pUpD:
      shiftDown=0;
      if(shiftUp<11){
        shiftUp++;
      }
      if(shiftUp==2){
        tOffset=timeDiv;
        clutchCount=0;
        if(gear==7){
          gear=2;
        }else if(gear<6){
          gear++;
        }
      }
      break;
    case pUpD|pDownD:
      shiftUp=0;
      shiftDown=0;
      break;
    }
  }
  
  shift=shiftUp|shiftDown;
  
  if((PINC&pNeutralIN)==0){
    if((shift<1)|(shift>10)){
      tOffset=timeDiv;
      gear=7;
      clutchCount=0;
    }
  }else if(gear==7){
    if((shift<1)|(shift>10)){
//      tOffset=timeDiv;
      gear=0;
    }
//    gear=0;
  }
  
  return gear;
}
