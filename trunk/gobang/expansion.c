#include "expansion.h"
#include <stdlib.h>

ChildIterator getExpansion(Configuration v) {
	ChildIterator retval=malloc(sizeof (struct ChildPointer));
	int i,j;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE) {
				retval->x=i;
				retval->y=j;
				return retval;
			}
	return NULL;
}

void getNext(Configuration v, ChildIterator itr) {
	int i,j;
	for (i=itr->y; i<15; ++i)
		if (getColor(v, itr->x, i)==NONE){
			itr->y=i;
			return;
		}
	for (i=itr->x+1; i<15; ++i)
		for (j=0; j<15; ++j)
			if (getColor(v, i, j)==NONE) {
				itr->x=i;
				itr->y=j;
				return;
			}
	itr=NULL;
}