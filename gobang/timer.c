#include <stdio.h>
#include <sys/timeb.h>
#include "timer.h"
#include "globalconst.h"

int _timer_counter;

struct timeb _timer_time, _timer_ori;

void initializeTimer(){
	resetTimer();
}

void resetTimer(){
	_timer_counter=0;
	ftime(&_timer_ori);
	ftime(&_timer_time);
}

int tickTimer(){
	++_timer_counter;
	if (_timer_counter % 50==0){
		ftime(&_timer_time);
	}
	if (_timer_time.time-_timer_ori.time<=timeLimitSecond()-1
			|| (_timer_time.time-_timer_ori.time==timeLimitSecond()
			   && _timer_time.millitm-_timer_ori.millitm<=timeLimitMillis()))
		return 1;
	else
		return 0;
}

void showTimer(){
	printf("timer: %d\n", _timer_counter);
}