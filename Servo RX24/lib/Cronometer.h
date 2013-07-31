#ifndef CRONOMETER_H
#define CRONOMETER_H

#include <time.h>

class Cronometer{
	private:
		char *cronoName;
		//Cronometro tempo corrido
		struct timespec cronoTime0,cronoTime1;
		double cronoTimeElapsed;
		int cronoActive;

		//Frequencimetro
		struct timespec freqTimeLast,freqTimeNow;
		int logControl;
		double freq,freqLog[10];
		int freqActive;
	public:
		void init(char *Name);
		void startCrono();
		float finishCrono(bool check);
		void startFreq();
		void stopFreq();
		void lapFreq();
		void showFreq();
		void showAverangeFreq(int n); //Max of 10
};

#endif