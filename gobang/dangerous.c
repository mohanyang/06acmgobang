#include "advstat.h"
#include "dangerous.h"
#include "four_three.h"

enum {
	DEBUG_DANGER = 0
};

/**
 * returns 0 if (x,y) is not dangerous
 * 1 if is four danger
 * 2 if is active three danger
 * 3 if is 4+3 danger
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
// 				getchar();
			}
			return 1;
		}
		else if (v->statistics[x][y][ACTIVE_THREE][idx]){
			if (DEBUG_DANGER){
				printf("hazard active three %d %d\n", x, y);
// 				getchar();
			}
			return 2;
		}
		else if (four_three(v, x, y, p)){
			if (DEBUG_DANGER){
				printf("hazard %d %d\n", x, y);
// 				getchar();
			}
			return 3;
		}
		else
			return 0;
	}
	else
		return 0;
}