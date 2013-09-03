#include "../integer.h"

typedef struct {
	WORD	year;	/* 2000..2099 */
	BYTE	month;	/* 1..12 */
	BYTE	mday;	/* 1.. 31 */
	BYTE	wday;	/* 1..7 */
	BYTE	hour;	/* 0..23 */
	BYTE	min;	/* 0..59 */
	BYTE	sec;	/* 0..59 */
} RTC;

/* Clears halt bit in the RTC. See datasheet */
BOOL rtc_clearHaltBit(void);

/* Able to write bytes to the rtc */
BOOL RTC_write(
	char dev,		/* Device address */
	uint8_t adr,		/* Write start address */
	uint8_t cnt,		/* Write byte count */
	uint8_t *buff		/* Write data buffer */
);

/* Able to read bytes from the rtc */
BOOL RTC_read(
	char dev,		/* Device address */
	uint8_t adr,		/* Read start address */
	uint8_t cnt,		/* Read byte count */
	uint8_t *buff		/* Read data buffer */
);

BOOL rtc_init (void);						/* Initialize RTC */
BOOL rtc_gettime (RTC*);					/* Get time */
BOOL rtc_settime (const RTC*);				/* Set time */

