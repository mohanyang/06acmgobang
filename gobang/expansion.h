#ifndef _EXPANSION_H_
#define _EXPANSION_H_

#include "basetypes.h"

struct ChildPointer;

typedef struct ChildPointer *ChildIterator;

ChildIterator getExpansion(Configuration v);

void getNext(ChildIterator *itr);

Move getCurrent(ChildIterator itr);

int getCurrentValue(ChildIterator itr);

#endif
