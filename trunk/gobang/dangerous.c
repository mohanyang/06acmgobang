#include "advstat.h"
#include "dangerous.h"

/**
 * returns 0 if (x,y) is not dangerous
 */
int isDangerous(Configuration v, int x, int y, PEBBLE_COLOR p){
	int idx=(p==BLACK)?0:1;
/*	printBoardNonBlock(v);
	printf("%d %d %d\n", x, y, idx);
	printf("%d\n", v->statistics[x][y][ACTIVE_THREE][idx]);
	if (x==7 && y==10)
		getchar();*/
	
	if (getColor(v, x, y)==NONE){
		if (v->statistics[x][y][AFOUR][idx]
		   || v->statistics[x][y][ACTIVE_FOUR][idx]){
// 			printf("hazard four %d %d\n", x, y);
			return 1;
		}
		else if (v->statistics[x][y][ACTIVE_THREE][idx]){
// 			printf("hazard active three %d %d\n", x, y);
			return 1;
		}
		else if (v->statistics[x][y][SLEEPY_THREE][idx]
				&& v->statistics[x][y][ACTIVE_TWO][idx]){
// 			printf("hazard %d %d\n", x, y);
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}