#ifndef _FORBIDDEN_H_
#define _FORBIDDEN_H_

#include "basetypes.h"
#include "basestat.h"

/**
 * especially used to decide whether a move
 * is forbidden
 */

int doubleThree(Configuration v, Move m);

int doubleFour(Configuration v, Move m);

int overline(Configuration v, Move m);

#endif
