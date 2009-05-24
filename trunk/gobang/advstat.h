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
	ACTIVE_TWO=0,
	ACTIVE_THREE=1,
	ACTIVE_FOUR=2,
	FIVE=3,
	DFOUR=4, 
	AFOUR=5, 
	FTHREE=6, 
	DTHREE=7
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
	int stat[8][2];
	// black=0, white=1
	int score[2];
} AdvancedStat;

void calculate(Configuration v);

int getScore(PEBBLE_COLOR col);

#endif
