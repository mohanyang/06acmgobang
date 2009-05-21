#ifndef _EXPANSION_H_
#define _EXPANSION_H_

#include "basetypes.h"

struct ChildPointer{
	Move current;
};

typedef struct ChildPointer *ChildIterator;

ChildIterator getExpansion(Configuration v);

void getNext(Configuration v, ChildIterator itr);

#endif
