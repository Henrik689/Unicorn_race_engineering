#include <stdint.h>
#include "sdcard_fs_driver/ff.h"

#define SD_BUF_SIZE 100

/*
 * Vars used by sd save functions
 */
extern uint8_t sd_data_buf1[100];	/* Data buf1 */
extern uint8_t sd_data_buf2[100];	/* Data buf2 */

extern uint8_t sd_buf1_head;		/* Head of buffer1 */
extern uint8_t sd_buf2_head;		/* Head of buffer2 */

extern uint8_t sd_buf_in_use;		/* Var that indicate what buffer that is active */
extern uint8_t sd_buf_write;		/* One when buffer full and rdy for write to sd */
/* finder den log der har det største nummer og retuner dette + 1 */
int get_free_log_number( DIR *);

/*
 * Add data to log file
 */
uint8_t sd_log_write(uint8_t *data, uint8_t len);
uint8_t sd_log_check( FIL *fp);
