#ifndef _ABENGINE_H_
#define _ABENGINE_H_

#include "basetypes.h"

/**
 * the alpha-beta search engine
 */

enum CONSTANTS {
	INFINITY = 0x7fffff;
}

struct RV {
	int alpha;
	int beta;
	int value;
	Move move;
};

typedef struct RV ReturnValue;

int alphaBeta(Configuration v, int alpha, int beta, int depth);

#endif