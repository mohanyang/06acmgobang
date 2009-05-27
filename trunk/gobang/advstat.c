#include <string.h>
#include <stdio.h>
#include "advstat.h"
#include "basestat.h"
#include "enginetypes.h"

AdvancedStat into->

// TODO need to cache this
void calculate(Configuration v){
	memset((void*)(&into->, 0, sizeof(AdvancedStat));
	int i, j;
	int k1, k2;
	int l1, l2;
	int s;
	PEBBLE_COLOR cur;
	Move temp;
	int a[6], s4;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if ((cur=getColor(v, i, j))!=NONE) {
				memset(a, 0, sizeof(a));
				s4=0;
				temp.x=i;
				temp.y=j;
				
				k1=getConsecutive(v, cur, &temp, N);
				k2=getConsecutive(v, cur, &temp, S);
				l1=getClosestOpponent(v, cur, &temp, N);
				l2=getClosestOpponent(v, cur, &temp, S);
				s=(l1>k1+1)+(l2>k2+1);
				//printf("%d %d %d %d %d %d %d NS\n",i, j, k1, k2, l1, l2, s);
				if (k1+k2+1<4){
					if (s>1){
						++a[k1+k2+1];
					}
				}
				else if (k1+k2+1>=5){
					++a[5];
				}
				else {
					if (s==2){
						++a[4];
					}
					else if (s==1){
						++s4;
					}
				}
				
				k1=getConsecutive(v, cur, &temp, E);
				k2=getConsecutive(v, cur, &temp, W);
				l1=getClosestOpponent(v, cur, &temp, E);
				l2=getClosestOpponent(v, cur, &temp, W);
				s=(l1>k1+1)+(l2>k2+1);
				//printf("%d %d %d %d %d %d %d EW\n",i, j, k1, k2, l1, l2, s);
				if (k1+k2+1<4){
					if (s>1)
						++a[k1+k2+1];
				}
				else if (k1+k2+1>=5){
					++a[5];
				}
				else {
					if (s==2){
						++a[4];
					}
					else if (s==1){
						++s4;
					}
				}
				
				k1=getConsecutive(v, cur, &temp, NE);
				k2=getConsecutive(v, cur, &temp, SW);
				l1=getClosestOpponent(v, cur, &temp, NE);
				l2=getClosestOpponent(v, cur, &temp, SW);
				s=(l1>k1+1)+(l2>k2+1);
				//printf("%d %d %d %d %d %d %d NE-SW\n",i, j, k1, k2, l1, l2, s);
				if (k1+k2+1<4){
					if (s>1)
						++a[k1+k2+1];
				}
				else if (k1+k2+1>=5){
					++a[5];
				}
				else {
					if (s==2){
						++a[4];
					}
					else if (s==1){
						++s4;
					}
				}
				
				k1=getConsecutive(v, cur, &temp, NW);
				k2=getConsecutive(v, cur, &temp, SE);
				l1=getClosestOpponent(v, cur, &temp, NW);
				l2=getClosestOpponent(v, cur, &temp, SE);
				s=(l1>k1+1)+(l2>k2+1);
				//printf("%d %d %d %d %d %d %d NW-SE\n",i, j, k1, k2, l1, l2, s);
				if (k1+k2+1<4){
					if (s>1)
						++a[k1+k2+1];
				}
				else if (k1+k2+1>=5){
					++a[5];
				}
				else {
					if (s==2){
						++a[4];
					}
					else if (s==1){
						++s4;
					}
				}
				
				if (cur==BLACK){
					if (a[5])
						++into->stat[FIVE][0];
					switch (a[4]){
						case 0:
							break;
						case 1:
							++into->stat
									[ACTIVE_FOUR][0];
							break;
						default:
							++into->stat[DFOUR][0];
					}
					switch (a[3]){
						case 0:
							break;
						case 1:
//							printf("black athree %d %d\n", i, j);
							++into->stat
									[ACTIVE_THREE][0];
							break;
						default:
							++into->stat[DTHREE][0];
					}
					if (a[2]){
						++into->stat[ACTIVE_TWO][0];
					}
					if (s4) {
						if (a[4])
							++into->stat[DFOUR][0];
						else
							++into->stat[AFOUR][0];
					}
					if ((a[4] || s4) && a[3])
						++into->stat[FTHREE][0];
				}
				else {
					if (a[5])
						++into->stat[FIVE][1];
					switch (a[4]){
						case 0:
							break;
						case 1:
							++into->stat
									[ACTIVE_FOUR][1];
							break;
						default:
							++into->stat[DFOUR][1];
					}
					switch (a[3]){
						case 0:
							break;
						case 1:
							++into->stat
									[ACTIVE_THREE][1];
							//printf("white athree %d %d\n", i, j);
							break;
						default:
							++into->stat[DTHREE][1];
					}
					if (a[2]){
						++into->stat[ACTIVE_TWO][1];
					}
					if (s4) {
						if (a[4])
							++into->stat[DFOUR][1];
						else
							++into->stat[AFOUR][1];
					}
					if ((a[4] || s4) && a[3])
						++into->stat[FTHREE][1];
				}
			}
}

int getScore(AdvancedStat *info, PEBBLE_COLOR col){
	switch (col){
		case BLACK:
			if (into->stat[FIVE][1])
				return -INFINITY-1;
			else if (into->stat[FIVE][0])
				return INFINITY+1;
			else if (into->stat[DFOUR][1])
				return -INFINITY;
			else if (into->stat[ACTIVE_FOUR][1])
				return -INFINITY;
			else if (into->stat[AFOUR][1])
				return -INFINITY;
			else if (into->stat[FTHREE][1])
				return -INFINITY;
			else if (into->stat[DFOUR][0])
				return INFINITY;
			else if (into->stat[ACTIVE_FOUR][0])
				return INFINITY;
			else if (into->stat[FTHREE][0])
				return INFINITY;
			else if (into->stat[DTHREE][1])
				return -INFINITY;
			else if (into->stat[ACTIVE_THREE][1])
				return -INFINITY;
			else if (into->stat[DTHREE][0])
				return 900;
			else if (into->stat[AFOUR][0])
				return 850;
			else if (into->stat[ACTIVE_THREE][0])
				return 800;
			else {
				return (into->stat[ACTIVE_TWO][0]*2
						-into->stat[ACTIVE_TWO][1]);
			}
		case WHITE:
			if (into->stat[FIVE][0])
				return INFINITY+1;
			else if (into->stat[FIVE][1])
				return -INFINITY-1;
			else if (into->stat[DFOUR][0])
				return INFINITY;
			else if (into->stat[ACTIVE_FOUR][0])
				return INFINITY;
			else if (into->stat[AFOUR][0])
				return INFINITY;
			else if (into->stat[FTHREE][0])
				return INFINITY;
			else if (into->stat[DFOUR][1])
				return -INFINITY;
			else if (into->stat[ACTIVE_FOUR][1])
				return -INFINITY;
			else if (into->stat[FTHREE][1])
				return -INFINITY;
			else if (into->stat[DTHREE][0])
				return INFINITY;
			else if (into->stat[ACTIVE_THREE][0])
				return INFINITY;
			else if (into->stat[DTHREE][1])
				return -900;
			else if (into->stat[AFOUR][1])
				return -850;
			else if (into->stat[ACTIVE_THREE][1])
				return -800;
			else {
				return -(into->stat[ACTIVE_TWO][1]*2
						+into->stat[ACTIVE_TWO][0]);
			}
		default:
			return 0;
	}
}

void dumpcalc(){
	int i,j;
	for (i=0; i<8; ++i)
		for (j=0; j<2; ++j)
			printf("%d ", into->stat[i][j]);
	printf("\n");
}