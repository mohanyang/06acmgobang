#include <stdio.h>
#include "engine.h"
#include "basetypes.h"
#include "mtdfengine.h"
#include "timer.h"
#include "hashsrv.h"
#include "transposition.h"
#include "evaluator.h"
#include "advstat.h"
#include "opening.h"
#include "moveheuristic.h"

Configuration glbl;
int _opening_state;

void initializeEngine(){
	initializeBaseType();
	initializeTimer();
	initializeHashService();
	hashInitialize();
	initializeEvaluate();
	initializeMoveHeuristic();
	initializeAdvStat();
	initialize_opening();
	initializeRound();
	_opening_state=BEGINNING_STATE;
	glbl=allocConfiguration();
	initializeConfiguration(glbl, BLACK);
}

ReturnValue search(PEBBLE_COLOR p){
	ReturnValue ret;
	if (_opening_state){
		Move best, current;
		int opt, temp;
		switch (p) {
			case BLACK:
				opt=-INFINITY;
				break;
			case WHITE:
				opt=+INFINITY;
				break;
			default:
				break;
		}
		for (current.x=0; current.x<15; ++current.x)
			for (current.y=0; current.y<15; ++current.y){
				if (move_opening(_opening_state, 
					current.x, current.y)) {
					putPebble(glbl, current.x, current.y, p);
					temp=evaluateBoard(glbl, p);
					printf("%d %d %d\n", current.x, current.y,
						   temp);
					removePebble(glbl, current.x, current.y);
					switch (p) {
						case BLACK:
							if (temp>opt){
								opt=temp;
								best=current;
							}
							break;
						case WHITE:
							if (temp<opt){
								opt=temp;
								best=current;
							}
						default:
							break;
					}
				}
			}
		switch (p) {
			case BLACK:
				if (opt>-INFINITY) {
					printf("a.o.\n");
					ret.move=best;
					ret.value=opt;
					return ret;
				}
				break;
			case WHITE:
				if (opt<+INFINITY) {
					printf("a.o.\n");
					ret.move=best;
					ret.value=opt;
					return ret;
				}
				break;
			default:
				break;
		}
	}
	initializeTimer();
	ret=id_mtdf(glbl);
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
	increaseRound();
	_opening_state=move_opening(_opening_state, m.x, m.y);
	printf("state=%d\n", _opening_state);
}

void playchess(int c){
	Move m;
	m.x=(c >> 8) & 0xff;
	m.y=c & 0xff;
	playMove(m);
}
