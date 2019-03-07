#include "TimeKeeper.h"

TimeKeeper::TimeKeeper() {
	startTime	= 0;
	fcount		= 0;
	fps			= 0;
	allCount	= 0;
}

float TimeKeeper::record() {
	if (fcount == 0) startTime = GetNowCount();
	if (fcount == N) {
		int t	  = GetNowCount();
		fps		  = 1000.f / ((t - startTime) / (float)N);
		fcount	  = 0;
		startTime = t;
	}
	fcount++;
	return fps;
}

unsigned long int TimeKeeper::getCount() {
	return allCount;
}

void TimeKeeper::wait() {
	int tookTime = GetNowCount() - startTime;
	int waitTime = fcount * 1000 / FPS - tookTime;
	if (allCount == 4294967295) allCount = 0;
	allCount++;
	if (waitTime > 0) Sleep(waitTime);
}

void TimeKeeper::init(){
	startTime	= 0;
	fcount		= 0;
	fps			= 0;
	allCount	= 0;
}