#include "evaluator.h"
#include "basestat.h"
#include "enginetypes.h"

/**
 * decides whether the player can win
 * after applying move m
 */
int isWin(Configuration v, Move *m);

int evaluate(Configuration v, Move *m){
	int k=isWin(v, m);
	if (v->type==MAXNODE) {
		if (k>=5)
			return INFINITY;
		else
			return k+1;
	}
	else {
		if (k>=5)
			return -INFINITY;
		else
			return -k-1;
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