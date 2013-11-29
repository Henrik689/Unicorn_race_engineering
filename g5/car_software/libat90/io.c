/**
* @file io.c
* @brief 
*	Used for simple digital pin IO operations
* @details
*	Provides functions to read and write 
*	digital values from specified pin
*	as well as pin setup modes.
*/

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/sfr_defs.h> // bit_is_clear()


#include "bitwise.h"
#include "io.h"

/** 
* @brief 
* 	Read the digital value of a given pin
*
* @details
* 	This function is used to see if a pin is currently
* 	either HIGH or LOW. If we want to do something
* 	if pin 5 on port A is high we would do the following
* 	example: \n
* 	if( digitalRead(&PINA, PA5) == HIGH ) { DoSomething() }
*
* @note
*	The pin must be set to the appropriate mode
* 	(typically this is either INPUT or INPUT_PULLUP)
*	using the function pinMode() before use 
*	
*
* @param[in] inputPinRegister
* 	Pointer to the pin register that the pin is
* 	located on.
*
* @param[in] pin
* 	The pin to read from
*
* @return
*	Either HIGH or LOW depending on the pin reading
* 	where HIGH is 1 and LOW is 0
*
*/
int digitalRead(volatile uint8_t *inputPinRegister, int pin){
	return bit_is_clear(inputPinRegister, pin) != 0 ? HIGH : LOW;
}

/**
* @brief
*	Write a digital value to a specified port pin
*
* @note
*	The pin must be set to the appropriate mode
*	(typically this is OUTPUT) using pinMode()
*	before use
*
* @param[in] port
*	The port the pin is located on
* @param[in] pin
*	The	pin to write to
* @param[in] value
*	The value (HIGH or LOW) to write
*/
void digitalWrite(volatile uint8_t *port, int pin, int value){
	if (value == LOW) {
		BIT_CLEAR(*port, pin);
	} else {
		BIT_SET(*port, pin);
	} 
}

/**
* @brief
*	if PORTA is provided DDRB will be returned
*
* @details
* 	Returns a pointer to the DDR register for the
* 	provided port. So if we have PORTA it will return DDRA
*
* @note
*	This should only be used if the resulting DDR register should
*	be assigned to a variable. Else use the macro DDR_PORT
*
* @param[in] port
*	PORTA through PORTF
*
* @return
*	the corresponding DDRx
*/
static inline volatile uint8_t* getDDRXFromPORTX(volatile uint8_t* port){
	volatile uint8_t *ddr_port = NULL;
	if (*port == PORTA){
		ddr_port = &DDRA;
	} else if (*port == PORTB){
		ddr_port = &DDRB;
	} else if (*port == PORTC){
		ddr_port = &DDRC;
	} else if (*port == PORTD){
		ddr_port = &DDRD;
	} else if (*port == PORTE){
		ddr_port = &DDRE;
	} else if (*port == PORTF){
		ddr_port = &DDRF;
	} else if (*port == PORTG){
		ddr_port = &DDRG;
	}
	return ddr_port;
}

/**
* @brief
*	Set the pin to INPUT, OUTPUT or INPUT_PULLUP
*
* @param[in] port volatile uint8_t
*	The port that the pin in located on
* @param[in] pin int
*	The pin to set the mode on
* @param[in] mode enum io_pinmode_t
*	The mode that the pin will be set to
*
* @return
*	1 if error \n
*	0 on success
*/
int pinMode(volatile uint8_t *port, int pin, enum io_pinmode_t mode){
	//volatile uint8_t *ddr_port = getDDRXFromPORTX(port);
	//if( ddr_port == NULL ) return 1;

	if(mode == OUTPUT){
		//BIT_SET(*ddr_port, pin);
		BIT_SET(DDR_PORT(*port), pin);
	} else if((mode == INPUT) || (mode == INPUT_PULLUP)){
		//BIT_CLEAR(*ddr_port, pin);
		BIT_SET(DDR_PORT(*port), pin);
		if(mode == INPUT_PULLUP)  BIT_SET(*port, pin);
	}

	return 0;
}

