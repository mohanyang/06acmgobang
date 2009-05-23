#include "timer.h"

// TODO read from config or test when load
const int MAX_NODE_COUNT = 10000;

int _timer_counter;

void initializeTimer(){
	resetTimer();
}

void resetTimer(){
	_timer_counter=0;
}

int tickTimer(){
	++_timer_counter;
	return (_timer_counter<MAX_NODE_COUNT)?1:0;
}