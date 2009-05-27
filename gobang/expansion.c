#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "expansion.h"
#include "evaluator.h"
#include "enginetypes.h"

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

// TODO a repository for struct ChildPointer

int _compMovesInc(const void *x, const void *y){
	return ((MoveListType*)x)->val-((MoveListType*)y)->val;
}

int _compMovesDec(const void *x, const void *y){
	return ((MoveListType*)y)->val-((MoveListType*)x)->val;
}

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=&_childitrcontainer[_childitrpointer];
	memset(retval, 0, sizeof(struct ChildPointer));
	++_childitrpointer;
//	printf("child pointer %d\n", _childitrpointer);
	
	int i,j;
	retval->v=v;
	retval->mllen=0;
	retval->currentidx=0;
	// TODO what if no expansion is possible
	int target;
	int flag=0;
	if (getMover(v)==BLACK)
		target=INFINITY;
	else
		target=-INFINITY;
	for (i=0; i<15; ++i) {
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE) {
//				printf("%d,%d\n", i, j);
				retval->movelist[retval->mllen].m.x=i;
				retval->movelist[retval->mllen].m.y=j;
				retval->movelist[retval->mllen].val=
						evaluate(v, &(retval->movelist[
						retval->mllen].m));
				++(retval->mllen);
				/*
				if (retval->movelist[retval->mllen-1].val==target) {
					retval->mllen=1;
					retval->movelist[0].m.x=i;
					retval->movelist[0].m.y=j;
					retval->movelist[0].val=target;
					flag=1;
					break;
				}*/
			}
		if (flag)
			break;
	}
	if (getType(v)==MAXNODE)
		qsort(retval->movelist, retval->mllen,
			  sizeof(MoveListType), _compMovesDec);		
	else
		qsort(retval->movelist, retval->mllen,
			sizeof(MoveListType), _compMovesInc);
	if (retval->mllen>0){
		int i=0;
		while (retval->movelist[i].val==target)
			++i;
		if (i>0) {
			retval->mllen=i;
			/*
			printf("mllen=%d\n", retval->mllen);
			getchar();
			*/
		}
	}
	
//	printf("mllen=%d\n", retval->mllen);
	/*
	for (i=0; i<retval->mllen; ++i){
		printf("- %d %d %d\n", retval->movelist[i].m.x,
			  retval->movelist[i].m.y, retval->movelist[i].val);
	}
	getchar();
	*/
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