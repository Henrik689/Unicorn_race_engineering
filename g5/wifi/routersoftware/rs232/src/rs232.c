#include <stdio.h>   /* Standard input/output definitions */

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <linux/serial.h> 

#define LENGTH_ECU_DATA 114
#define BUFLEN 228
#define CHECKOFFSET 92
#define RPMDATASTART 108

int main(){

int fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NONBLOCK);	//For ROUTER
//int fd2 = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);	//For PC

char rxbuffer[BUFLEN];
char received[BUFLEN];
char checkchars[BUFLEN];
char request_status_ecu[10]={0x12,0x34,0x56,0x78,0x17,0x08,0x00,0x00,0x00,0x00};
int len=0;
int k=0;
int rpm=0;
int check=0;
int runs=0;
int readlen=0;

struct termios options;

memset(received,0,BUFLEN);
memset(rxbuffer,0,BUFLEN);
memset(checkchars,0,BUFLEN);


memset( &options, 0, sizeof(options) );
options.c_cflag = B19200 | CS8 | CLOCAL | CREAD | IGNPAR;
options.c_oflag = 0;
options.c_lflag = 0;
options.c_iflag = INPCK | ISTRIP;
options.c_cc[VTIME] = 1;
options.c_cc[VMIN] = 1;
//options.c_line = N_TTY;
if ( tcflush( fd, TCIOFLUSH ) < 0 )
{
	printf("prob flush\n");
}

if( tcsetattr( fd, TCSANOW, &options) < 0 )
{
	printf("prob set\n");
} 

	write(fd,"START!\n",7);
	printf("Wrote START\n");
	
while(1){
	//usleep(1000000);
	printf("Requesting new status from ECU\n");

	tcflush(fd,TCIOFLUSH);
	printf("ERRNO_flush: %d\n",errno);
	errno=0;
	
	write(fd,request_status_ecu,10);
	printf("ERRNO_write: %d\n",errno);
	errno=0;

	tcdrain(fd);
        printf("ERRNO_drain: %d\n",errno);
        errno=0;

	usleep(1000000);

	//	printf("rxbuffer: \n");
	//while(readlen<50){
	len = read(fd,rxbuffer,114);
	//printf("ERRNO_read: %d\n",errno);
	//errno=0;
	strncpy(received, rxbuffer,BUFLEN);
	readlen=readlen+len;
	//memset(rxbuffer,0,BUFLEN);
	//}
	//printf("\n\n");
	//rxbuffer[len]=0;
	printf("strlen req: %d, strlen %d, len:%d\n",strlen(request_status_ecu),strlen(received),readlen);
	if(readlen==114){
	printf("RXBUFFER: ");
	for(k=0;k<BUFLEN;k++){
		printf("%02X",(unsigned char)received[k]);
		//putchar(rxbuffer[k]);
	}
	printf("\n");
	}
	else{
		printf("Wrong data\n");
	printf("RXBUFFER: ");
	for(k=0;k<BUFLEN;k++){
		printf("%02X",(unsigned char)received[k]);
		//putchar(rxbuffer[k]);
	}
	}
	//strncat(received,rxbuffer,116);
	/*printf("Decoding data\n");
	printf("Rpm old value: %d       ", rpm);

	rpm=((rxbuffer[RPMDATASTART] << 8) | rxbuffer[RPMDATASTART+1])*0.9408;

	printf("Rpm test: %d       ", rpm);
	rpm=0;
	printf("Rpm test: %d\n",rpm);
	printf("Rpm detail test: ");
	for(k=RPMDATASTART;k<RPMDATASTART+4;k++){
		printf("%d: %02x, ",k,rxbuffer[k]);
	}
	printf("\n");*/	
	/*for(k=0;k<BUFLEN;k++){
	if(received[0]!=checkchars[0] && check==0){
		check=1;
	}
	if(received[k]==checkchars[k]){
		check=0;
		printf("No data");

	}
	else{
		printf("Length string: %d \n",k);
	}
	}*/
	memset(rxbuffer,0,BUFLEN);
	memset(received,0,BUFLEN);
	readlen=0;
	//sleep(1);

}
	close(fd);
}
