#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "expansion.h"
#include "evaluator.h"
#include "enginetypes.h"
#include "moveheuristic.h"
#include "forbid.h"

typedef struct {
	Move m;
	int val;
} MoveListType;

struct ChildPointer {
	Configuration v;
	MoveListType movelist[225];
	int mllen;
	int currentidx;
};

struct ChildPointer _childitrcontainer[100];
int _childitrpointer=0;
int _forbid[15][15];

int _compMovesInc(const void *x, const void *y){
	return ((MoveListType*)x)->val-((MoveListType*)y)->val;
}

int _compMovesDec(const void *x, const void *y){
	return ((MoveListType*)y)->val-((MoveListType*)x)->val;
}

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=&_childitrcontainer[_childitrpointer];
	++_childitrpointer;
//	printf("child pointer %d\n", _childitrpointer);
	
	int i, j, k, l;
	retval->v=v;
	retval->mllen=0;
	retval->currentidx=0;
	// TODO what if no expansion is possible
	int target;
	int flag=0;
	PEBBLE_COLOR player=getMover(v);
	if (player==BLACK) {
		target=INFINITY;
		for (i=0; i<15; ++i)
			for (j=0; j<15; ++j)
				if (getColor(v, i, j)==NONE && 
					havePebbleAround(v, i, j)){
					if (forbid(v, i, j))
						_forbid[i][j]=-1;
					else
						_forbid[i][j]=1;
				}
				else
					_forbid[i][j]=0;
		// find hazards
		evaluateBoard(v, BLACK);
		for (i=0; i<15; ++i)
			for (j=0; j<15; ++j)
				if (isDangerous(v, i, j, WHITE) 
					&& _forbid[i][j]==1
				   	&& getColor(v, i, j)==NONE){
					retval->movelist[retval->mllen].m.x=i;
					retval->movelist[retval->mllen].m.y=j;
					++(retval->mllen);
				}
		if (retval->mllen==0) {
			// if there is no hazard
			for (i=0; i<15; ++i)
				for (j=0; j<15; ++j)
					if (isDangerous(v, i, j, WHITE)==0 
						&& _forbid[i][j]==1
					   	&& getColor(v, i, j)==NONE){
						retval->movelist[retval->mllen].m.x=i;
						retval->movelist[retval->mllen].m.y=j;
						retval->movelist[retval->mllen].val
								=getMoveEvaluate(v, i, j, &flag);
						if (flag){
							putPebble(v, i, j, player);
							retval->movelist[retval->mllen].val
									=evaluateBoard(v, player);
							removePebble(v, i, j);
						}
						++(retval->mllen);
					}
			if (retval->mllen==0){
				// TODO if still nothing found
			}
		}
		else {
			// if there is hazard, evaluate the score
			for (i=0; i<retval->mllen; ++i){
				retval->movelist[i].val
					=getMoveEvaluate(v, 
						retval->movelist[i].m.x,
	  					retval->movelist[i].m.y,
						&flag);
				if (flag){
					putPebble(v,
						retval->movelist[i].m.x,
	  					retval->movelist[i].m.y,
						player);
					retval->movelist[i].val
							=evaluateBoard(v, player);
					removePebble(v,
						retval->movelist[i].m.x,
						retval->movelist[i].m.y);
				}
			}
		}
		qsort(retval->movelist, retval->mllen,
			  sizeof(MoveListType), _compMovesDec);		
/*		if (retval->mllen>0){
			int i=0;
			while (retval->movelist[i].val>=INFINITY)
				++i;
			if (i>0) {
				retval->mllen=i;
			}
		}*/
	}
	else {
		target=-INFINITY;
// 		getchar();
		// find hazards
		evaluateBoard(v, WHITE);
		for (i=0; i<15; ++i)
			for (j=0; j<15; ++j)
				if (getColor(v, i, j)==NONE
					&& isDangerous(v, i, j, BLACK)) {
					retval->movelist[retval->mllen].m.x=i;
					retval->movelist[retval->mllen].m.y=j;
/*					printf("%d, %d\n", i, j);
					getchar();*/
					++(retval->mllen);
				}
		if (retval->mllen==0) {
			// if there is no hazard
			for (i=0; i<15; ++i)
				for (j=0; j<15; ++j)
					if (isDangerous(v, i, j, BLACK)==0
						&& getColor(v, i, j)==NONE
						&& havePebbleAround(v, i, j)){
						retval->movelist[retval->mllen].m.x=i;
						retval->movelist[retval->mllen].m.y=j;
						retval->movelist[retval->mllen].val
								=getMoveEvaluate(v, i, j, &flag);
						if (flag){
							putPebble(v, i, j, player);
							retval->movelist[retval->mllen].val
									=evaluateBoard(v, player);
							removePebble(v, i, j);
						}
						++(retval->mllen);
					}
			if (retval->mllen==0){
				// TODO if still nothing found
			}
		}
		else {
			// if there is hazard, evaluate the score
			for (i=0; i<retval->mllen; ++i){
				retval->movelist[i].val
					=getMoveEvaluate(v, 
						retval->movelist[i].m.x,
						retval->movelist[i].m.y,
						&flag);
				if (flag){
					putPebble(v,
						retval->movelist[i].m.x,
		 				retval->movelist[i].m.y,
   						player);
					retval->movelist[i].val
						=evaluateBoard(v, player);
					removePebble(v,
						retval->movelist[i].m.x,
						retval->movelist[i].m.y);
				}
			}
		}
		qsort(retval->movelist, retval->mllen,
			  sizeof(MoveListType), _compMovesInc);
/*		if (retval->mllen>0){
			int i=0;
			while (retval->movelist[i].val<=-INFINITY)
				++i;
			if (i>0) {
				retval->mllen=i;
			}
		}*/
	}
/*	printf(">>>>>>>>>>>>\n");
	printf("altogether %d\n", retval->mllen);
	for (i=0; i<retval->mllen; ++i)
		printf("%d, %d\n", retval->movelist[i].m.x, retval->movelist[i].m.y);
	printf("<<<<<<<<<<<<\n");
	getchar();*/
	return retval;
}

void getNext(ChildIterator *itr) {
	if (*itr==NULL)
		return;
	++((*itr)->currentidx);
	if ((*itr)->currentidx>=(*itr)->mllen) {
		--_childitrpointer;
		*itr=NULL;
	}
}

/**
 * should assert that itr is not NULL when calling
 */
Move getCurrent(ChildIterator itr) {
	return (itr->movelist[itr->currentidx]).m;
}

int getCurrentValue(ChildIterator itr) {
	return (itr->movelist[itr->currentidx]).val;
}

void releaseChildIterator(ChildIterator itr){
	if (itr!=NULL)
		--_childitrpointer;
//	printf("childitrpointer %d\n", _childitrpointer);
}