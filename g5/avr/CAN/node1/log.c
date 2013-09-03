#include "sdcard/ff.h"

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
