#include <stdlib.h>
#include <stdio.h>    /* Standard input/output definitions */
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <pthread.h>
#include <math.h>
#include <time.h>

//#include "serial.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "Cronometer.h"
#include "Cronometer.cpp"
#include "robota_data.h"
#include "robota_opencv.h"

#include "serialArduino.h"
#include "serialArduino.cpp"
#include "robotaMatlab.h"
#include "filtro.h"

//VideoCapture cap("/home/patrick/Área de Trabalho/Dropbox/Bolsa/Fluxo optico/Programas/video3.mp4");                /* declare video capture */
VideoCapture cap(0);