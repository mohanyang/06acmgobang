#include <stdio.h>
#include "mtdfengine.h"
#include "abengine.h"
#include "timer.h"

int _global_round_counter;

ReturnValue mtdf(Configuration v, int firstguess, int depth);

ReturnValue id_mtdf(Configuration v){
	ReturnValue f;
	ReturnValue temp;
	int target;
	if (getMover(v)==BLACK)
		target=INFINITY+1;
	else
		target=-INFINITY-1;
	f.value=0;
	int depth=0;
	int MAXROUND=(225-_global_round_counter)/2;
	printf("round: %d\n", _global_round_counter);
	for (depth=0; depth<MAXROUND; depth+=2) {
		printf("id depth = %d\n", depth);
		f=mtdf(v, f.value, depth);
		if (tickTimer()==0)
			break;
		else
			temp=f;
		if (f.value==target)
			break;
	}
	return temp;
}

/**
 * begin mtd(f) search with firstguess and maximum depth
 * specified
 */
ReturnValue mtdf(Configuration v, int firstguess, int depth){
	ReturnValue g;
	ReturnValue temp;
	g.value=firstguess;
	int ub=INFINITY, lb=-INFINITY;
	int beta;
	v->depth=depth;
	do {
		if (g.value==lb)
			beta=g.value+1;
		else
			beta=g.value;
		printf(">>>>> search restart %d %d\n", beta, depth);
		g=alphaBeta(v, beta-1, beta, depth);
		if (tickTimer())
			temp=g;
		if (g.value<beta)
			ub=g.value;
		else
			lb=g.value;
		printf("ub=%d, lb=%d\n", ub, lb);
		printf("value=%d, move=(%d, %d)\n", g.value,
			  g.move.x, g.move.y);
	} while (ub>lb);
	return temp;
}

void increaseRound(){
	++_global_round_counter;
}

void initializeRound(){
	_global_round_counter=0;
}
