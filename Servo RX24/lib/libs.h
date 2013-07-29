/*
#-----------------------------------------------------------------------------
# Filename:    libs.h
#-----------------------------------------------------------------------------
#      ___  ____  ___  ____  _________ 
#     / _ \/ __ \/ _ )/ __ \/_  __/ _ |
#    / , _/ /_/ / _  / /_/ / / / / __ |
#   /_/|_|\____/____/\____/ /_/ /_/ |_|
#-----------------------------------------------------------------------------
#
# Summary:     Lib to serial and servo comunication
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
#include <math.h> 	  /* Math lib*/
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <pthread.h>  /* Lib for threads use */
#include <math.h>	  /* Lib for Math use */
#include <time.h>     /* Lib to use time functions */
#include "serial.h"   /* lib for serial comunication */
#include "protocolVals.h"  /* Protocol values to servo comunication  */
#include "protocolFunct.h"  /* Protocol functions to servo comunication  */
#include "Cronometer.cpp"	/* Lib for cronometer the time function */

/***************************LIBS*****************************/