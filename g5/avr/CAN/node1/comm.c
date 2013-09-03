#include "config.h"
#include "uart.h"
#include "comm.h"
#include "ecu.h"
#include "error.h"
#include "adc.h"

unsigned short int gearneutral = 0;
unsigned short int oilpress = 0;


void send_status( void )
{
	unsigned int tmp=0;
	USART1_Transmit((U8)123);	// Pakke start
	USART1_Transmit((U8)255);
	USART1_Transmit((U8)10);
	USART1_Transmit((U8)EcuCommError);	// Ecu error var
	USART1_Transmit((U8)EcuData[RPMSTART]);		// Send RPM
	USART1_Transmit((U8)EcuData[RPMSTART+RPMLENGTH-1]);
	USART1_Transmit((U8)EcuData[SPEEDSTART]);	// Send Hastighed
	USART1_Transmit((U8)EcuData[SPEEDSTART+SPEEDLENGTH-1]);
	USART1_Transmit((U8)EcuData[46]);	// Vand temp
	USART1_Transmit((U8)EcuData[47]);
	USART1_Transmit((U8)EcuData[48]);	// Luft temp
	USART1_Transmit((U8)EcuData[49]);
	USART1_Transmit((U8)EcuData[66]);	// Batt V
	USART1_Transmit((U8)EcuData[67]);		
	USART1_Transmit((U8)EcuData[POTSTART]);	// Pot 
	USART1_Transmit((U8)EcuData[POTSTART+POTLENGTH-1]);		
	USART1_Transmit((U8)EcuData[MAPSTART]);	// Map 
	USART1_Transmit((U8)EcuData[MAPSTART+MAPLENGTH-1]);	
	gearneutral = convertanalog(3); // GEAR-neutral
	USART1_Transmit((U8)(gearneutral>>2) & 0xff);
	oilpress = convertanalog(7); //Olie tryk (on/off)
	USART1_Transmit((U8)(oilpress>>2) & 0xff);	
	tmp = convertanalog(1); // Vand temp efter koeler
	USART1_Transmit((U8)(tmp>>2) & 0xff);	
	tmp = convertanalog(5); // Olie temp
	USART1_Transmit((U8)(tmp>>2) & 0xff);
	USART1_Transmit((U8)EcuData[96]);	// gX (acc)
	USART1_Transmit((U8)EcuData[97]);		
	USART1_Transmit((U8)EcuData[98]);	// gy (acc)
	USART1_Transmit((U8)EcuData[99]);		
	USART1_Transmit((U8)EcuData[100]);	// gz (acc)
	USART1_Transmit((U8)EcuData[101]);		
	USART1_Transmit((U8)EcuData[68]);	// lambda
	USART1_Transmit((U8)EcuData[69]);		
	USART1_Transmit((U8)EcuData[78]);	// injectorTime
	USART1_Transmit((U8)EcuData[79]);		
	USART1_Transmit((U8)EcuData[82]);	// ignitionTime
	USART1_Transmit((U8)EcuData[83]);		
	USART1_Transmit((U8)EcuData[74]);	// engine load
	USART1_Transmit((U8)EcuData[75]);		
	USART1_Transmit((U8)EcuData[56]);	// trigger error
	USART1_Transmit((U8)EcuData[57]);		

}
