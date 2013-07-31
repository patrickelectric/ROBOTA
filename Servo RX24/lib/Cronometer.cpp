#include "Cronometer.h"

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <string.h>


using namespace std;



void Cronometer::init(char *Name){
  	cronoName = Name;
  	cronoTimeElapsed = 0;
  	cronoActive = 0;
  	freqActive = 0;
  	logControl = 0;
  	printf("%s inicializado.\n", cronoName);
}

void Cronometer::startCrono(){
	cronoTimeElapsed = 0;
  	cronoActive = 0;
  	freqActive = 0;
  	logControl = 0;

	if(!cronoActive){
		clock_gettime(CLOCK_MONOTONIC ,&cronoTime0);
		cronoActive = 1;
	/*
		if(cronoName!=NULL)
		{
			printf("%s: Cronometer mode actived.\n", cronoName);
		}
		*/
	}
	/*
	else{
		printf("%s: Cronometer mode aready in use.\n", cronoName);
	}
	*/
}

float Cronometer::finishCrono(bool check){
	double difSec=0,difNsec=0;
	clock_gettime(CLOCK_MONOTONIC ,&cronoTime1);
	difSec  = cronoTime1.tv_sec  - cronoTime0.tv_sec;
	difNsec = cronoTime1.tv_nsec - cronoTime0.tv_nsec;
	cronoTimeElapsed = difSec + difNsec*0.000000001;
	cronoActive =0;
	if(check==true)
	{
		if(cronoName!=NULL)
			printf("%s: messure: %f (s)\n",cronoName,cronoTimeElapsed);
		else
			printf("messure: %f (s)\n",cronoTimeElapsed);
	}
	return cronoTimeElapsed;
}

void Cronometer::startFreq(){
	if(!freqActive){
		clock_gettime(CLOCK_MONOTONIC ,&freqTimeLast);
		freqActive = 1;
		printf("%s: Frequencimeter mode actived.\nWork pretty weel for low Frequencies\n", cronoName);
	}
	else{
		printf("%s: Frequencimeter mode aready in use.\n", cronoName);
	}
}

void Cronometer::lapFreq(){
	double difSec=0,difNsec=0;
	clock_gettime(CLOCK_MONOTONIC ,&freqTimeNow);
	difSec  = freqTimeNow.tv_sec  - freqTimeLast.tv_sec;
	difNsec = freqTimeNow.tv_nsec - freqTimeLast.tv_nsec;
	freq = 1/(difSec + difNsec*0.000000001);
	freqTimeLast.tv_sec = freqTimeNow.tv_sec;
	freqTimeLast.tv_nsec = freqTimeNow.tv_nsec;

	freqLog[logControl] = freq;
	logControl++;
	if(logControl>9) logControl=0;
}

void Cronometer::stopFreq(){
	freqActive = 0;
	printf("%s: Frequencimeter mode desatived.\n", cronoName);
}

void Cronometer::showFreq(){
	double showfreq=0;
	if(freq>1000000000){
		showfreq = freq/1000000000;
		printf("%s Freq: %.3f (GHz)\n",cronoName,showfreq);
	}
	else if(freq>1000000){
		showfreq = freq/1000000;
		printf("%s Freq: %.3f (MHz)\n",cronoName,showfreq);
	}
	else if(freq>1000){
		showfreq = freq/1000;
		printf("%s Freq: %.3f (kHz)\n",cronoName,showfreq);
	}
	else
		printf("%s Freq: %.3f (Hz)\n",cronoName,freq);	
}

void Cronometer::showAverangeFreq(int n){
	int i,aux;
	double sum=0;
	aux = logControl;
	for(i=0;i<n;i++){
		sum = sum+freqLog[aux];
		aux--;
		if(aux<0)aux=9;
	}
	printf("%s: Averange Freq %.3f (Hz)\n",cronoName,sum/n);
}