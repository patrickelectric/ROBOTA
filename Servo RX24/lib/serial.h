/*
#-----------------------------------------------------------------------------
# Filename:    serial.h
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     Lib to serial comunication
*/

/***************************LIBS*****************************/
#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>   /* Standard library definitions */
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
/***************************LIBS*****************************/

using namespace std;

/*************************SERIAL VARIABLES*******************/
int fd = 0;
char serialport[256];
char buf[20], dat[20], use[1];
int rc,n;
bool debugmode =false;
/*************************SERIAL VARIABLES*******************/


class Serial
{
    public:
        int baudrate;
        char *mensagem;
        char *local_usb;
};
Serial Serial;



/*************************SERIAL FUNCTIONS*******************/
int serialport_init(const char* serialport, int baud); 
int serialport_writebyte(int fd, uint8_t b);
int serialport_write(int fd, const char* str);
int serialport_read_until(int fd, char* buf, char until);
/*************************SERIAL FUNCTIONS*******************/



int serialport_read_until(int fd, char* buf, char until)
{
    char b[1];
    int i=0;
    do {
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 )
        {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0]; i++;

    }while( b[0] != until );
    buf[i] = 0;  // null terminate the string
    return i;
}


//int serialport_writebyte( int fd, uint8_t b)
int serialport_writebyte(uint8_t b)
{
    if(debugmode == true) printf("%d\n", b);
    int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;
}

int serialport_write(int fd, const char* str)
{
    int len = strlen(str);
    int n = write(fd, str, len);
    if( n!=len )
        return -1;
    return n;
}

int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;
    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //        serialport,baud);
    //fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);

    fd = open(serialport, O_RDWR | O_NOCTTY);
    if (fd == -1)
    {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    if (tcgetattr(fd, &toptions) < 0)
    {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
    case 4800:   brate=B4800;   break;
    case 9600:   brate=B9600;   break;
    // if you want these speeds, uncomment these and set #defines if Linux
    //#ifndef OSNAME_LINUX
    //    case 14400:  brate=B14400;  break;
    //#endif
    case 19200:  brate=B19200;  break;
    // if you want these speeds, uncomment these and set #defines if Linux
    //#ifndef OSNAME_LINUX
    //#ifndef OSNAME_LINUX
    //    case 28800:  brate=B28800;  break;
    //#endif
    //case 28800:  brate=B28800;  break;
    case 38400:  brate=B38400;  break;
    case 57600:  brate=B57600;  break;
    case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);
    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;
    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl
    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }
    return fd;
}


