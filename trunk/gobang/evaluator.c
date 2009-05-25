#include <stdio.h>
#include <stdlib.h>
#include "evaluator.h"
#include "basestat.h"
#include "enginetypes.h"
#include "advstat.h"
#include "assert.h"

int _priority[15][15];

/**
 * decides whether the player can win
 * after applying move m
 */
int isWin(Configuration v, Move *m);

EvalRetVal evaluateBoard(Configuration v, PEBBLE_COLOR c){
	EvalRetVal ret;
	if (c==BLACK){
		Move t, opt;
		int val=-INFINITY-1, cval;
		for (t.x=0; t.x<15; ++t.x)
			for (t.y=0; t.y<15; ++t.y)
				if (getColor(v, t.x, t.y)==NONE){
					cval=evaluate(v, &t);
					if (cval>val){
						opt=t;
						val=cval;
					}
				}
		ret.value=val;
		ret.mv=opt;
	}
	else {
		Move t, opt;
		int val=INFINITY+1, cval;
		for (t.x=0; t.x<15; ++t.x)
			for (t.y=0; t.y<15; ++t.y)
				if (getColor(v, t.x, t.y)==NONE){
					cval=evaluate(v, &t);
					if (cval<val){
						opt=t;
						val=cval;
					}
				}
		ret.value=val;
		ret.mv=opt;
	}
	return ret;
}

int evaluate(Configuration v, Move *m){
	applyMove(v, *m);
	calculate(v);
	int ret=getScore(getMover(v));
	if (abs(ret)<200){
		int i,j;
//		printf("* %d\n", ret);
		for (i=0; i<15; ++i)
			for (j=0; j<15; ++j)
				switch (getColor(v, i, j)){
					case BLACK:
						ret+=_priority[i][j];
						break;
					case WHITE:
						ret-=_priority[i][j];
						break;
					default:
						break;
				}
	}
	undoMove(v, *m);
	assert(abs(ret)<=1000);
	return ret;
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
//	if (mx>=3)
//		printf("iswin return %d\n", mx);
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