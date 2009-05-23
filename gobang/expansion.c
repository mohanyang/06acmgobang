#include "expansion.h"
#include <stdlib.h>

// TODO a repository for struct ChildPointer

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=malloc(sizeof (struct ChildPointer));
	int i,j;
	retval->v=v;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE) {
				retval->x=i;
				retval->y=j;
				return retval;
			}
	return NULL;
}

void getNext(ChildIterator itr) {
	if (itr==NULL)
		return;
	int i,j;
	for (i=itr->y; i<15; ++i)
		if (getColor(itr->v, itr->x, i)==NONE){
			itr->y=i;
			return;
		}
	for (i=itr->x+1; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(itr->v, i, j)==NONE) {
				itr->x=i;
				itr->y=j;
				return;
			}
	free(itr);
	itr=NULL;
}
