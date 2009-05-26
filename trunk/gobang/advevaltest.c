#include "advevaltest.h"

void move(Configuration v, int x, int y){
	Move m;
	m.x=x;
	m.y=y;
	applyMove(v, m);
}

void main(){
	Configuration v=allocConfiguration();
	initializeConfiguration(v, BLACK);
	move(v, 7, 7);
	move(v, 6, 6);
	move(v, 5, 7);
	move(v, 6, 7);
	move(v, 6, 8);
	move(v, 7, 6);
	move(v, 5, 8);
	move(v, 5, 6);
	move(v, 8, 6);
	move(v, 5, 9);
	move(v, 4, 6);
	move(v, 7, 9);
	move(v, 6, 9);
	move(v, 4, 5);
	move(v, 3, 5);
	move(v, 2, 4);
	move(v, 10, 4);
	printBoardNonBlock(v);
	/*
	calculate(v);
	dumpcalc();
	printf("%d %d\n", getScore(BLACK), getScore(WHITE));
	*/
	EvalRetVal r1=evaluateBoard(v, BLACK);
	EvalRetVal r2=evaluateBoard(v, WHITE);
	printf("black %d,%d %d\n", r1.mv.x, r1.mv.y, r1.value);
	printf("white %d,%d %d\n", r2.mv.x, r2.mv.y, r2.value);
}