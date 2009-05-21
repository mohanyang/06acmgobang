#include <stdlib.h>
#include "transposition.h"

struct HNode{
	int depth;
	int value;
	int hconf[16], vconf[16];
	Move move;
	// next in the hash list
	int next;
};

enum {
	MAX_TRANSPOSITION_DEPTH = 8;
};

struct HNode* container[MAX_TRANSPOSITION_DEPTH];

void hashInitialize(){
}

