#include <stdint.h>
#include "../config.h"
#include <util/twi.h>
#include <util/delay.h>
#include "display.h"
#include "../twi/twi.h"

uint8_t buttons_state = BUTTON1 | BUTTON2;
uint16_t indi_leds_state = 0;

display_test(void)
{
	uint8_t mode;

	mode = LED_ON;
	
	set_leds(LED0_7_ADDR, mode<<LED_RPM0 | mode<<LED_RPM1 | mode<<LED_RPM2 | mode<<LED_RPM3 | mode<<LED_RPM4 | mode<<LED_RPM5 | mode<<LED_RPM6 | mode<<LED_RPM7);
	set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
	set_leds(SEG_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
	set_leds(LED_BUTTONS_ADDR, mode<<LED_INDI1 | mode<<LED_INDI2 | mode<<LED_INDI3 | mode<<LED_INDI4 | mode<<LED_INDI5 | mode<<LED_INDI6 | mode<<LED_BUTTON_1 | mode<<LED_BUTTON_2);

	_delay_ms(1000);

	set_leds(LED0_7_ADDR, 0);
	set_leds(LED8_15_ADDR, 0);
	set_leds(SEG_ADDR, 0);
	set_leds(LED_BUTTONS_ADDR, 0);
}

set_rpm(uint16_t rpm, uint8_t mode)
{	
	/* Set leds based on rpm value */
	if (rpm<=RPM0 ) {
		set_leds(LED0_7_ADDR, 0);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM0 && rpm <= RPM1 ){
		set_leds(LED0_7_ADDR, mode<<0);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM1 && rpm <= RPM2 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM2 && rpm <= RPM3 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM3 && rpm <= RPM4 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM4 && rpm <= RPM5 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM5 && rpm <= RPM6 ){
		set_leds(LED0_7_ADDR,  mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM6 && rpm <= RPM7 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12);
		set_leds(LED8_15_ADDR, 0);
	
	} else if( rpm > RPM7 && rpm <= RPM8 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
		set_leds(LED8_15_ADDR, 0);

	} else if( rpm > RPM8 && rpm <= RPM9 ){
		set_leds(LED8_15_ADDR, mode<<0);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM9 && rpm <= RPM10 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM10 && rpm <= RPM11 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM11 && rpm <= RPM12 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM12 && rpm <= RPM13 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM13 && rpm <= RPM14 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM14 && rpm <= RPM15 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM15 && rpm <= RPM16 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( rpm > RPM16 ){
		set_leds(LED8_15_ADDR, LED_BLINK1<<8 | LED_BLINK1<<10 | LED_BLINK1<<12 | LED_BLINK1<<14);
		set_leds(LED0_7_ADDR, 0); 
	}
}

set_voltage(uint16_t voltage, uint8_t mode)
{	
	/* Set leds based on voltage value */
	if (voltage<=VOL0 ) {
		set_leds(LED0_7_ADDR, 0);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL0 && voltage <= VOL1 ){
		set_leds(LED0_7_ADDR, mode<<0);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL1 && voltage <= VOL2 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL2 && voltage <= VOL3 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL3 && voltage <= VOL4 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL4 && voltage <= VOL5 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL5 && voltage <= VOL6 ){
		set_leds(LED0_7_ADDR,  mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL6 && voltage <= VOL7 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12);
		set_leds(LED8_15_ADDR, 0);
	
	} else if( voltage > VOL7 && voltage <= VOL8 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
		set_leds(LED8_15_ADDR, 0);

	} else if( voltage > VOL8 && voltage <= VOL9 ){
		set_leds(LED8_15_ADDR, mode<<0);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( voltage > VOL9 && voltage <= VOL10 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( voltage > VOL10 && voltage <= VOL11 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( voltage > VOL11 && voltage <= VOL12 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( voltage > VOL12 && voltage <= VOL13 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( voltage > VOL13 && voltage <= VOL14 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( voltage > VOL14 && voltage <= VOL15 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( voltage > VOL15){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	}
}

set_water_temp(uint16_t temp, uint8_t mode)
{	
	/* Set leds based on voltage value */
	if (temp<=TEMP0 ) {
		set_leds(LED0_7_ADDR, 0);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP0 && temp <= TEMP1 ){
		set_leds(LED0_7_ADDR, mode<<0);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP1 && temp <= TEMP2 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP2 && temp <= TEMP3 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP3 && temp <= TEMP4 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP4 && temp <= TEMP5 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP5 && temp <= TEMP6 ){
		set_leds(LED0_7_ADDR,  mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP6 && temp <= TEMP7 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12);
		set_leds(LED8_15_ADDR, 0);
	
	} else if( temp > TEMP7 && temp <= TEMP8 ){
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
		set_leds(LED8_15_ADDR, 0);

	} else if( temp > TEMP8 && temp <= TEMP9 ){
		set_leds(LED8_15_ADDR, mode<<0);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( temp > TEMP9 && temp <= TEMP10 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( temp > TEMP10 && temp <= TEMP11 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( temp > TEMP11 && temp <= TEMP12 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( temp > TEMP12 && temp <= TEMP13 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( temp > TEMP13 && temp <= TEMP14 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( temp > TEMP14 && temp <= TEMP15 ){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	} else if( temp > TEMP15){
		set_leds(LED8_15_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);
		set_leds(LED0_7_ADDR, mode<<0 | mode<<2 | mode<<4 | mode<<6 | mode<<8 | mode<<10 | mode<<12 | mode<<14);

	}
}


int8_t set_leds(int addr, uint16_t leds)
{
	if (!(TWI_start())) return -1;
	
	TWI_send(addr);
	if (!(TW_STATUS == TW_MT_SLA_ACK)) return -2;		/* No ACK from device return */

	TWI_send(AI|LS0);					/* Auto-increment and start from first led reg */
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;		/* No ACK from device return */

	TWI_send(leds & 0xff);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

	TWI_send( (leds>>8) & 0xff);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

	/* send stop */
	TWI_stop();
	return 1;
}


int8_t set_blink_rate(int addr, uint8_t blink_nr, uint8_t blink_rate, uint8_t blink_duty_cycle)
{
	if (!(TWI_start())) return -1;
	
	TWI_send(addr);
	if (!(TW_STATUS == TW_MT_SLA_ACK)) return -2;		/* No ACK from device return */
	if (blink_nr == LED_BLINK1) {
		TWI_send(AI|PSC0);					/* Auto-increment and start from PSC0 */
		if (!(TW_STATUS == TW_MT_DATA_ACK)) return -3;
	} else if (blink_nr == LED_BLINK2) {
		TWI_send(AI|PSC1);					/* Auto-increment and start from PSC0 */
		if (!(TW_STATUS == TW_MT_DATA_ACK)) return -4;
	}
		
	TWI_send(blink_rate);
	if (!(TW_STATUS == TW_MT_DATA_ACK)) return -5;		/* No ACK from device return */

	TWI_send(blink_duty_cycle);
	if (!(TW_STATUS == TW_MT_DATA_ACK)) return -6;		/* No ACK from device return */

	/* send stop */
	TWI_stop();
	return 1;
}

uint8_t get_buttons(int addr)
{
	uint8_t data;

	if (!(TWI_start())) return 8;

	TWI_send(addr);
	if (!(TW_STATUS == TW_MT_SLA_ACK)) return 9;		/* No ACK from device return */

	TWI_send(INPUT1);
	if (!(TW_STATUS == TW_MT_DATA_ACK)) return 10;		/* No ACK from device return */

	if (!(TWI_start())) return 11;

	TWI_send(addr | 1);
	if (!(TW_STATUS == TW_MR_SLA_ACK)) return 12;		/* No ACK from device return */

	data = 0;
	data = TWI_rcvr(0);

	/* send stop */
	TWI_stop();
	return data;
}

int8_t set_seg(int addr, uint8_t i, uint8_t j)
{
	if (!(TWI_start())) return -1;

	TWI_send(addr);
	if (!(TW_STATUS == TW_MT_SLA_ACK)) return -2;		/* No ACK from device return */

	TWI_send(AI|LS0);					/* Auto-increment and start from first led reg */
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;		/* No ACK from device return */

        TWI_send(i);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(j);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */


	/* send stop */
	TWI_stop();
	return 1;
}

int test_display(int addr, int i, int j )
{
	if (!(TWI_start())) return -1;
	
	TWI_send(0xc0);
	if (!(TW_STATUS == TW_MT_SLA_ACK)) return -2;		/* No ACK from device return */

	TWI_send(0x12);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;		/* No ACK from device return */

        TWI_send(0x00);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(0x10);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(0x00);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(0x10);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(i);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(j);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(0xFF);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

        TWI_send(0x00);
        if (!(TW_STATUS == TW_MT_DATA_ACK)) return -2;          /* No ACK from device return */

	/* send stop */
	TWI_stop();

	return 1;
}
