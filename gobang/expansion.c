#include "expansion.h"
#include <stdio.h>
#include <stdlib.h>

// TODO a repository for struct ChildPointer

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=malloc(sizeof (struct ChildPointer));
	int i,j;
	retval->v=v;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE) {
				retval->current.x=i;
				retval->current.y=j;
				return retval;
			}
	return NULL;
}

void getNext(ChildIterator *itr) {
	if (itr==NULL)
		return;
	int i,j;
	for (i=(*itr)->current.y+1; i<15; ++i)
		if (getColor((*itr)->v, (*itr)->current.x, i)==NONE){
			(*itr)->current.y=i;
//			printf("%d %d\n", (*itr)->current.x, (*itr)->current.y);
			return;
		}
	for (i=(*itr)->current.x+1; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor((*itr)->v, i, j)==NONE) {
				(*itr)->current.x=i;
				(*itr)->current.y=j;
//				printf("%d %d\n", (*itr)->current.x, (*itr)->current.y);
				return;
			}
	free(*itr);
	*itr=NULL;
}
