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
#include "serialTest.cpp"

using namespace std;

#if 0
	VideoCapture cap(0);
#else
	#if 0
		VideoCapture cap("/home/patrick/Área de Trabalho/Dropbox/Bolsa/Fluxo optico/Programas/video3.mp4");                /* declare video capture */
	#else
		VideoCapture cap(1);
	#endif
#endif

struct vetor
{
	float modulo;
	float angulo;
};

//-----------------------------------------------------------------             //funcao para fazer mapping
float fmap(float x, float in_min, float in_mRX, float out_min, float out_mRX)          //map value
{
	/*
	printf("%f\n",(x - in_min));
	printf("%f\n",((x - in_min)*(out_mRX - out_min)));
	printf("%f\n",(((x - in_min)*(out_mRX - out_min))/(in_mRX - in_min)));
	*/
  	return ((((x - in_min)*(out_mRX - out_min))/(in_mRX - in_min)) + out_min);
};
