#include "config.h"
#include "can_std/can_lib.h"
#include "can_func.h"
#include "../lib/can_defs.h"
#include "uart.h"
#include "ecu.h"
#include "error.h"

int RecIndex = 0;	// Bruges til at tælle hvor mange bytes der er modtaget fra ECU'en
int testvar = 0;	// Tmp var for at køre TIMER0_COMP_vect langsommere

/* Funktion der sender data req til ECU */
ISR(TIMER0_COMP_vect)
{
	int i;
	testvar++;
	if (testvar == 4) {
		if (RecIndex != 114 && EcuErrorTmp < 5) { // Test for fejl
			EcuErrorTmp++;
			if (EcuErrorTmp == 5) {
				EcuCommError = 1;
			}
		} else if (EcuErrorTmp > 0 && RecIndex == 114) {
			EcuErrorTmp--;
			EcuCommError = 0;
		}
		testvar=0;
		RecIndex = 0;
		for (i=0;i<=9;i++) {		// Send data req streng
			USART0_Transmit(ecu_data[i]); 
		}
	}
}

/* Funktion til at læse data fra ECU */
ISR(USART0_RX_vect)
{
	if (RecIndex <= 114) {			// Der skal modtages 228 bytes fra ECU'en
		EcuData[RecIndex] = UDR0;	// Gem data fra ECU
		RecIndex++;
	}
}
