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
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>

//#include "serial.h"
#include "Cronometer.h"
#include "Cronometer.cpp"
#include "serialArduino.h"
#include "serialArduino.cpp"
#include "robotaMatlab.h"
#include "filtro.h"
#include "serialTest.cpp"