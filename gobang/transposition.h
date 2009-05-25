#ifndef _TRANSPOSITION_H_
#define _TRANSPOSITION_H_

#include <stdlib.h>
#include "basetypes.h"

struct HashRet {
	int lowerbound;
	int upperbound;
	Move mv;
};

typedef struct HashRet *HashRetVal;

/**
 * initialize the hashset, allocating space, etc.
 */
void hashInitialize();

/**
 * given an incomplete configuration, search in the transposition table,
 * return the corresponding node if exists
 */
HashRetVal retrieve(Configuration v);

/**
 * stores the current configuration into the transposition table
 */
void store(Configuration v, Move m);

void updateHash(Configuration v, int x, int y, PEBBLE_COLOR c);

#endif
