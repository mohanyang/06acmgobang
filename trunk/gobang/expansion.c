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
					retval->movelist[retval->mllen].val=2*INFINITY;
					++(retval->mllen);
				}
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
/*		else {
		}*/
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
		// if there is hazard, evaluate the score
		l=0;
		while (l<retval->mllen && retval->movelist[l].val==2*INFINITY){
			retval->movelist[l].val
				=getMoveEvaluate(v, 
					retval->movelist[l].m.x,
		  			retval->movelist[l].m.y,
					&flag);
			if (flag){
				putPebble(v,
					retval->movelist[l].m.x,
					retval->movelist[l].m.y,
  					player);
				retval->movelist[l].val
					=evaluateBoard(v, player);
				removePebble(v,
					retval->movelist[l].m.x,
					retval->movelist[l].m.y);
			}
			++l;
		}
		qsort(retval->movelist, l, sizeof(MoveListType),
			  _compMovesDec);
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
					retval->movelist[retval->mllen].val=-2*INFINITY;
					++(retval->mllen);
				}
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
		qsort(retval->movelist, retval->mllen,
			  sizeof(MoveListType), _compMovesInc);
		l=0;
		while (l<retval->mllen && retval->movelist[l].val==-2*INFINITY){
			retval->movelist[l].val
				=getMoveEvaluate(v, 
					retval->movelist[l].m.x,
					retval->movelist[l].m.y,
					&flag);
			if (flag){
				putPebble(v,
					retval->movelist[l].m.x,
					retval->movelist[l].m.y,
  					player);
				retval->movelist[l].val
					=evaluateBoard(v, player);
				removePebble(v,
					retval->movelist[l].m.x,
					retval->movelist[l].m.y);
			}
			++l;
		}
		qsort(retval->movelist, l, sizeof(MoveListType),
			  _compMovesInc);
	}
/*	printf(">>>>>>>>>>>>\n");
	printf("altogether %d\n", retval->mllen);
	for (i=0; i<retval->mllen; ++i)
		printf("%d, %d\n", retval->movelist[i].m.x, retval->movelist[i].m.y);
	printf("<<<<<<<<<<<<\n");*/
// 	getchar();
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