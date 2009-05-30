#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "advstat.h"
#include "basestat.h"
#include "enginetypes.h"


int getScore(AdvancedStat *info, PEBBLE_COLOR col){
/*	int i, j;
	printf("eeeeeeeeeeeeeee\n");
	for (i=1; i<=5; ++i)
		for (j=0; j<2; ++j)
			printf("%d ", info->stat[i][j]);
	printf("\ndddddddddddddddd\n");*/
	switch (col){
		case BLACK:
			if (info->stat[FIVE][1])
				return -FIVE_SCORE;
			else if (info->stat[FIVE][0])
				return FIVE_SCORE;
			else if (info->stat[ACTIVE_FOUR][1])
				return -FOUR_SCORE;
			else if (info->stat[AFOUR][1])
				return -FOUR_SCORE;
			else if (info->stat[AFOUR][0]>1)
				return DFOUR_SCORE;
			else if (info->stat[ACTIVE_FOUR][0])
				return FOUR_SCORE;
			else if (info->stat[AFOUR][0] &&
							  info->stat[ACTIVE_THREE][0])
				return FTHREE_SCORE;
			else if (info->stat[ACTIVE_THREE][1])
				return -ATHREE_SCORE;
			else if (info->stat[ACTIVE_THREE][0]>=2)
				return DTHREE_SCORE;
			else if (info->stat[ACTIVE_THREE][0]){
				if (info->stat[SLEEPY_THREE][1])
					return 350;
				else
					return 600;
			}
			else if (info->stat[AFOUR][0])
				return 300;
			else {
			  int k=info->assoc->localPriority;
				return (info->stat[SLEEPY_THREE][0]*10
						+info->stat[ACTIVE_TWO][0]*5
						-info->stat[SLEEPY_THREE][1]*3)*2
						+k;
			}
		case WHITE:
			if (info->stat[FIVE][0])
				return FIVE_SCORE;
			else if (info->stat[FIVE][1])
				return -FIVE_SCORE;
			else if (info->stat[ACTIVE_FOUR][0])
				return FOUR_SCORE;
			else if (info->stat[AFOUR][0])
				return FOUR_SCORE;
			else if (info->stat[AFOUR][1]>1)
				return -DFOUR_SCORE;
			else if (info->stat[ACTIVE_FOUR][1])
				return -FOUR_SCORE;
			else if (info->stat[AFOUR][1] &&
							  info->stat[ACTIVE_THREE][1])
				return -FTHREE_SCORE;
			else if (info->stat[ACTIVE_THREE][0])
				return ATHREE_SCORE;
			else if (info->stat[ACTIVE_THREE][1]>=2)
				return -DTHREE_SCORE;
			else if (info->stat[ACTIVE_THREE][1]){
				if (info->stat[SLEEPY_THREE][0])
					return -350;
				else
					return -600;
			}
			else if (info->stat[AFOUR][1])
				return -300;
			else {
			  /*
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
			  */
				int k = info->assoc->localPriority;

				return (-info->stat[SLEEPY_THREE][1]*10
						-info->stat[ACTIVE_TWO][1]*5
						+info->stat[SLEEPY_THREE][1]*3)*2
						+k;
			}
		default:
			return 0;
	}
}

void initializeAdvStat(){
}
