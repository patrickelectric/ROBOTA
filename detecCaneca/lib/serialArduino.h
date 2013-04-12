#ifndef SERIALARDUINO_H
#define SERIALARDUINO_H

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


class serialArduino{
    private:
	    char serialport[256];
	    int baudrate;  // default
	    char buf[20], dat[20], use[1];
	    int rc,n;
    public:
    	int fd;
    	int begin(int baud,const char *port);
    	int sendByte(uint8_t b);
    	char reciveByte();
};



#endif // SERIALARDUINO_H
