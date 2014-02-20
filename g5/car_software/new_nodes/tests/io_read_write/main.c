#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <io.h>
#include <uart.h>

#define NUM_PINS	(8)

int main(void) {
	uart0_init();
	uart1_init();								//Serial communication

	sei();										//Enable interrupt

	uart1_printf("\n\n\nSTARTING\n");

	SET_PIN_MODE(PORTF, PIN0, OUTPUT);
	SET_PIN_MODE(PORTF, PIN1, OUTPUT);
	SET_PIN_MODE(PORTF, PIN2, OUTPUT);
	SET_PIN_MODE(PORTF, PIN3, OUTPUT);
	SET_PIN_MODE(PORTF, PIN4, OUTPUT);
	SET_PIN_MODE(PORTF, PIN5, OUTPUT);
	SET_PIN_MODE(PORTF, PIN6, OUTPUT);
	SET_PIN_MODE(PORTF, PIN7, OUTPUT);

	DIGITAL_WRITE(PORTF, PIN0, HIGH); 	// takes 6 instruktions
	IO_SET_HIGH(PORTF, PIN1); 			// takes 1 instruktion
	DIGITAL_WRITE(PORTF, PIN2, HIGH);
	DIGITAL_WRITE(PORTF, PIN3, HIGH);
	DIGITAL_WRITE(PORTF, PIN4, HIGH);
	DIGITAL_WRITE(PORTF, PIN5, HIGH);
	DIGITAL_WRITE(PORTF, PIN6, HIGH);
	IO_SET_HIGH(PORTF, PIN7);

	int i = 0;
	while(1){
		// Main work loop
		_delay_ms(250*4); // 1 second

		DIGITAL_TOGGLE(PORTF, i); // This expands to 6 instruktions

		int pin;
		for (pin = 0; pin < NUM_PINS; ++pin){
			int pinVal = DIGITAL_READ(PORTF, pin);
			uart1_printf("pin %d is logical %d\n", pin, pinVal);
		}
		uart1_txchar('\n');

		if(++i == NUM_PINS){
			i = 0;
		}
	}

    return 0;
}

