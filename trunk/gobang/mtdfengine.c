#include <stdio.h>
#include "mtdfengine.h"
#include "abengine.h"
#include "timer.h"

enum {
	MAX_SEARCH_DEPTH = 4
};

ReturnValue mtdf(Configuration v, int firstguess, int depth);

ReturnValue id_mtdf(Configuration v){
	ReturnValue f;
	f.value=0;
	int depth=1;
	resetTimer();
	// TODO read from configuration and 
	// use heuristics
	for (depth=MAX_SEARCH_DEPTH; depth<=MAX_SEARCH_DEPTH; ++depth) {
		printf("id depth = %d\n", depth);
		f=mtdf(v, f.value, depth);
		if (tickTimer()==0)
			break;
	}
	return f;
}

/**
 * begin mtd(f) search with firstguess and maximum depth
 * specified
 */
ReturnValue mtdf(Configuration v, int firstguess, int depth){
	ReturnValue g;
	g.value=firstguess;
	int ub=INFINITY, lb=-INFINITY;
	int beta;
	do {
		printf(">>>>> search restart\n");
		if (g.value==lb)
			beta=g.value+1;
		else
			beta=g.value;
		g=alphaBeta(v, beta-1, beta, depth);
		if (g.value<beta)
			ub=g.value;
		else
			lb=g.value;
		printf("ub=%d, lb=%d\n", ub, lb);
		printf("value=%d\n", g.value);
	} while (ub>lb);
	return g;
}
