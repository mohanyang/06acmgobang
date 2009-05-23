#include "engine.h"
#include "basetypes.h"
#include "mtdfengine.h"
#include "timer.h"
#include "transposition.h"

Configuration glbl;

void initializeEngine(){
	initializeBaseType();
	initializeTimer();
	hashInitialize();
	glbl=allocConfiguration();
}

ReturnValue search(PEBBLE_COLOR p){
//	initializeConfiguration(glbl, p);
	return id_mtdf(glbl);
}

void playMove(Move m){
	applyMove(glbl, m);
	printBoard(glbl);
}
