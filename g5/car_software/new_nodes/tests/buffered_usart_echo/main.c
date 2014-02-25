#include <stdlib.h> // size_t
#include <avr/interrupt.h> // sei()
#include <usart.h>

char buf[256] = {'\0'};
size_t bufIndex = 0;

int main(void) {
	usart1_init();								//Serial communication

	sei();										//Enable interrupt

	usart1_printf("\n\n\nSTARTING\n");

	while(1){
		// Main work loop
		if (usart1_hasData()) {
			char c = usart1_getc();
			buf[bufIndex] = c;
			if (c == '\n') {
				buf[bufIndex+1] = '\0';
				usart1_printf("s", buf); // Echo the string back
				bufIndex = 0;
			}
		}
	}

    return 0;
}

