#include <stdio.h>
#include "engine.h"
#include "basetypes.h"
#include "mtdfengine.h"
#include "timer.h"
#include "hashsrv.h"
#include "transposition.h"
#include "evaluator.h"

Configuration glbl;
ReturnValue glblretval;

void initializeEngine(){
	initializeBaseType();
	initializeTimer();
	initializeHashService();
	hashInitialize();
	initializeEvaluate();
	initializeMoveHeuristic();
	glbl=allocConfiguration();
	initializeConfiguration(glbl, BLACK);
}

ReturnValue search(PEBBLE_COLOR p){
//	initializeConfiguration(glbl, p);
//	initializeTimer();
	ReturnValue ret=id_mtdf(glbl, &glblretval);
	printf("====search finished, returned %d %d %d====\n",
		  ret.move.x, ret.move.y, ret.value);
	showTimer();
	return ret;
}

int generate(){
	ReturnValue r=search(getMover(glbl));
	return (r.move.x << 8) | r.move.y;
}

void playMove(Move m){
	applyMove(glbl, m);
	printBoardNonBlock(glbl);
}

void playchess(int c){
	Move m;
	m.x=(c >> 8) & 0xff;
	m.y=c & 0xff;
	playMove(m);
}

int getCurrentSolution(){
	return (glblretval.move.x << 8) | glblretval.move.y;
}