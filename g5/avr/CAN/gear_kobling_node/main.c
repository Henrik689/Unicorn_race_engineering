//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "can_lib.h"
#include <util/delay.h>
#include "led.h"
#include "display.h"
#include "can_func.h"
#include "kobling.h"
#include "../lib/can_defs.h"

//_____ D E F I N I T I O N S __________________________________________________
#define ID_BASE 0x80
//_____ D E C L A R A T I O N S ________________________________________________
void init(void);
void init_knapper(void);

int main (void)
{	
    U8 canData=0;
    int j=0;
    int k=12;
    CLKPR = 0x80;  CLKPR = 0x00;  // Clock prescaler Reset
	int EcuError = 0;
   int ratio,gearTmp;

//  Init CAN, UART, I/O
	init();
	init_led();
	init_knapper();
	init_kobling_adc();
    sei();

    // --- Init variables
    error_msg.pt_data = &error_response_buffer[0];
    error_msg.status = 0;

    rpm_msg.pt_data = &rpm_response_buffer[0];
    rpm_msg.status = 0;

    gear_status_msg.pt_data = &gear_status_response_buffer[0];
    gear_status_msg.status = 0;

    fade_in(1000, 50);
    SEG_0;

	while (1) {
	}

    can_update_rx_msg(&error_msg, error_msgid, 8);
    can_update_rx_msg(&rpm_msg, rpm_msgid, 8);
    can_update_rx_msg(&gear_status_msg, gear_status_msgid, 1);   
	while (1) {
		if (can_get_status(&error_msg) == CAN_STATUS_COMPLETED) {  // check for error_msg
			EcuError = error_response_buffer[0];                     
			can_update_rx_msg(&error_msg, error_msgid, 8);      // update error_msg to accept a new msg
		}
		if (EcuError == 1) {
			SEG_E;
		} else {
			SEG_0;
		}

		if (can_get_status(&rpm_msg) == CAN_STATUS_COMPLETED) {  // check for rpm_msg
			canData = rpm_response_buffer[0];                     
			can_update_rx_msg(&rpm_msg, rpm_msgid, 8);      // update rpm_msg to accept a new msg
			if (EcuError ==0)
				disp_rpm(canData);
		}

		if (can_get_status(&gear_status_msg) == CAN_STATUS_COMPLETED) {  // check for gear_status_msg
			canData = gear_status_response_buffer[0];                     
			can_update_rx_msg(&gear_status_msg, gear_status_msgid, 8);  // update gear_status_msg to accept a new msg
			ratio = canData;
			if (EcuError ==0) {
				if(ratio>48 && ratio<53) {
					if(gearTmp==1) {
						SEG_1;
					}
					gearTmp=1;
				} else if(ratio>67 && ratio<72) {
					if(gearTmp==2) {
						SEG_2;
				}
					gearTmp=2;
				} else if(ratio>82 && ratio<86) {
					if(gearTmp==3) {
						SEG_3;
					}
					gearTmp=3;
				} else if(ratio>93 && ratio<97) {
					if(gearTmp==4) {
						SEG_4;
					}
					gearTmp=4;
				}
			}
		}
	}
	return 0;
}

void init(void)
{
    // CAN Init
    can_init(0);

    // IO Init
    DDRA = 0xFF;
}

void init_knapper(void)
{
	//  Gear knapper init
	DDRD &=~ (1<<PD2);
	DDRD &=~ (1<<PD3);  

	// INT2
	EICRA |=(1<<ISC20);
	EICRA |=(1<<ISC21);

	// INT3
	EICRA |=(1<<ISC30);
	EICRA |=(1<<ISC31);

    	EIMSK |= (1<<INT2); 
	EIMSK |= (1<<INT3);
}
