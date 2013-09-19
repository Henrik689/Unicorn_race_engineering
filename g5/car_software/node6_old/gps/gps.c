#include "../config.h"
#include "gps.h"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>

char GPGGA_flag = 0;
char GPRMC_flag = 0;
int char_counter = 0;

char msg_type[5];

// Time from gps
// Uden init, virker index 0 ikke !?!?!?
char the_time_char[7] = {'0','0','0','0','0','0'}; 
char sec_char[3] = {'0','0'};
char min_char[3] = {'0','0'};
char hours_char[3] = {'0','0'};

char satellites_char[3] = {'0','0'};

char speed_char[5] = {'0','0','0','0'}; 

// Pos
char lat_dd_char[3] = {'0','0','\0'};
char lat_mm_char[7] = {'0','0','0','0','0','0','\0'};
char lon_ddd_char[4] = {'0','0','0','\0'};
char lon_mm_char[7] = {'0','0','0','0','0','0','\0'};
double lat_dd = 0.0;	
double lat_mm = 0.0;
double lon_ddd = 0.0;	
double lon_mm = 0.0;

ISR(USART1_RX_vect)
{			
	char data;
	data = UDR1;
	//UDR0 = UDR1;

	// Index
	char_counter++;

	// Finder starten af beskeden
	if((data == '$'))
	{
		char_counter = 0;	// Reset char counter
		GPGGA_flag = 0;
		GPRMC_flag = 0;
	}

	// Besked typen, index 1-5
	if((char_counter>=1) && (char_counter<=5))
		msg_type[char_counter-1] = data;

	// Undersøger beskedtypen
	if(char_counter == 5)
	{
		if(!strcmp(msg_type,"GPGGA"))
			GPGGA_flag = 1;
		else if(!strcmp(msg_type,"GPRMC"))
			GPRMC_flag = 1;
		else
			char_counter = 0;
	}

	// The time, index 7 to 12
	if((GPGGA_flag == 1) && ((char_counter>=7) && (char_counter<=12)))
		the_time_char[char_counter-7] = data;

	if((char_counter==12))
	{
	char i;
		for(i=0 ; i<=5 ; i++)
		{
			if((i==0) || (i==1))
				hours_char[i] = the_time_char[i];

			if((i==2) || (i==3))
				min_char[i-2] = the_time_char[i];

			if((i==4) || (i==5))
				sec_char[i-4] = the_time_char[i];
		}
		// Time to integer
		the_time_h = atoi(hours_char) +1;
		the_time_m = atoi(min_char);
		the_time_s = atoi(sec_char);
	}

	// N/S Indicator, Index 28
	if((GPGGA_flag == 1) && (char_counter==28))
		N_S_indicator = data;

	// E/W Indicator, Index 41
	if((GPGGA_flag == 1) && (char_counter==41))
		E_W_indicator = data;

	// Antal satelitter, index 45 to 46
	if((GPGGA_flag == 1) && ((char_counter>=45) && (char_counter<=46)))
		satellites_char[char_counter-45] = data;

	if((GPGGA_flag == 1) && (char_counter==46))
		satellites = atoi(satellites_char);

	// Speed, index 45 til 48
	if((GPRMC_flag == 1) && ((char_counter>=45) && (char_counter<=48)))
		speed_char[char_counter-45] = data;

	if((GPRMC_flag == 1) && (char_counter==48))
	speed = (int)(atof(speed_char)*1.852); // Knots til km/h

	// Latitude dd, index 18 til 19
	if((GPGGA_flag == 1) && ((char_counter>=18) && (char_counter<=19)))	
		lat_dd_char[char_counter-18] = data;

	// Latitude mm.mmmm,  index 20 til 26
	if((GPGGA_flag == 1) && ((char_counter>=20) && (char_counter<=26)))	
		lat_mm_char[char_counter-20] = data;

	if((GPGGA_flag == 1) && (char_counter==26))
	{
		lat_dd = atof(lat_dd_char);
		lat_mm = atof(lat_mm_char);
		lat = lat_dd + (lat_mm/60.0); // Konvertering til dd.dddd (google maps)
	}

	// Longitude ddd, index 30 til 32
	if((GPGGA_flag == 1) && ((char_counter>=30) && (char_counter<=32)))	
		lon_ddd_char[char_counter-30] = data;

	// Longitude mm.mmmm,  index 33 til 39
	if((GPGGA_flag == 1) && ((char_counter>=33) && (char_counter<=39)))	
		lon_mm_char[char_counter-33] = data;

	if((GPGGA_flag == 1) && (char_counter==39))
	{
		lon_ddd = atof(lon_ddd_char);
		lon_mm = atof(lon_mm_char);
		lon = lon_ddd + (lon_mm/60.0); // Konvertering til dd.dddd (google maps)
	}
}

void uartinit1(void)
{
	//Enable TXen og RXen
	UCSR1B = (1<<RXEN1)|(1<<TXEN1); 
	
	// Format: 8data, 1 stop bit
	UCSR1C = (3<<UCSZ10);

	// Baud rate
	UBRR1L = BAUD_PRESCALE1;
	UBRR1H = (BAUD_PRESCALE1 >> 8);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	UCSR1B|=(1<<RXCIE1);
}
