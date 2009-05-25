#ifndef _EVALUATOR_H_
#define _EVALUATOR_H_

#include "basetypes.h"

typedef struct {
	int value;
	Move mv;
} EvalRetVal;

void initializeEvaluate();

int evaluate(Configuration v, Move *m);

EvalRetVal evaluateBoard(Configuration v, PEBBLE_COLOR c);

#endif
