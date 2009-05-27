#include "engine.h"
#include "basetypes.h"
#include "enginetypes.h"
#include <stdio.h>

void main(){
	Move m;
	ReturnValue r;
	initializeEngine();
	
	m.x=7;	m.y=7;
	playMove(m);
	m.x=6; m.y=6;
	playMove(m);
	m.x=6; m.y=8;
	playMove(m);
	m.x=5; m.y=6;
	playMove(m);
	m.x=8; m.y=6;
	playMove(m);
	getchar();
	r=search(WHITE);
	playMove(r.move);
	
	while (1){
		printf("black: ");
		scanf("%d %d", &(m.x), &(m.y));
		if (m.x<0)
			break;
		playMove(m);
		r=search(WHITE);
		printf("white: %d %d\n", r.move.x, r.move.y);
		playMove(r.move);
	}
}