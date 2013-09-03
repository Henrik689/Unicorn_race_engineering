#include "config.h"
#include "led.h"


void disp_rpm(U8 leds) 
{
                  if (leds <= 7) {
                        LED_REG1 = (1<<leds)-1;
                        LED_REG2 = 0;
                    }
                    if (leds>=8) {
                        LED_REG1 = 255;
                        LED_REG2 = (1<<(leds-8))-1;
                    }
}

void disp_water_temp(U8 temp)
{
	if (temp <= 60) {
		STATUS_LED_YELLOW_ON;
		STATUS_LED_GREEN_OFF;
	} else if (temp > 60) {
		STATUS_LED_GREEN_ON;
		STATUS_LED_YELLOW_OFF;
	}
}

void disp_gear_neutral(U8 gear_n)
{
	if(gear_n <= 100)
		SEG_neutral;
	else
		SEG_OFF;
}

void disp_oil_press(U8 press)
{
	if (press < 128) {
		STATUS_LED_RED_ON;
	} else {
		STATUS_LED_RED_OFF;
	}
}
