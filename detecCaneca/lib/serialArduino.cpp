#include "serialArduino.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>


using namespace std;
int serialport_init(const char* serialport, int baud);
int  serialArduino::sendByte(uint8_t b){
	int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;}
int  serialArduino::begin(int baud,const char *port){
	fd = 0;
	baudrate = B115200;
	baudrate = baud;
    fd = serialport_init(port, baudrate);
	if(fd==-1) return -1;}
char serialArduino::reciveByte(){
    char b[1];
    int n=0;
    while(1){
        n = read(fd, b, 1);  // read a char at a time
   		if(n==-1) return -1;    // couldn't read
        if(n==0) usleep(10000); // wait 10 msec try again
       	if(n>0) break;
    }
    return b[0];}
int  serialport_init(const char* serialport, int baud){
    struct termios toptions;
    int fd;
    fd = open(serialport, O_RDWR | O_NOCTTY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
		if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
    case 19200:  brate=B19200;  break;
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    toptions.c_cflag &= ~CRTSCTS;
    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }
    return fd;}