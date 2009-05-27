#ifndef _ADVSTAT_H_
#define _ADVSTAT_H_

#include "basetypes.h"

/**
 * calculate advanced statistics of a 
 * configuration
 * NOTE: this implementation is not thread
 * safe
 */

enum {
	FIVE = 1,
	ACTIVE_FOUR = 2,
	AFOUR = 3, 
	ACTIVE_THREE = 4,
	SLEEPY_THREE = 5,
	ACTIVE_TWO = 6,
	

	FTHREE = 7, 
	DTHREE = 8,
	DFOUR = 9, 
};

typedef struct {
	/**
	 * 0 - active two
	 * 1 - active three
	 * 2 - active four
	 * 3 - consecutive five
	 * 4 - double four
	 * 5 - about four
	 * 6 - four + three
	 * 7 - double three
	 */
	int stat[10][2];
} AdvancedStat;

void calculate(Configuration v);

/**
 * the score of the board after the 
 * col player has moved
 */
int getScore(PEBBLE_COLOR col);

#endif
