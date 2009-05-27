#ifndef _TRANSPOSITION_H_
#define _TRANSPOSITION_H_

#include <stdlib.h>
#include "basetypes.h"

typedef enum {
	EXACT, FAIL_LOW, FAIL_HIGH
} HashNodeType;

struct HashRet {
	int lowerbound;
	int upperbound;
	Move mv;
	HashNodeType type;
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
void store(Configuration v, Move m, HashNodeType type);

#endif
