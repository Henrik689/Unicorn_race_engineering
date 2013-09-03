#include <stdlib.h>
#include "sdcard_fs_driver/ff.h"
#include "log.h"

uint8_t sd_data_buf1[SD_BUF_SIZE];
uint8_t sd_data_buf2[SD_BUF_SIZE];
uint8_t sd_buf1_head = 0;
uint8_t sd_buf2_head = 0;
uint8_t sd_buf_in_use = 1;
uint8_t sd_buf_write = 0;

FILINFO Finfo;

#if _USE_LFN
char Lfname[_MAX_LFN+1];
#endif

int get_free_log_number( DIR *dir )
{
	BYTE res;
	DWORD p1, p2;
	UINT s1, s2;
	UINT maxname=0;
#if _USE_LFN
	Finfo.lfname = Lfname;
	Finfo.lfsize = sizeof(Lfname);
#endif
	for(;;) {
		res = f_readdir(dir, &Finfo);
		if ((res != FR_OK) || !Finfo.fname[0]) break;
		if ( atoi(&(Finfo.fname[0])) > maxname )
			maxname = atoi(&(Finfo.fname[0]));
	}
	return maxname + 1;
}

uint8_t sd_log_write(uint8_t *data, uint8_t len)
{
	switch (sd_buf_in_use) {
		case 1:
			if (SD_BUF_SIZE < sd_buf1_head + len) {
				sd_buf_in_use = 2;
				sd_buf_write = 1;
			}
			break;
		case 2:	
			if (SD_BUF_SIZE < sd_buf2_head + len) {
				sd_buf_in_use = 1;
				sd_buf_write = 1;
			}
			break;
		default:
			break;
	}

	switch (sd_buf_in_use) {
		case 1:
			while (len--) {
				sd_data_buf1[sd_buf1_head++] = *data++;
			}
			break;
		case 2:
			while (len--) {
				sd_data_buf2[sd_buf2_head++] = *data++;
			}
			break;
	}

	return 0;
}

uint8_t sd_log_check(FIL *fp)
{
	uint8_t bw;
	if (sd_buf_write == 1) {	/* One of the buffers is full */
		switch (sd_buf_in_use) {
			case 1:
				/* Write sd_data_buf2 to sd */
				f_write( fp, &sd_data_buf2, sd_buf2_head, &bw);
				sd_buf2_head = 0;
				sd_buf_write = 0;
				return 1;
		case 2:
				/* Write sd_data_buf1 to sd */
				f_write( fp, &sd_data_buf1, sd_buf1_head, &bw);
				sd_buf1_head = 0;
				sd_buf_write = 0;
				return 1;
			default:
				break;
		}
	}
	
	return 0;
}
