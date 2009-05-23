#ifndef _EXPANSION_H_
#define _EXPANSION_H_

#include "basetypes.h"

struct ChildPointer{
	Configuration v;
	Move current;
};

typedef struct ChildPointer *ChildIterator;

ChildIterator getExpansion(Configuration v);

void getNext(ChildIterator itr);

#endif
