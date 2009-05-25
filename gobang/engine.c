#include "engine.h"
#include "basetypes.h"
#include "mtdfengine.h"
#include "timer.h"
#include "transposition.h"
#include "evaluator.h"

Configuration glbl;

void initializeEngine(){
	initializeBaseType();
	initializeTimer();
	hashInitialize();
	initializeEvaluate();
	glbl=allocConfiguration();
	initializeConfiguration(glbl, BLACK);
}

ReturnValue search(PEBBLE_COLOR p){
//	initializeConfiguration(glbl, p);
	return id_mtdf(glbl);
}

void playMove(Move m){
	applyMove(glbl, m);
	printBoardNonBlock(glbl);
}
