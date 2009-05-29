#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "advstat.h"
#include "expansion.h"
#include "evaluator.h"
#include "enginetypes.h"
#include "moveheuristic.h"
#include "forbid.h"
#include "dangerous.h"

enum {
	DEBUG_EXPAND = 1
};

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

int getEvaluateForMove(Configuration v, PEBBLE_COLOR col, int x, int y){
	int flag;
	int ret=getMoveEvaluate(v, x, y, &flag);
	if (flag){
		putPebble(v, x, y, col);
		ret=evaluateBoard(v, col);
		removePebble(v, x, y);
	}
	return ret;
}

MoveListType dangerthree[225];
int dangerthreecount;
MoveListType dangerfour[225];
int dangerfourcount;
MoveListType dangerfthree[225];
int dangerfthreecount;
MoveListType winningfive[225];
int winningfivecount;
MoveListType winningfour[225];
int winningfourcount;
MoveListType winningfthree[225];
int winningfthreecount;
MoveListType winningdthree[225];
int winningdthreecount;
MoveListType ordinarymove[225];
int ordinarymovecount;
MoveListType forbiddenmove[225];
int forbiddenmovecount;
// int marked[15][15];

void dumpAll(){
	int i;
	printf("dangerthree %d\n", dangerthreecount);
	for (i=0; i<dangerthreecount; ++i)
		printf("(%d, %d)\n", dangerthree[i].m.x, dangerthree[i].m.y);
	printf("dangerfour %d\n", dangerfourcount);
	for (i=0; i<dangerfourcount; ++i)
		printf("(%d, %d)\n", dangerfour[i].m.x, dangerfour[i].m.y);
	printf("dangerfthree %d\n", dangerfthreecount);
	for (i=0; i<dangerfthreecount; ++i)
		printf("(%d, %d)\n", dangerfthree[i].m.x, dangerfthree[i].m.y);
	printf("winningfive %d\n", winningfivecount);
	for (i=0; i<winningfivecount; ++i)
		printf("(%d, %d)\n", winningfive[i].m.x, winningfive[i].m.y);
	printf("winningfour %d\n", winningfourcount);
	for (i=0; i<winningfourcount; ++i)
		printf("(%d, %d)\n", winningfour[i].m.x, winningfour[i].m.y);
	printf("winningfthree %d\n", winningfthreecount);
	for (i=0; i<winningfthreecount; ++i)
		printf("(%d, %d)\n", winningfthree[i].m.x, winningfthree[i].m.y);
	printf("winningdthree %d\n", winningdthreecount);
	for (i=0; i<winningdthreecount; ++i)
		printf("(%d, %d)\n", winningdthree[i].m.x, winningdthree[i].m.y);
}

void expandBlack(Configuration v, ChildIterator retval){
	int i, j, temp;
	int k;
// 	memset(marked, 0, sizeof(marked));
	dangerthreecount=dangerfourcount=dangerfthreecount
		=winningfivecount=winningfourcount=winningfthreecount
		=winningdthreecount=ordinarymovecount=forbiddenmovecount=0;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE && 
				havePebbleAround(v, i, j)){
				if (forbid(v, i, j)) {
					_forbid[i][j]=-1;
					forbiddenmove[forbiddenmovecount].m.x=i;
					forbiddenmove[forbiddenmovecount].m.y=j;
// 					marked[i][j]=1;
					++forbiddenmovecount;
				}
				else
					_forbid[i][j]=1;
			}
			else
				_forbid[i][j]=0;
	// find hazards
	calculateStat(v);
	// evaluateBoard(v, BLACK);
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if ((temp=isDangerous(v, i, j, WHITE))
				&& _forbid[i][j]==1){
				switch (temp) {
					case 1:
						dangerfour[dangerfourcount].m.x=i;
						dangerfour[dangerfourcount].m.y=j;
// 						marked[i][j]=1;
						++dangerfourcount;
						break;
					case 2:
						dangerthree[dangerthreecount].m.x=i;
						dangerthree[dangerthreecount].m.y=j;
// 						marked[i][j]=1;
						++dangerthreecount;
						break;
					case 3:
						dangerfthree[dangerfthreecount].m.x=i;
						dangerfthree[dangerfthreecount].m.y=j;
// 						marked[i][j]=1;
						++dangerfthreecount;
						break;
					default:
						break;
				}
			}
	// if there is no hazard
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (_forbid[i][j]==1){
/*			if (marked[i][j]==0 
				&& _forbid[i][j]==1){*/
				k=getEvaluateForMove(v, BLACK, i, j);
//  				printf("evaluate %d,%d %d\n", i, j, k);
				switch (k) {
					case FIVE_SCORE:
						winningfive[winningfivecount].m.x=i;
						winningfive[winningfivecount].m.y=j;
						winningfive[winningfivecount].val=k;
// 						marked[i][j]=1;
						++winningfivecount;
						break;
					case FOUR_SCORE:
						winningfour[winningfourcount].m.x=i;
						winningfour[winningfourcount].m.y=j;
						winningfour[winningfourcount].val=k;
// 						marked[i][j]=1;
						++winningfourcount;
						break;
					case FTHREE_SCORE:
						winningfthree[winningfthreecount].m.x=i;
						winningfthree[winningfthreecount].m.y=j;
						winningfthree[winningfthreecount].val=k;
// 						marked[i][j]=1;
						++winningfthreecount;
						break;
					case DTHREE_SCORE:
						winningdthree[winningdthreecount].m.x=i;
						winningdthree[winningdthreecount].m.y=j;
						winningdthree[winningdthreecount].val=k;
// 						marked[i][j]=1;
						++winningdthreecount;
						break;
					default:
						ordinarymove[ordinarymovecount].m.x=i;
						ordinarymove[ordinarymovecount].m.y=j;
						ordinarymove[ordinarymovecount].val=k;
// 						marked[i][j]=1;
						++ordinarymovecount;
						break;
				}
			}
	if (DEBUG_EXPAND){
		printf("dump black\n");
		dumpAll();
	}
	if (winningfivecount){
		retval->mllen=1;
		retval->movelist[0]=winningfive[0];
	}
	else if (dangerfourcount){
		retval->mllen=dangerfourcount;
		for (i=0; i<dangerfourcount; ++i){
			retval->movelist[i]=dangerfour[i];
			retval->movelist[i].val=getEvaluateForMove(v,
				BLACK,
				dangerfour[i].m.x, dangerfour[i].m.y);
		}
		qsort(retval->movelist, retval->mllen,
				sizeof(MoveListType), _compMovesDec);
	}
	else if (winningfourcount){
		retval->mllen=winningfourcount;
		for (i=0; i<winningfourcount; ++i){
			retval->movelist[i]=winningfour[i];
		}
	}
/*	else if (winningfthreecount) {
		printf("d\n");
		retval->mllen=winningfthreecount;
		for (i=0; i<winningfthreecount; ++i){
			retval->movelist[i]=winningfthree[i];
		}
	}*/
	else if (dangerthreecount==0
		&& dangerfthreecount==0 && winningdthreecount){
		retval->mllen=winningdthreecount;
		for (i=0; i<winningdthreecount; ++i){
			retval->movelist[i]=winningdthree[i];
		}
	}
	else if (dangerthreecount+winningdthreecount+winningfthreecount
		+dangerfthreecount+ordinarymovecount>0) {
		int ofs=0;
		retval->mllen=dangerthreecount+winningdthreecount
			+dangerfthreecount+ordinarymovecount+winningfthreecount;
			
		for (i=0; i<dangerthreecount; ++i){
			retval->movelist[ofs+i].m=dangerthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, BLACK,
				dangerthree[i].m.x, dangerthree[i].m.y);
		}
		qsort(retval->movelist, dangerthreecount,
			sizeof(MoveListType), _compMovesDec);
		ofs+=dangerthreecount;
		for (i=0; i<dangerfthreecount; ++i){
			retval->movelist[ofs+i].m=dangerfthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, BLACK,
				dangerfthree[i].m.x, dangerfthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]),
			dangerfthreecount, sizeof(MoveListType), _compMovesDec);
		ofs+=dangerfthreecount;
		for (i=0; i<winningdthreecount; ++i)
			retval->movelist[ofs+i]=winningdthree[i];
		qsort(&(retval->movelist[ofs]),
			winningdthreecount, sizeof(MoveListType), _compMovesDec);
		ofs+=winningdthreecount;
		for (i=0; i<winningfthreecount; ++i)
			retval->movelist[ofs+i]=winningfthree[i];
		qsort(&(retval->movelist[ofs]),
			winningfthreecount, sizeof(MoveListType), _compMovesDec);
		ofs+=winningfthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			retval->movelist[ofs+i]=ordinarymove[i];
		qsort(&(retval->movelist[ofs]),
			ordinarymovecount, sizeof(MoveListType), _compMovesDec);
	}
	else {
		// TODO if no move available
		printf("????\n");
		dumpAll();
		printf("!!!!\n");
	}
}

void expandWhite(Configuration v, ChildIterator retval){
	int i, j, temp;
	int k;
// 	memset(marked, 0, sizeof(marked));
	dangerthreecount=dangerfourcount=dangerfthreecount
		=winningfivecount=winningfourcount=winningfthreecount
		=winningdthreecount=ordinarymovecount=forbiddenmovecount=0;
	// find hazards
	calculateStat(v);
	// evaluateBoard(v, BLACK);
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j){
			if (getColor(v, i, j)==NONE && 
				havePebbleAround(v, i, j))
				_forbid[i][j]=1;
			else {
				_forbid[i][j]=0;
			}
		}
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if ((temp=isDangerous(v, i, j, BLACK))
				&& _forbid[i][j]==1){
				if (forbid(v, i, j)) {
					continue;
				}
				switch (temp) {
					case 1:
						dangerfour[dangerfourcount].m.x=i;
						dangerfour[dangerfourcount].m.y=j;
// 						marked[i][j]=1;
						++dangerfourcount;
						break;
					case 2:
						dangerthree[dangerthreecount].m.x=i;
						dangerthree[dangerthreecount].m.y=j;
// 						marked[i][j]=1;
						++dangerthreecount;
						break;
					case 3:
						dangerfthree[dangerfthreecount].m.x=i;
						dangerfthree[dangerfthreecount].m.y=j;
// 						marked[i][j]=1;
						++dangerfthreecount;
						break;
					default:
						break;
				}
			}
	// if there is no hazard
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (_forbid[i][j]==1){
/*			if (marked[i][j]==0 
				&& _forbid[i][j]==1){*/
				k=getEvaluateForMove(v, WHITE, i, j);
				switch (k) {
					case -FIVE_SCORE:
						winningfive[winningfivecount].m.x=i;
						winningfive[winningfivecount].m.y=j;
						winningfive[winningfivecount].val=k;
						++winningfivecount;
// 						marked[i][j]=1;
						break;
					case -FOUR_SCORE:
						winningfour[winningfourcount].m.x=i;
						winningfour[winningfourcount].m.y=j;
						winningfour[winningfourcount].val=k;
						++winningfourcount;
// 						marked[i][j]=1;
						break;
					case -FTHREE_SCORE:
						winningfthree[winningfthreecount].m.x=i;
						winningfthree[winningfthreecount].m.y=j;
						winningfthree[winningfthreecount].val=k;
// 						marked[i][j]=1;
						++winningfthreecount;
						break;
					case -DTHREE_SCORE:
						winningdthree[winningdthreecount].m.x=i;
						winningdthree[winningdthreecount].m.y=j;
						winningdthree[winningdthreecount].val=k;
						++winningdthreecount;
// 						marked[i][j]=1;
						break;
					default:
						ordinarymove[ordinarymovecount].m.x=i;
						ordinarymove[ordinarymovecount].m.y=j;
						ordinarymove[ordinarymovecount].val=k;
						++ordinarymovecount;
// 						marked[i][j]=1;
						break;
				}
			}
	if (DEBUG_EXPAND){
		printf("dump white\n");
		dumpAll();
	}
	if (winningfivecount){
		retval->mllen=1;
		retval->movelist[0]=winningfive[0];
	}
	else if (dangerfourcount){
		retval->mllen=dangerfourcount;
		for (i=0; i<dangerfourcount; ++i){
			retval->movelist[i]=dangerfour[i];
			retval->movelist[i].val=getEvaluateForMove(v,
				WHITE,
				dangerfour[i].m.x, dangerfour[i].m.y);
		}
		qsort(retval->movelist, retval->mllen,
				sizeof(MoveListType), _compMovesInc);
	}
	else if (winningfourcount){
		retval->mllen=winningfourcount;
		for (i=0; i<winningfourcount; ++i){
			retval->movelist[i]=winningfour[i];
		}
	}
	else if (dangerthreecount==0 
		&& dangerfthreecount==0 
		&& winningdthreecount){
		retval->mllen=winningdthreecount;
		for (i=0; i<winningdthreecount; ++i){
			retval->movelist[i]=winningdthree[i];
		}
	}
	else if (dangerthreecount+winningdthreecount+dangerfthreecount
		+ordinarymovecount+winningfthreecount>0) {
		retval->mllen=dangerthreecount+winningdthreecount
			+dangerfthreecount+ordinarymovecount+winningfthreecount;
		int ofs=0;
		for (i=0; i<dangerthreecount; ++i){
			retval->movelist[i+ofs].m=dangerthree[i].m;
			retval->movelist[i+ofs].val=getEvaluateForMove(v, WHITE, 
				dangerthree[i].m.x, dangerthree[i].m.y);
		}
		qsort(retval->movelist, dangerthreecount,
			sizeof(MoveListType), _compMovesInc);
		ofs+=dangerthreecount;
		for (i=0; i<dangerfthreecount; ++i){
			retval->movelist[ofs+i].m=dangerfthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, WHITE,
				dangerfthree[i].m.x, dangerfthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]),
			dangerfthreecount, sizeof(MoveListType), _compMovesInc);
		ofs+=dangerfthreecount;
		for (i=0; i<winningdthreecount; ++i)
			retval->movelist[ofs+i]=winningdthree[i];
		qsort(&(retval->movelist[ofs]),
			winningdthreecount, sizeof(MoveListType), _compMovesInc);
		ofs+=winningdthreecount;
		for (i=0; i<winningfthreecount; ++i)
			retval->movelist[ofs+i]=winningfthree[i];
		qsort(&(retval->movelist[ofs]),
			winningfthreecount, sizeof(MoveListType), _compMovesInc);
		ofs+=winningfthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			retval->movelist[ofs+i]=ordinarymove[i];
		qsort(&(retval->movelist[ofs]),
			ordinarymovecount, sizeof(MoveListType), _compMovesInc);
	}
	else {
		// TODO if no move available
		printf("????\n");
		dumpAll();
		printf("????\n");
	}
}

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=&_childitrcontainer[_childitrpointer];
	++_childitrpointer;
	int i;
	retval->v=v;
	retval->mllen=0;
	retval->currentidx=0;
	// TODO what if no expansion is possible
	PEBBLE_COLOR player=getMover(v);
	if (player==BLACK) {
		expandBlack(v, retval);
	}
	else {
		expandWhite(v, retval);
	}
	if (retval->mllen>12)
		retval->mllen=12;
	if (DEBUG_EXPAND){
		printf(">>>>>>>>>>>>\n");
		printf("altogether %d\n", retval->mllen);
		for (i=0; i<retval->mllen; ++i)
			printf("%d, %d\n", retval->movelist[i].m.x, retval->movelist[i].m.y);
		printf("<<<<<<<<<<<<\n");
//		getchar();
	}
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

int getChildrenCount(ChildIterator itr){
	return itr->mllen;
}