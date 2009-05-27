#include "engine.h"
#include "basetypes.h"
#include "enginetypes.h"
#include <stdio.h>

int main(){
	Move m;
	ReturnValue r;
	initializeEngine();
	
	m.x=6;	m.y=6;
	playMove(m);
	m.x=4; m.y=4;
	playMove(m);
	m.x=7; m.y=5;
	playMove(m);
	m.x=2; m.y=2;
	playMove(m);
	m.x=3; m.y=3;
	playMove(m);
	m.x=5; m.y=4;
	playMove(m);
	m.x=6; m.y=4;
	playMove(m);
	m.x=0; m.y=2;
	playMove(m);
	m.x=1; m.y=2;
	playMove(m);
	m.x=4; m.y=5;
	playMove(m);
	m.x=1; m.y=3;
	playMove(m);
	m.x=4; m.y=8;
	playMove(m);
	m.x=3; m.y=7;
	playMove(m);
	m.x=4; m.y=6;
	playMove(m);
	m.x=4; m.y=7;
	playMove(m);
	m.x=3; m.y=2;
	playMove(m);
	m.x=4; m.y=3;
	playMove(m);
	m.x=0; m.y=3;
	playMove(m);
	m.x=0; m.y=4;
	playMove(m);
	r=search(WHITE);
	playMove(r.move);
	
/*	FILE *fp=fopen("log.txt", "w");
	while (1){
		printf("black: ");
		scanf("%d %d", &(m.x), &(m.y));
		if (m.x<0)
			break;
		fprintf(fp, "black: %d %d\n", m.x, m.y);
		playMove(m);
		r=search(WHITE);
		printf("white: %d %d\n", r.move.x, r.move.y);
		playMove(r.move);
		fprintf(fp, "white: %d %d\n", r.move.x, r.move.y);
	}
	fclose(fp);*/
	return 0;
}