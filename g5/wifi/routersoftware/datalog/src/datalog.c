#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <linux/serial.h> 
#include <sys/socket.h>
#include <netinet/in.h>


#define LENGTH_ECU_DATA 255
#define BUFLEN 255
#define CHECKOFFSET 92
#define RPMDATASTART 108

int main(){

int fd = open("/dev/ttyS1", O_RDWR | O_NOCTTY);	//For ROUTER
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

struct termios options;

//Socket structs for client socket
int sockfd,n;
struct sockaddr_in servaddr,cliaddr;
char sendline[1000];
char linebuffer[255];

//Sockets structs for server socket
int sockserver;
struct sockaddr_in servsockaddr,clisockaddr;
char servbuffer[255];
int servrecv=0;
int servreceived=0;
int datavalid=0;


memset(linebuffer,0,255);

memset(received,0,LENGTH_ECU_DATA);
memset(rxbuffer,0,BUFLEN);
memset(checkchars,0,BUFLEN);


memset( &options, 0, sizeof(options) );
options.c_cflag = B19200 | CS8 | CLOCAL | CREAD | IGNPAR;
options.c_oflag = 0;
options.c_lflag = 0;
options.c_iflag = INPCK | ISTRIP;
options.c_cc[VTIME] = 1;
options.c_cc[VMIN] = 114;
//options.c_line = N_TTY;
tcsetattr(fd,TCSANOW,&options);
if ( tcflush( fd, TCIOFLUSH ) < 0 )
{
	printf("prob flush\n");
}

//Initialize the server socket
printf("Initializing the server socket\n"); 
if((sockserver=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
	printf("Failed to create server socket\n");
} 
memset(&servsockaddr,0,sizeof(servsockaddr));
servsockaddr.sin_family = AF_INET;
servsockaddr.sin_addr.s_addr=inet_addr("192.168.2.1");
servsockaddr.sin_port=htons(1200);
if (bind(sockserver, (struct sockaddr *) &servsockaddr, sizeof(servsockaddr))<0){
	printf("Failed to bind socket\n");
}
printf("DONE \n");


//Initialize the client socket to send to the PIT PC via wifi
sockfd=socket(AF_INET,SOCK_DGRAM,0);
strcpy(sendline,"test linje \n");
memset(&servaddr,0,sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr=inet_addr("192.168.2.219");
servaddr.sin_port=htons(21000);

sendto(sockfd,sendline,BUFLEN,0, (struct sockaddr *)&servaddr,sizeof(servaddr));


while(1){
	runs++;
	printf("Requesting new status from ECU, runs: %d\n", runs);

	//tcflush(fd,TCIOFLUSH);


	//write(fd,request_status_ecu,10);


	//tcdrain(fd);


//	usleep(100000);
	len=0;
	//while(len<115){
	len = read(fd,rxbuffer,114);
	//usleep(100000);
	//}
	printf("length of buffer: %d\n",len);
	rxbuffer[len]=0;

	
	//Receive data from the network node
	/*printf("Attempt to get data from node\n");
	if ((servrecv = recvfrom(sockserver, servbuffer, 50, 0, (struct sockaddr *) &clisockaddr, sizeof(clisockaddr))) < 0) {
                printf("Failed to receive message, error: %d \n",servrecv);
              }
	printf("Servbuffer: %s\n",servbuffer);
	servreceived=(servbuffer[0]|servbuffer[1]<<8);
        printf("Received %d mV\n",servreceived);
	for(k=0;k<10;k++){
		printf("%02x ",servbuffer[k]);
	}
	printf("\n");*/






	for(k=115;k<119;k++){
	rxbuffer[k]=servbuffer[k-109];
	}
	sendto(sockfd,rxbuffer,BUFLEN,0, (struct sockaddr *)&servaddr,sizeof(servaddr));

	//printf("Sent udp, runs: %d\n",runs);
	memset(rxbuffer,0,BUFLEN);
	memset(servbuffer,0,255);

}
	close(fd);
}
