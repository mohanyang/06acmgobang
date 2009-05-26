#include <stdio.h>
#include "mtdfengine.h"
#include "abengine.h"
#include "timer.h"

enum {
	MAX_SEARCH_DEPTH = 4
};

ReturnValue mtdf(Configuration v, int firstguess, int depth);

int _globalcount=0;

ReturnValue id_mtdf(Configuration v){
	ReturnValue f;
	f.value=0;
	int depth=1;
	resetTimer();
	++_globalcount;
	if (_globalcount<3)
		f=mtdf(v, f.value, 2);
	else {
	// TODO read from configuration and 
	// use heuristics
	// TODO unbounded search depth
		for (depth=0; depth<=MAX_SEARCH_DEPTH; depth+=2) {
			printf("id depth = %d\n", depth);
			f=mtdf(v, f.value, depth);
			if (tickTimer()==0)
				break;
		}
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
	v->depth=depth;
	do {
		if (g.value==lb)
			beta=g.value+1;
		else
			beta=g.value;
		printf(">>>>> search restart %d %d\n", beta, depth);
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
