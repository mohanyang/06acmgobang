#include "expansion.h"
#include <stdio.h>
#include <stdlib.h>

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

// TODO a repository for struct ChildPointer

int _compMovesInc(const void *x, const void *y){
	return ((MoveListType*)x)->val-((MoveListType*)y)->val;
}

int _compMovesDec(const void *x, const void *y){
	return ((MoveListType*)y)->val-((MoveListType*)x)->val;
}

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=malloc(sizeof (struct ChildPointer));
	int i,j;
	retval->v=v;
	retval->mllen=0;
	retval->currentidx=0;
	// TODO what if no expansion is possible
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE) {
				retval->movelist[retval->mllen].m.x=i;
				retval->movelist[retval->mllen].m.y=j;
				retval->movelist[retval->mllen].val=
						evaluate(v, &(retval->movelist[
						retval->mllen].m));
				++(retval->mllen);
			}
	if (getType(v)==MAXNODE)
		qsort(retval->movelist, retval->mllen,
			  sizeof(MoveListType), _compMovesDec);		
	else
		qsort(retval->movelist, retval->mllen,
			sizeof(MoveListType), _compMovesInc);
	printf("mllen=%d\n", retval->mllen);
	/*
	for (i=0; i<retval->mllen; ++i){
		printf("- %d %d\n", retval->movelist[i].m.x,
			  retval->movelist[i].m.y);
	}
	*/
	return retval;
}

void getNext(ChildIterator *itr) {
	if (*itr==NULL)
		return;
	++((*itr)->currentidx);
	if ((*itr)->currentidx>=(*itr)->mllen) {
		free(*itr);
		*itr=NULL;
	}
}

/**
 * should assert that itr is not NULL when calling
 */
Move getCurrent(ChildIterator itr) {
	return (itr->movelist[itr->currentidx]).m;
}
