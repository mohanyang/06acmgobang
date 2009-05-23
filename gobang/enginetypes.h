#ifndef _ENGINETYPES_H_
#define _ENGINETYPES_H_

#include "basetypes.h"

enum CONSTANTS {
	INFINITY = 0x7fffff
};

struct RV {
	int alpha;
	int beta;
	int value;
	Move move;
};

typedef struct RV ReturnValue;

#endif
