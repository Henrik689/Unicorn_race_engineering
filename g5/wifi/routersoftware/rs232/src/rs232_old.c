#include <stdio.h>   /* Standard input/output definitions */

#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#define LENGTH_ECU_DATA 255
#define BUFLEN 255
#define CHECKOFFSET 92
#define RPMDATASTART 108

int initport(int fd, int baud);

int main(){

int fd2 = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NONBLOCK);	//For ROUTER
//int fd2 = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);	//For PC

char rxbuffer[BUFLEN];
char received[LENGTH_ECU_DATA];
char checkchars[BUFLEN];
char request_status_ecu[10]={0x12,0x34,0x56,0x78,0x17,0x08,0x00,0x00,0x00,0x00};
int len=0;
int k=0;
int rpm=0;
int check=0;
int runs=0;

memset(received,0,LENGTH_ECU_DATA);
memset(rxbuffer,0,BUFLEN);
memset(checkchars,0,BUFLEN);

initport(fd2,19200);

	write(fd2,"START!\n",7);
	printf("Wrote START\n");
	
while(1){
	printf("Requesting new status from ECU\n");

	tcflush(fd2,TCIOFLUSH);
	printf("ERRNO_flush: %d\n",errno);
	errno=0;

	write(fd2,request_status_ecu,10);
	printf("ERRNO_write: %d\n",errno);
	errno=0;

	tcdrain(fd2);
        printf("ERRNO_drain: %d\n",errno);
        errno=0;

	usleep(200000);

	len = read(fd2,rxbuffer,BUFLEN);
	printf("ERRNO_read: %d\n",errno);
	errno=0;
	rxbuffer[len]=0;
	printf("strlen req: %d, strlen %d, len:%d\n",strlen(request_status_ecu),strlen(rxbuffer),len);
	if(len==114){
	printf("RXBUFFER: ");
	for(k=0;k<BUFLEN;k++){
		printf("%02X",(unsigned char)rxbuffer[k]);
		//putchar(rxbuffer[k]);
	}
	printf("\n");
	}
	else{
		printf("Wrong data\n");
	printf("RXBUFFER: ");
	for(k=0;k<BUFLEN;k++){
		printf("%02X",(unsigned char)rxbuffer[k]);
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
	sleep(1);

}
	close(fd2);
}

int initport(int fd, int baud){
//!This function opens up the RS232 with baud baud
	struct termios options;
	char bauds[6];
	sprintf(bauds,"B%d",baud);

	// Get the current options for the port...
	tcgetattr(fd, &options);
	// Set the baud rates to baud...
	if(baud==9600){
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
	}
	if(baud==19200){
	cfsetispeed(&options, B19200);
	cfsetospeed(&options, B19200);
	}
	if(baud==115200){
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);
	}
	// Enable the receiver and set local mode...
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_lflag = 0;
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 2;

	// Set the new options for the port...
	tcsetattr(fd, TCSANOW, &options);
	return 1;
}
