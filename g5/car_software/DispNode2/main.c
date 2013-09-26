// TODO
// -------------------------------- ERRORS -------------------------------------
// - Find the error where the board stops sending gear buttons press to can 
//


//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "can_std/can_lib.h"
#include "can_std/can_drv.h"
#include <util/delay.h>
#include <util/atomic.h>
#include "twi/twi.h"
#include "can.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "display/display.h"
#include "prototyper.h"
#include "data.h"

//_____ D E F I N I T I O N S __________________________________________________
#define ID_BASE 0x80
//_____ D E C L A R A T I O N S ________________________________________________

enum MODES {
	RPM_MODE,
	VOLTAGE_MODE,
	WATER_TEMP_MODE,
};

static uint8_t mode=0;
void init(void);
char gearbut = 0;
char j = 0;
int gearActive = 0;

int main (void)
{	
	int tmp,i,res;
	CLKPR = 0x80;  CLKPR = 0x00;  // Clock prescaler Reset

/*-----------------------------------------------------------------*
 *------------------------- Gear buttoms setup---------------------*
 *-----------------------------------------------------------------*/
	DDRC&=~(1<<PC7); // Neutral
	PORTC |= (1<<PC7); // Neutral pull-up
	DDRE&=~(1<<PE6); // Knap1 
	DDRE&=~(1<<PE7); // Knap2

	/* Buttoms interrupt */
	EICRB |= (1<<ISC71|1<<ISC70|1<<ISC61|1<<ISC60); /* Rising edge */
	EIMSK |= (1<<INT7 | 1<<INT6);


	uint8_t test_rx[8];

	int8_t data;
	char streng[10];

	// Recieve buffer
	st_cmd_t msg;

//  Init CAN, UART, I/O
	init();
	uartinit();
	sendtekst("UART initialized\n\r");
	TWI_init();
	sendtekst("TWI initialized\n\r");

	sei();		/* Interrupt enable */
	sendtekst("Interrupt enabled\n\r");
/*-----------------------------------------------------------------*
 *----------------------------Display setup -----------------------*
 *-----------------------------------------------------------------*/

	/* Set blink rates */
	set_blink_rate(LED0_7_ADDR, LED_BLINK1, 20, 100);
	set_blink_rate(LED0_7_ADDR, LED_BLINK2, 0, RPM_LED_DUTYCYCLE*2.56);

	set_blink_rate(LED8_15_ADDR, LED_BLINK1, (1.0/RPM16_RATE)*252, RPM16_DUTYCYCLE*2.56);
	set_blink_rate(LED8_15_ADDR, LED_BLINK2, 0, RPM_LED_DUTYCYCLE*2.56);

	set_blink_rate(SEG_ADDR, LED_BLINK1, 20, 100);
	set_blink_rate(SEG_ADDR, LED_BLINK2, 0, SEG_DUTYCYCLE*2.56);

	set_blink_rate(LED_BUTTONS_ADDR, LED_BLINK1, 20, 100);
	set_blink_rate(LED_BUTTONS_ADDR, LED_BLINK2, 0, SEG_DUTYCYCLE*2.56);


/*-----------------------------------------------------------------*
 *----------------------------CAN interrupt setup -----------------*
 *-----------------------------------------------------------------*/
	Can_sei();		/* Enable general can interrupt */
	Can_set_tx_int();	/* Enable can tx interrupt */
	Can_set_rx_int();	/* Enable can rx interrupt */

	/*
	 *	Kode til hurtig test af can 
	 */
	sendtekst("Config 3 mailboxes for rpm_msgid...\n\r");
	msg.id.std = rpm_msgid;
	msg.dlc = 8;
	res = can_config_rx_mailbox(&msg, 3);
	if (res == CAN_CMD_ACCEPTED) {
		sendtekst("SUCCESS\n\r");
	} else {
		sendtekst("FAIL\n\r");
	}	
    	// --- Init variables

	/* Init user led 0 & 1 */
	DDRB |= (1<<PB6 | 1<<PB5);
	PORTB |= (1<<PB6 | 1<<PB5);

	sendtekst("Beep\n\r");
	display_test();
    
    params.GearEst = 0;

	char dataout[] = {gear,0};

	while (1) {
		_delay_ms(20);

		/* Display selected parameter */
		if (mode == RPM_MODE) {		
			set_rpm(params.rpm, LED_ON);
		} else if (mode == VOLTAGE_MODE) {
			set_voltage(params.batteryV, LED_ON);
		} else if (mode == WATER_TEMP_MODE) {
			set_water_temp(params.waterTemp, LED_ON);
		}

		// Geat buttons to CAN
		dataout[1] = 0;
		/* Format buttom states for sending */ 
		dataout[1] |= (params.GearButDown*GEARDOWNBUT | 
					GEARUPBUT*params.GearButUp | 
					params.GearButNeutral*GEARNEUBUT);

		/* Send buttom states */
		if(dataout[1] != 0) {
			// Hack, sender gearskiftesignal et par gange, sådan at det går igennem
			// Symptombehandling, sygdommen skal kureres...
			if (dataout[1] & (GEARDOWNBUT) == GEARDOWNBUT)
				indi_leds_state |= (LED_BLINK2<<LED_BUTTON_1);
			if (dataout[1] & (GEARUPBUT) == GEARUPBUT)
				indi_leds_state |= (LED_BLINK2<<LED_BUTTON_1);
		

			set_leds(LED_BUTTONS_ADDR, indi_leds_state);
			for(j=0;j<1;j++){
				can_send_non_blocking(gear_msgid, dataout, 2);
				_delay_ms(5);
			}
			indi_leds_state &= ~(LED_BLINK2<<LED_BUTTON_2);
			indi_leds_state &= ~(LED_BLINK2<<LED_BUTTON_1);
			set_leds(LED_BUTTONS_ADDR, indi_leds_state);

		}
		/* Clear buttom states */
		params.GearButDown = 0;
		params.GearButUp = 0;
		params.GearButNeutral = 0;

		/* Display bottons code */
		buttons_state = get_buttons(LED_BUTTONS_ADDR) & (BUTTON1 | BUTTON2);
		if (buttons_state == 2) {
			indi_leds_state |= (LED_BLINK2<<LED_BUTTON_1);
			indi_leds_state &= ~(LED_BLINK2<<LED_BUTTON_2);
			mode = VOLTAGE_MODE;
		} else if (buttons_state == 1) {
			indi_leds_state |= (LED_BLINK2<<LED_BUTTON_2);
			indi_leds_state &= ~(LED_BLINK2<<LED_BUTTON_1);
			mode = WATER_TEMP_MODE;
		} else if (buttons_state == 0) {
			indi_leds_state |= (LED_BLINK2<<LED_BUTTON_1 | LED_BLINK2<<LED_BUTTON_2);
		} else {
			indi_leds_state &= ~(LED_BLINK2<<LED_BUTTON_1 | LED_BLINK2<<LED_BUTTON_2);
			mode = RPM_MODE;
		}

		/* Indicator for water temp */
		if (params.waterTemp <= WATER_OK) {
			indi_leds_state |= (LED_BLINK2<<LED_INDI1);
			indi_leds_state &= ~(LED_BLINK2<<LED_INDI4);
		} else if (params.waterTemp > WATER_OK) {
			indi_leds_state |= (LED_BLINK2<<LED_INDI4);
			indi_leds_state &= ~(LED_BLINK2<<LED_INDI1);
		}

		/* Indicator for batt ok */
		if (params.batteryV <= VOLTAGE_OK) {
			indi_leds_state |= (LED_BLINK2<<LED_INDI2);
			indi_leds_state &= ~(LED_BLINK2<<LED_INDI5);
		} else if (params.batteryV > VOLTAGE_OK) {
			indi_leds_state |= (LED_BLINK2<<LED_INDI5);
			indi_leds_state &= ~(LED_BLINK2<<LED_INDI2);
		}
		
		/* Indicator for oil pressure ok */
		if (params.oilPressure <= OILPRESS_OK) {
			indi_leds_state |= (LED_BLINK2<<LED_INDI3);
			indi_leds_state &= ~(LED_BLINK2<<LED_INDI6);
		} else if (params.oilPressure > OILPRESS_OK) {
			indi_leds_state |= (LED_BLINK2<<LED_INDI6);
			indi_leds_state &= ~(LED_BLINK2<<LED_INDI3);
		}

		/* Indicator for Gear */
		if (params.GearNeutral < 0) {
			SEG_N(LED_ON);
		} else {
			if (params.GearEst > 6) {
				SEG_OFF();
			} else {
				switch (params.GearEst) {
                    case 0:
						SEG_N(LED_ON);
						break;
					case 1:
						SEG_1(LED_ON);
						break;
					case 2:
						SEG_2(LED_ON);
						break;
					case 3:
						SEG_3(LED_ON);
						break;
					case 4:
						SEG_4(LED_ON);
						break;
					case 5:
						SEG_5(LED_ON);
						break;
					case 6:
						SEG_6(LED_ON);
						break;
					default:
						break;

				}
			}
		}


/*		if (params.GearNeutral == 0) {
			SEG_OFF();
		} else if (params.GearNeutral > 0) {
			SEG_N(LED_BLINK2);
		}
*/

		/* Set indicator leds */
		set_leds(LED_BUTTONS_ADDR, indi_leds_state);
			
/*		itoa(params.batteryV, streng, 10);*/
/*		sendtekst(streng);*/
/*		sendtekst("\n\r");		*/
		PORTB ^= (1<<PB6);
	}
	return 0;
}

void init(void)
{
    // CAN Init
    can_init(0);
}
