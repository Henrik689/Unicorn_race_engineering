/*********************************************
 * Unicorn main gear kontrol
 *********************************************/

#include <config.h>
#include <extern.h>
#include <prototyper.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	ioinit();
	uartinit();
	pwm8Init();
	//pwm16Init();
	adcInit(0);
	counter0Init();
	pcintInit();
	hbroEnable(1);
	sei();

	gearPosTargetUp = GEARPOSMAX;
	gearPosTargetDown = GEARPOSMIN;

	while (1)
	{

		gearPosTargetUp = GEARPOSMAX;
		gearPosTargetDown = GEARPOSMIN;
		gearRetning = 0;

		if((PINB & 0x01) == 0x00) // Gear Kontakt GEAROP
		{
			_delay_ms(5);

			if((PINB & 0x01) == 0x00) // Stadig trykket ? (væk med støj)
			{
				if((PINB & 0x04) == 0x00)
					gearPosTargetUp = GEARPOSNEUTRALUP;
				else
					gearPosTargetUp = GEARPOSMAX;

				gearRetning = GEAROP;
				softwareTrig;
				_delay_ms(300); // Vent til gearskift er ca done
			}			
		}	

		if((PINB & 0x02) == 0x00) // Gear Kontakt GEARNED
		{
			_delay_ms(5);

			if((PINB & 0x02) == 0x00) 
			{
				if((PINB & 0x04) == 0x00)
					gearPosTargetDown = GEARPOSNEUTRALDOWN;
				else
					gearPosTargetDown = GEARPOSMIN;

				gearRetning = GEARNED;
				softwareTrig;

				_delay_ms(300);
			}			
		}

		// Venter pa at begge kontakter er sluppet igen
		while((PINB & 0b00000011) != 0b00000011){}
	}
    return 0;
}
