#include "engine.h"
#include "basetypes.h"
#include "enginetypes.h"
#include <stdio.h>

int main(){
	Move m;
	ReturnValue r;
	initializeEngine();
	while (1) {
		printf("black: ");
		scanf("%d %d", &(m.x), &(m.y));
		playMove(m);
		r=search(WHITE);
		printf("white: %d %d\n", r.move.x, r.move.y);
		playMove(r.move);
	}
}