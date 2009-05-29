#include "advstat.h"
#include "dangerous.h"

enum {
	DEBUG_DANGER = 0
};

/**
 * returns 0 if (x,y) is not dangerous
 * otherwise, returns the minimum number of 
 * defeating step if the current move is not
 * dealt with.
 * note that since 4+3 pattern can not be recognized
 * correctly, we return a 3, actually it is 2
 */
int isDangerous(Configuration v, int x, int y, PEBBLE_COLOR p){
	int idx=(p==BLACK)?0:1;
	
/*	printBoardNonBlock(v);
	printf("%d %d %d\n", x, y, idx);*/
	
	if (getColor(v, x, y)==NONE){
		if (v->statistics[x][y][AFOUR][idx]
		   || v->statistics[x][y][ACTIVE_FOUR][idx]){
			if (DEBUG_DANGER){
				printf("hazard four %d %d\n", x, y);
			}
			return 1;
		}
		else if (v->statistics[x][y][ACTIVE_THREE][idx]){
			if (DEBUG_DANGER){
				printf("hazard active three %d %d\n", x, y);
			}
			return 2;
		}
		else if (v->statistics[x][y][SLEEPY_THREE][idx]
				&& v->statistics[x][y][ACTIVE_TWO][idx]){
			if (DEBUG_DANGER){
				printf("hazard %d %d\n", x, y);
			}
			return 3;
		}
		else
			return 0;
	}
	else
		return 0;
}