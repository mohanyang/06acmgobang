#include <stdio.h>
#include "mtdfengine.h"
#include "abengine.h"
#include "timer.h"

enum {
	MAX_SEARCH_DEPTH = 8
};

ReturnValue mtdf(Configuration v, int firstguess, int depth);

int _globalcount=10;

ReturnValue id_mtdf(Configuration v){
	ReturnValue f;
	ReturnValue temp;
	int target;
	if (getMover(v)==BLACK)
		target=INFINITY;
	else
		target=-INFINITY;
	f.value=0;
	int depth=1;
	// TODO read from configuration and 
	// use heuristics
	// TODO unbounded search depth
	for (depth=0; 1; depth+=2) {
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
