#ifndef _MOVEHEURISTIC_H_
#define _MOVEHEURISTIC_H_

#include "basetypes.h"

/**
 * heuristic for move ordering
 */

void initializeMoveHeuristic();

int getMoveEvaluate(Configuration v, int x, int y, int *status);

void updateMoveHeuristic(Configuration v, int x, int y, int val);

#endif
