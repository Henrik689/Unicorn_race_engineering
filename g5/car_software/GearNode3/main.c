/*********************************************
 * Unicorn main gear control
 *********************************************/
/*
	- Gear servo is controlled through PE5 PWM.
	- Measures the position on the gear actuator through ADC.
	- Timer overflow (TIMER0_OVF_vect) is an interrupt which controls the update frequency of the gear actuator (~170 Hz)
	- Signal from gear buttons originates from node 2 through Can-bus.
*/

#include "config.h"
#include "extern.h"
#include "prototyper.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "can_std/can_lib.h"
#include "can_new.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"

unsigned int GearEst_val = 0;
unsigned int gearPosition = 0;
unsigned int gearPositionOld = 0;
unsigned int gearGotoPosition = 0;
unsigned int gearCounter = 0;
unsigned int gearBut = 0;
unsigned int gearButNeuMeas = 0;
unsigned short int gearActive = 0;
unsigned short int gearButActive = 0;			//Set if gear button was active 
unsigned short int gearButCAN = 0;


int main(void)
{
	//Initialise the Gear node
	_delay_ms(500);
	uint8_t data_buf[8];
	ioinit();									//Port setup
	uartinit();									//Serial communication
	can_init(0);								//Can setup
    pwm16Init2();								//Setup PWM controller
	adcInit(1);									//Setup ADC for pot-meter or Amp meter
	st_cmd_t rpm_msg;							//Define struct for received rpm message
	counter0Init();								//Init interrupt counter to overflow with 168Hz

	_delay_ms(500);

	sei();										//Enable interrupt

	//rpm_msg.pt_data = rpm_response_buffer;
	//rpm_msg.status = 0;

    can_update_rx_msg(&rpm_msg, gear_msgid, 8);	//Wait on acceptances from MotorDriver, to see if RPM is 0

	//Enable CAN
	Can_sei();												//Enable all interrupts
	Can_set_tx_int();										//Enable interrupt on transmit message complete on CAN bus
	Can_set_rx_int();										//Enable interrupt on receive message complete on CAN bus
    
    //Main loop for verification of gear positioning
	while(1)
	{
		//Send a non-blocking 3 byte message to the CAN with ID and gear neutral state 
        _delay_ms(100);
		data_buf[0] = GearNeutral;							//Gear neutral ID
		data_buf[1] = GEARNEUTRALMEAS;						//Gear neutral state
		data_buf[2] = 0;
		can_send_non_blocking(gear_msgid, data_buf, 3);
        
        //Send a non-blocking 3 byte message to the CAN with ID and the estimated gear value 
		data_buf[0] = GearEst;								//Gear estimated ID
		data_buf[1] = 0;	
		data_buf[2] = GearEst_val;							//Gear estimated value
		can_send_non_blocking(rpm_msgid, data_buf, 3);
    }
    return 0;
}