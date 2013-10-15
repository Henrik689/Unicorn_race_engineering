/*********************************************
 * Unicorn main gear kontrol
 *********************************************/
/*
	- Gear servo is controlled through PE5 PWM.
	- Measures the position on the gear actuator through ADC.
-Timer overflow (TIMER0_OVF_vect) styrer opdateringen af signal til aktuator (~170 Hz)
	- Timer overflow (TIMER0_OVF_vect) is a interrupt which controls the sampling frequency  
-Signal fra gearkontakter, kommer fra node 2 over CAN
*/

#include <avr/interrupt.h>
#include "config.h"
#include <util/delay.h>
#include "can_std/can_lib.h"
#include "can_comm.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "init.h"
#include "uart.h"


int main(void)
{
	//Initialise the Gear node
	ioinit();									//Port setup
	uart_init();									//Serial communication

	uart_txstring("\n\nStarting template!\n\n");

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
	Can_sei();									//Enable all interrupts
	Can_set_tx_int();							//Enable interrupt on transmit message complete on CAN bus
	Can_set_rx_int();							//Enable interrupt on receive message complete on CAN bus

	uart_txstring("\n\nNu laver vi ballede!\n\n");

 //    //Main loop for verification of gear positioning
	// while(1)
	// {
 //        _delay_ms(100);
	// 	data_buf[0] = GearNeutral;
	// 	data_buf[1] = GEARNEUTRALMEAS;
	// 	data_buf[2] = 0;
	// 	can_send_non_blocking(gear_msgid, data_buf, 3);
        
	// 	data_buf[0] = GearEst;
	// 	data_buf[1] = 0;
	// 	data_buf[2] = GearEst_val;
	// 	can_send_non_blocking(rpm_msgid, data_buf, 3);
 //    }
    return 0;
   
}
