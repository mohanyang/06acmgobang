#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "advstat.h"
#include "basestat.h"
#include "enginetypes.h"

int _localpriority[15][15];

int getScore(AdvancedStat *info, PEBBLE_COLOR col){
/*	int i, j;
	for (i=1; i<=5; ++i)
		for (j=0; j<2; ++j)
			printf("%d ", info->stat[i][j]);
	printf("\n");*/
	switch (col){
		case BLACK:
			if (info->stat[FIVE][1])
				return -INFINITY-1;
			else if (info->stat[FIVE][0])
				return INFINITY+1;
			else if (info->stat[ACTIVE_FOUR][1])
				return -INFINITY;
			else if (info->stat[AFOUR][1])
				return -INFINITY;
			else if (info->stat[ACTIVE_FOUR][0])
				return INFINITY;
			else if (info->stat[AFOUR][0] &&
							  info->stat[ACTIVE_THREE][0])
				return INFINITY;
			else if (info->stat[ACTIVE_THREE][1])
				return -INFINITY;
			else if (info->stat[ACTIVE_THREE][0]>=2)
				return INFINITY;
			else if (info->stat[ACTIVE_THREE][0])
				return 800;
			else if (info->stat[AFOUR][0])
				return 700;
			else {
				// TODO quick calculation
				int i, j, k=0;
				for (i=0; i<15; ++i)
					for (j=0; j<15; ++j)
						switch (getColor(info->assoc, i, j)){
							case BLACK:
								k+=_localpriority[i][j];
								break;
							case WHITE:
								k-=_localpriority[i][j];
								break;
							default:
								break;
						}
				return (info->stat[SLEEPY_THREE][0]*2
						-info->stat[SLEEPY_THREE][1])*100
						+k*100;
			}
		case WHITE:
			if (info->stat[FIVE][0])
				return +INFINITY+1;
			else if (info->stat[FIVE][1])
				return -INFINITY-1;
			else if (info->stat[ACTIVE_FOUR][0])
				return INFINITY;
			else if (info->stat[AFOUR][0])
				return INFINITY;
			else if (info->stat[ACTIVE_FOUR][1])
				return -INFINITY;
			else if (info->stat[AFOUR][1] &&
							  info->stat[ACTIVE_THREE][1])
				return -INFINITY;
			else if (info->stat[ACTIVE_THREE][0])
				return INFINITY;
			else if (info->stat[ACTIVE_THREE][1]>=2)
				return -INFINITY;
			else if (info->stat[ACTIVE_THREE][1])
				return -800;
			else if (info->stat[AFOUR][1])
				return -700;
			else {
				int i, j, k=0;
				for (i=0; i<15; ++i)
					for (j=0; j<15; ++j)
						switch (getColor(info->assoc, i, j)){
							case BLACK:
								k+=_localpriority[i][j];
								break;
							case WHITE:
								k-=_localpriority[i][j];
								break;
							default:
								break;
						}
				return (-info->stat[SLEEPY_THREE][0]
						+2*info->stat[SLEEPY_THREE][1])*100
						-k*100;
			}
		default:
			return 0;
	}
}

void initializeAdvStat(){
	int i, j;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			_localpriority[i][j]=7-
					abs(7-i)>abs(7-j)?
					abs(7-i):abs(7-j);
}
