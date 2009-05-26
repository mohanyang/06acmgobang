#ifndef _HISTORYHEURISTIC_H_
#define _HISTORYHEURISTIC_H_

#include "basetypes.h"

void initializeHistoryHeuristic();

int getHistoryScore(int idx);

int getHistoryPosition(int idx);

void updateHistory(Move m, int depth);

void increaseHistoryRound();

#endif
