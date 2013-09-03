//_____ I N C L U D E S ________________________________________________________
#include "config.h"
#include "can_lib.h"
//#include "adc_drv.h"
//#include "led_drv.h"
//#include "sensor_drv.h"
#include "prototypes.h"

#define MY_ID_TAG 0x80       //- 128 decimal

	U8 dummy_counter = 0;
    U8  sensor_buffer[9];
    st_cmd_t sensor_message;

//------------------------------------------------------------------------------
//! This program performs a response to an remote frame of a master.
//! The response is a data frame that contents (in the order) the local
//! temperature, the local luminosity and the local VCC values.
//------------------------------------------------------------------------------
int main (void)
{	
    CLKPR = 0x80;  CLKPR = 0x00;  // Clock prescaler Reset

    can_init(0);

	DDRA = 0xFF; // LED'er output
	PORTA = 0xFF; // LED'er tændt

    DDRD = 0xFF;
    PORTD |= 0b1000000;

    // Interrupt
    sei();

    // Modtag CAN interrupt

    CANIE2 = 0xFF;
    CANIE1 = 0xFF;

    CANGIE |=(1<<ENIT);
    CANGIE |=(1<<ENRX);
    CANGIE |=(1<<ENTX);

	send_data_CAN();
    while(1);
    return 0;
}

//------------------------------------------------------------------------------
//! An Identifier value (CAN2.0A) is given to DVK90CAN1 board (ex: 0x080).
//! 1) The first operation is to detect a CAN receive remote frame with
//!    ID == MY_ID_TAG.
//! 2) The second operation is to send a CAN data frame with the same ID and as
//!    data:
//------------------------------------------------------------------------------

void send_data_CAN(void)
{
    // --- Init Reply data
    sensor_message.pt_data = &sensor_buffer[0];

    while(1)
    {   
        
		sensor_buffer[0] = (U8)(dummy_counter);
        sensor_buffer[1] = (U8)(dummy_counter+10);
        sensor_buffer[2] = (U8)(dummy_counter+20);
        sensor_buffer[3] = (U8)(dummy_counter+30);
		sensor_buffer[4] = (U8)(dummy_counter+40);
		sensor_buffer[5] = (U8)(dummy_counter+50);
		sensor_buffer[6] = (U8)(dummy_counter+60);
		sensor_buffer[7] = (U8)(convertanalog(1));
		
        // --- Auto-reply Command
        sensor_message.ctrl.ide = 0;            //- CAN 2.0A
        sensor_message.dlc = 8;                 //- Message with x-byte data
        sensor_message.id.std = MY_ID_TAG;
        sensor_message.cmd = CMD_RX;

        // --- Enable reply
        while(can_cmd(&sensor_message) != CAN_CMD_ACCEPTED);
        // --- Wait for Reply completed
        while(can_get_status(&sensor_message) == CAN_STATUS_NOT_COMPLETED);

        PORTA ^=_BV(PORTA7);
		
		dummy_counter++;
        
    }
}

//----- ADC konvertering ------------------------------------------------------
unsigned short int convertanalog(unsigned channel)
{
	unsigned int adlow = 0;
	unsigned int adhigh = 0; 
	
    ADMUX=(1<<REFS0)|(channel & 0x0f);
    ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    ADCSRA|= (1<<ADSC);
    while(bit_is_set(ADCSRA,ADSC)); 
    adlow=ADCL; 
    adhigh=ADCH;
    return((unsigned short int)(((adhigh<<8)|(adlow & 0xFF))>>2));
}

ISR(CANIT_vect)
{
    PORTA ^=_BV(PORTA7);
}
