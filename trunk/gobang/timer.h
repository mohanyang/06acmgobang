#ifndef _TIMER_H_
#define _TIMER_H_

// TODO test CPU speed
void initializeTimer();

/**
 * reset the clock counter
 */
void resetTimer();

/**
 * increases the clock counter, and if
 * the clock reaches the upperbound, return
 * false
 */
int tickTimer();

void showTimer();

#endif
