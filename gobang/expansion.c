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
#include "globalconst.h"
#include "four_three.h"
#include "engine.h"

enum {
	DEBUG_EXPAND = 0,
	MAX_CHILD = 50
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
	if (col==WHITE)
		ret=-ret;
	if (flag){
		putPebble(v, x, y, col);
		ret=evaluateBoard(v, col);
		removePebble(v, x, y);
		flag=four_three(v, x, y, col);
		if ((flag & FOUR_FOUR) && DFOUR_SCORE>abs(ret)){
			if (col==BLACK)
				ret=DFOUR_SCORE;
			else
				ret=-DFOUR_SCORE;
		}
		if ((flag & FOUR_THREE) && FTHREE_SCORE>abs(ret)){
/*			printf("###############\n");
			printBoardNonBlock(v);
			printf("##############\n");*/
			if (col==BLACK)
				ret=FTHREE_SCORE;
			else
				ret=-FTHREE_SCORE;
		}
		if ((flag & THREE_THREE) && DTHREE_SCORE>abs(ret)){
			if (col==BLACK)
				ret=DTHREE_SCORE;
			else
				ret=-DTHREE_SCORE;
		}
	}
	if ((col==BLACK && ret==FTHREE_SCORE)
			|| (col==WHITE && ret==-FTHREE_SCORE)){
/*		printf("@@@@@@@@@@@@@@@\n");
		printBoardNonBlock(v);
		printf("@@@@@@@@@@@@@@@\n");*/
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
// 	int has4=0;
// 	memset(marked, 0, sizeof(marked));
	dangerthreecount=dangerfourcount=dangerfthreecount
		=winningfivecount=winningfourcount=winningfthreecount
		=winningdthreecount=ordinarymovecount=forbiddenmovecount=0;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE && 
				havePebbleAround(v, i, j)){
				if (allowForbidden()==0 && forbid(v, i, j)) {
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
			if (_forbid[i][j]==1 && (temp=isDangerous(v, i, j, WHITE))){
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
					case DFOUR_SCORE:
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
// 						printf("fthree %d %d %d\n", i, j, k);
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
	else if (winningfthreecount){
		// winning 43 and danger 3, danger 43
		int ofs=0;
		for (i=0; i<winningfthreecount; ++i)
			retval->movelist[ofs+i]=winningfthree[i];
		ofs+=winningfthreecount;
		for (i=0; i<dangerfthreecount; ++i){
			retval->movelist[ofs+i].m=dangerfthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, 
				BLACK, dangerfthree[i].m.x, dangerfthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]), dangerfthreecount,
			sizeof(MoveListType), _compMovesDec);
		ofs+=dangerfthreecount;
		for (i=0; i<dangerthreecount; ++i){
			retval->movelist[ofs+i].m=dangerthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, 
					BLACK, dangerthree[i].m.x, dangerthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]), dangerthreecount,
				sizeof(MoveListType), _compMovesDec);
		ofs+=dangerthreecount;
		retval->mllen=ofs;
	}
	else if (dangerfthreecount){
		// danger 43, dthree, afour
		int ofs=0;
		for (i=0; i<dangerfthreecount; ++i){
			retval->movelist[ofs+i].m=dangerfthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, 
					BLACK, dangerfthree[i].m.x, dangerfthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]), dangerfthreecount,
				sizeof(MoveListType), _compMovesDec);
		ofs+=dangerfthreecount;
		for (i=0; i<winningdthreecount; ++i)
			retval->movelist[ofs+i]=winningdthree[i];
		ofs+=winningdthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			if (ordinarymove[i].val==300){
				retval->movelist[ofs]=ordinarymove[i];
				++ofs;
			}
		qsort(&(retval->movelist[dangerfthreecount+winningdthreecount]),
				ofs-dangerfthreecount-winningdthreecount,
				sizeof(MoveListType), _compMovesDec);
		retval->mllen=ofs;
	}
	else if (dangerthreecount==0 && winningdthreecount){
		retval->mllen=winningdthreecount;
		for (i=0; i<winningdthreecount; ++i){
			retval->movelist[i]=winningdthree[i];
		}
	}
	else if (dangerthreecount){
		int ofs=0;
		for (i=0; i<dangerthreecount; ++i){
			retval->movelist[ofs+i].m=dangerthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, BLACK,
					dangerthree[i].m.x, dangerthree[i].m.y);
		}
		qsort(retval->movelist, dangerthreecount,
			  sizeof(MoveListType), _compMovesDec);
		ofs+=dangerthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			if (ordinarymove[i].val==300){
				retval->movelist[ofs]=ordinarymove[i];
				++ofs;
			}
		qsort(&(retval->movelist[dangerthreecount]), ofs-dangerthreecount,
				sizeof(MoveListType), _compMovesDec);
		retval->mllen=ofs;
	}
	else if (winningdthreecount+ordinarymovecount>0) {
		int ofs=0;
		retval->mllen=winningdthreecount+ordinarymovecount;
		for (i=0; i<winningdthreecount; ++i)
			retval->movelist[ofs+i]=winningdthree[i];
		qsort(&(retval->movelist[ofs]),
			winningdthreecount, sizeof(MoveListType), _compMovesDec);
		ofs+=winningdthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			retval->movelist[ofs+i]=ordinarymove[i];
		qsort(&(retval->movelist[ofs]),
			ordinarymovecount, sizeof(MoveListType), _compMovesDec);
	}
	else {
		retval->mllen=forbiddenmovecount;
		for (i=0; i<forbiddenmovecount; ++i)
			retval->movelist[i]=forbiddenmove[i];
	}
}

void expandWhite(Configuration v, ChildIterator retval){
	int i, j, temp;
	int k;
	int has4=0;
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
			if (_forbid[i][j]==1 && (temp=isDangerous(v, i, j, BLACK))){
				if (allowForbidden()==0 && forbid(v, i, j)) {
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
/*			else {
				printf("%d %d\n", i, j);
				printBoard(v);
			}*/
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
					case -DFOUR_SCORE:
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
		printf("has four %d\n", has4);
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
	else if (winningfthreecount){
		// winning 43 and danger 3, danger 43
		int ofs=0;
		for (i=0; i<winningfthreecount; ++i)
			retval->movelist[ofs+i]=winningfthree[i];
		ofs+=winningfthreecount;
		for (i=0; i<dangerfthreecount; ++i){
			retval->movelist[ofs+i].m=dangerfthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, 
					WHITE, dangerfthree[i].m.x, dangerfthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]), dangerfthreecount,
				sizeof(MoveListType), _compMovesInc);
		ofs+=dangerfthreecount;
		for (i=0; i<dangerthreecount; ++i){
			retval->movelist[ofs+i].m=dangerthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, 
					WHITE, dangerthree[i].m.x, dangerthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]), dangerthreecount,
				sizeof(MoveListType), _compMovesInc);
		ofs+=dangerthreecount;
		retval->mllen=ofs;
	}
	else if (dangerfthreecount){
		// danger 43, dthree, afour
		int ofs=0;
		for (i=0; i<dangerfthreecount; ++i){
			retval->movelist[ofs+i].m=dangerfthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, 
					WHITE, dangerfthree[i].m.x, dangerfthree[i].m.y);
		}
		qsort(&(retval->movelist[ofs]), dangerfthreecount,
				sizeof(MoveListType), _compMovesInc);
		ofs+=dangerfthreecount;
		for (i=0; i<winningdthreecount; ++i)
			retval->movelist[ofs+i]=winningdthree[i];
		ofs+=winningdthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			if (ordinarymove[i].val==-300){
				retval->movelist[ofs]=ordinarymove[i];
				++ofs;
			}
		qsort(&(retval->movelist[dangerfthreecount+winningdthreecount]),
				ofs-dangerfthreecount-winningdthreecount,
				sizeof(MoveListType), _compMovesInc);
		retval->mllen=ofs;
	}
	else if (dangerthreecount==0 && winningdthreecount){
		retval->mllen=winningdthreecount;
		for (i=0; i<winningdthreecount; ++i){
			retval->movelist[i]=winningdthree[i];
		}
	}
	else if (dangerthreecount){
		int ofs=0;
		for (i=0; i<dangerthreecount; ++i){
			retval->movelist[ofs+i].m=dangerthree[i].m;
			retval->movelist[ofs+i].val=getEvaluateForMove(v, WHITE,
					dangerthree[i].m.x, dangerthree[i].m.y);
		}
		qsort(retval->movelist, dangerthreecount,
			  sizeof(MoveListType), _compMovesInc);
		ofs+=dangerthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			if (ordinarymove[i].val==-300){
				retval->movelist[ofs]=ordinarymove[i];
				++ofs;
			}
		qsort(&(retval->movelist[dangerthreecount]), ofs-dangerthreecount,
			sizeof(MoveListType), _compMovesInc);
		retval->mllen=ofs;
	}
	else if (winningdthreecount+ordinarymovecount>0) {
		retval->mllen=winningdthreecount+ordinarymovecount;
		int ofs=0;
		for (i=0; i<winningdthreecount; ++i)
			retval->movelist[ofs+i]=winningdthree[i];
		qsort(&(retval->movelist[ofs]),
			winningdthreecount, sizeof(MoveListType), _compMovesInc);
		ofs+=winningdthreecount;
		for (i=0; i<ordinarymovecount; ++i)
			retval->movelist[ofs+i]=ordinarymove[i];
		qsort(&(retval->movelist[ofs]),
			ordinarymovecount, sizeof(MoveListType), _compMovesInc);
	}
	else {
		retval->mllen=forbiddenmovecount;
		for (i=0; i<forbiddenmovecount; ++i)
			retval->movelist[i]=forbiddenmove[i];
	}
}

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=&_childitrcontainer[_childitrpointer];
	++_childitrpointer;
	int i;
	retval->v=v;
	retval->mllen=0;
	retval->currentidx=0;
	PEBBLE_COLOR player=getMover(v);
	if (player==BLACK) {
		expandBlack(v, retval);
	}
	else {
		expandWhite(v, retval);
	}
	if (whoAmI()==player && retval->mllen>MAX_CHILD)
		retval->mllen=MAX_CHILD;
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
