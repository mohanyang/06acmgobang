#include <string.h>
#include <stdio.h>
#include "advstat.h"
#include "basestat.h"
#include "enginetypes.h"

// TODO need to cache this
void calculate(Configuration v){
}

int getScore(AdvancedStat *info, PEBBLE_COLOR col){
	switch (col){
		case BLACK:
			if (info->stat[FIVE][1])
				return -INFINITY-1;
			else if (info->stat[FIVE][0])
				return INFINITY+1;
			else if (info->stat[DFOUR][1])
				return -INFINITY;
			else if (info->stat[ACTIVE_FOUR][1])
				return -INFINITY;
			else if (info->stat[AFOUR][1])
				return -INFINITY;
			else if (info->stat[FTHREE][1])
				return -INFINITY;
			else if (info->stat[DFOUR][0])
				return INFINITY;
			else if (info->stat[ACTIVE_FOUR][0])
				return INFINITY;
			else if (info->stat[FTHREE][0])
				return INFINITY;
			else if (info->stat[DTHREE][1])
				return -INFINITY;
			else if (info->stat[ACTIVE_THREE][1])
				return -INFINITY;
			else if (info->stat[DTHREE][0])
				return 900;
			else if (info->stat[AFOUR][0])
				return 850;
			else if (info->stat[ACTIVE_THREE][0])
				return 800;
			else {
				return (info->stat[ACTIVE_TWO][0]*2
						-info->stat[ACTIVE_TWO][1]);
			}
		case WHITE:
			if (info->stat[FIVE][0])
				return INFINITY+1;
			else if (info->stat[FIVE][1])
				return -INFINITY-1;
			else if (info->stat[DFOUR][0])
				return INFINITY;
			else if (info->stat[ACTIVE_FOUR][0])
				return INFINITY;
			else if (info->stat[AFOUR][0])
				return INFINITY;
			else if (info->stat[FTHREE][0])
				return INFINITY;
			else if (info->stat[DFOUR][1])
				return -INFINITY;
			else if (info->stat[ACTIVE_FOUR][1])
				return -INFINITY;
			else if (info->stat[FTHREE][1])
				return -INFINITY;
			else if (info->stat[DTHREE][0])
				return INFINITY;
			else if (info->stat[ACTIVE_THREE][0])
				return INFINITY;
			else if (info->stat[DTHREE][1])
				return -900;
			else if (info->stat[AFOUR][1])
				return -850;
			else if (info->stat[ACTIVE_THREE][1])
				return -800;
			else {
				return -(info->stat[ACTIVE_TWO][1]*2
						+info->stat[ACTIVE_TWO][0]);
			}
		default:
			return 0;
	}
}
