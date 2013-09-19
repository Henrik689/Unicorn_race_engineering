/*--------------------------------------------------------------------------*/
/*  RTC controls                                                            */

#include <avr/io.h>
#include <util/twi.h>
#include <string.h>
#include "rtc.h"
#include "../twi/twi.h"

BOOL rtc_clearHaltBit(void)
{
	uint8_t buf[1];
	buf[0] = 63;
	return RTC_write(0b11010000, 0x0C, 1, buf);
}

BOOL RTC_write(
	char dev,		/* Device address */
	uint8_t adr,		/* Write start address */
	uint8_t cnt,		/* Write byte count */
	uint8_t *buff		/* Write data buffer */
)
{
	uint8_t *wbuff = buff;
	
	if (!cnt) return FALSE;	
	/*
	 * Start in master write mode to transmit data to slave 
	 */
	if (!(TWI_start())) return FALSE;	/* send start condition */	

	TWI_send(dev | TW_WRITE);		/* Select device dev */
	if (!(TW_STATUS == TW_MT_SLA_ACK)) return FALSE;	/* Device could not be selected */

	/* Send address for writing start position to slave device */	
	TWI_send(adr);
	if (!(TW_STATUS == TW_MT_DATA_ACK)) return FALSE;	/* No ACK from device return */

	/* send data */
	do {
		TWI_send(*wbuff++);
		if (!(TW_STATUS == TW_MT_DATA_ACK)) return FALSE;	/* No ACK from device return */
	} while(--cnt);

	/* send stop */
	TWI_stop();

	return TRUE;
}

BOOL RTC_read(
	char dev,		/* Device address */
	uint8_t adr,		/* Read start address */
	uint8_t cnt,		/* Read byte count */
	uint8_t *buff		/* Read data buffer */
)
{
	uint8_t *rbuff = buff;
	uint8_t n;
	BOOL start = FALSE;
	uint8_t data;

	if (!cnt) return FALSE;
	/*
	 * Start in master write mode to transmit read start address to slave
	 */
	if (!(TWI_start())) return FALSE;	/* send start condition */				

	TWI_send(dev | TW_WRITE);		/* Select device dev */
	if (!(TW_STATUS == TW_MT_SLA_ACK)) return FALSE;	/* Device could not be selected */

	/* Send address for reading start position to slave device */	
	TWI_send(adr);
	if (!(TW_STATUS == TW_MT_DATA_ACK)) return FALSE;	/* No ACK from device return */

	/*
	 * Switch to master read mode to recive data from slave 
	 */	
	if (!(TWI_start())) return FALSE;	/* send start condition */

	TWI_send(dev | TW_READ);		/* Select device dev */
	if (!(TW_STATUS == TW_MR_SLA_ACK)) return FALSE;	/* Device could not be selected */

	/* Device should start sending now and first stop when do not recive a ACK after data transmition */
	do {					/* Receive data */
		cnt--;
		if (cnt > 0) {
			data = TWI_rcvr(TRUE);		/* Send ACK after reviced data */
			if (!(TW_STATUS == TW_MR_DATA_ACK)) return FALSE; /* Return if an ACK not where send after data recived */
			*rbuff++ = data;
		} else {
			data = TWI_rcvr(FALSE);		/* Send NACK after reviced data */
			if (!(TW_STATUS == TW_MR_DATA_NACK)) return FALSE; /* Return if an NACK not where send after data recived */
			*rbuff++ = TWDR;
		}
	} while (cnt);
	
	TWI_stop(); /* send stop condition */
	return TRUE;
}

BOOL rtc_gettime (RTC *rtc)
{

	uint8_t buf[7];

	if (!RTC_read(0b11010000, 0x01, 7, buf)) return FALSE;

	rtc->sec = (buf[0] & 0x0F) + ((buf[0] >> 4) & 7) * 10;
	rtc->min = (buf[1] & 0x0F) + (buf[1] >> 4) * 10;
	rtc->hour = (buf[2] & 0x0F) + ((buf[2] >> 4) & 3) * 10;
	rtc->wday = (buf[3] & 0x07);
	rtc->mday = (buf[4] & 0x0F) + ((buf[4] >> 4) & 3) * 10;
	rtc->month = (buf[5] & 0x0F) + (buf[5] >> 4) * 10;
	rtc->year = 2000 + (buf[6] & 0x0F) + (buf[6] >> 4) * 10;

	return TRUE;

/*   //This code is just to provide some kind of a valid response.*/
/*   rtc->sec = 1;*/
/*   rtc->min = 2;*/
/*   rtc->hour = 3;*/
/*   rtc->mday = 4;*/
/*   rtc->month = 5;*/
/*   rtc->year = 2006;*/


   return TRUE;
}

BOOL rtc_settime (const RTC *rtc)
{
	uint8_t buf[7];

	buf[0] = ((rtc->sec / 10) * 16 + rtc->sec % 10) & 0x7F;
	buf[1] = ((rtc->min / 10) * 16 + rtc->min % 10) & 0xF7;
	buf[2] = ((rtc->hour / 10) * 16 + rtc->hour % 10) & 0x3F;
	buf[3] = rtc->wday & 7;
	buf[4] = ((rtc->mday / 10) * 16 + rtc->mday % 10) & 0x3F;
	buf[5] = ((rtc->month / 10) * 16 + rtc->month % 10) & 0x1F;
	buf[6] = ((rtc->year - 2000) / 10) * 16 + (rtc->year - 2000) % 10;

	return RTC_write(0b11010000, 0x01, 7, buf);
}

BOOL rtc_init (void)
{
	/* This function should also test the RTC to ensure a valid clock is returned but this is work for the future */
	rtc_clearHaltBit();	/* Before this is cleared time read from RTC is time of last system power down */

	return TRUE;
} 
