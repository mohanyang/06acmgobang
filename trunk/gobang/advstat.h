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
};

typedef struct {
	/**
	 * compatible with the enum in this header
	 */
	int stat[10][2];
	Configuration assoc;
} AdvancedStat;

/**
 * the score of the board AFTER the 
 * col player has moved
 */
int getScore(AdvancedStat *info, PEBBLE_COLOR col);

void initializeAdvStat();

#endif
