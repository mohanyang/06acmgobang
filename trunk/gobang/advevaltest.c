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
	move(v, 1,1);
	move(v, 7,7);
	move(v, 2,2);
	move(v, 6,6);
	move(v, 3,3);
	move(v, 4,4);
	printBoardNonBlock(v);
	calculate(v);
	dumpcalc();
	printf("%d %d\n", getScore(BLACK), getScore(WHITE));
}