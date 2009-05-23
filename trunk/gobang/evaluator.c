#include "evaluator.h"
#include "basestat.h"
#include "enginetypes.h"

int _priority[15][15];

/**
 * decides whether the player can win
 * after applying move m
 */
int isWin(Configuration v, Move *m);

int evaluateBoard(Configuration v, PEBBLE_COLOR c){
	int i,j,k=0;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==c)
				k+=_priority[i,j];
	if (c==BLACK)
		return k;
	else
		return -k;
}

int evaluate(Configuration v, Move *m){
	int k=isWin(v, m);
	if (v->type==MAXNODE) {
		if (k>=5)
			return INFINITY;
		else
			return _priority[m->x][m->y];
	}
	else {
		if (k>=5)
			return -INFINITY;
		else
			return -_priority[m->x][m->y];
	}
}

int isWin(Configuration v, Move *m){
	PEBBLE_COLOR t=getMover(v);
	int mx=getConsecutive(v, t, m, N)+getConsecutive(v, t, m, S);
	int tmp=getConsecutive(v, t, m, W)+getConsecutive(v, t, m, E);
	if (tmp>mx)
		mx=tmp;
	tmp=getConsecutive(v, t, m, NW)+getConsecutive(v, t, m, SE);
	if (tmp>mx)
		mx=tmp;
	tmp=getConsecutive(v, t, m, SW)+getConsecutive(v, t, m, NE);
	if (tmp>mx)
		mx=tmp;
	return mx;
}

void initializeEvaluate(){
	int i,j,k;
	for (i=0; i<8; ++i)
		for (j=0; j<8; ++j){
			if (i>j)
				k=j;
			else
				k=i;
			_priority[i][j]=k;
			_priority[i][15-1-j]=k;
			_priority[15-1-i][j]=k;
			_priority[15-1-i][15-1-j]=k;
		}
}