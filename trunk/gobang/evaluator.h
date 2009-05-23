#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include "basetypes.h"

void initializeEvaluate();

int evaluate(Configuration v, Move *m);

int evaluateBoard(Configuration v, PEBBLE_COLOR c);

#endif
