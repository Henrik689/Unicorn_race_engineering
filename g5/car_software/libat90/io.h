/**
* @file io.h
* @brief 
*	Used for simple digital pin IO operations
*/

#ifndef IO_H
#define IO_H

#include <stdint.h>
#include "bitwise.h"

#define DDR_PORT(port)	(*(&(port) - 0x01)) //!< Maps a given PORT to the corresponding DDR eg. PORTA -> DDRA. The magic number 0x01 is the offset from the PORT register to the DDR register
#define PIN_PORT(port)	(*(&(port) - 0x02)) //!< Maps a given PORT to the corresponding PIN eg. PORTA -> PINA. The magic number 0x02 is the offset from the PORT register to the PIN register


/**
* @brief
*	Represents the two possible
*	digital values
*/
enum io_digital_t {
	LOW 	= 0,
	HIGH 	= 1
};

/**
* @brief
*	Different modes that the pins
*	can be set to
*/
enum io_pinmode_t {
	INPUT, 			//!< Useful for reading a sensor but not powering an LED, You would most likely want the INPUT pin to reference GND
	OUTPUT, 		//!< Sets the pin in a low impedance state. Thus the pin can power a LED but not read a sensor
	INPUT_PULLUP 	//!< Use the internal pull-up resistors. This effectively inverts its behavior, where HIGH means off and LOW means on
};

#define DIGITAL_READ(port, pin)			( BIT_CHECK(_SFR_BYTE(PIN_PORT((port))), (pin)) != 0 ? HIGH : LOW 	) //!< Same as digitalRead
#define DIGITAL_WRITE(port, pin, value)	( BITMASK_SET_OR_CLEAR(_SFR_BYTE((port)), (1 << (pin)), (value)) 	) //!< Same as digitalWrite
#define DIGITAL_TOGGLE(port, pin)		( DIGITAL_WRITE((port), (pin), !DIGITAL_READ((port), (pin))) 		) //!< Toggles a pin

enum io_digital_t digitalRead(volatile uint8_t *inputPinRegister, int pin);
void digitalWrite(volatile uint8_t *port, int pin, enum io_digital_t value);

int pinMode(volatile uint8_t *port, int pin, enum io_pinmode_t mode);


#endif /* IO_H */
