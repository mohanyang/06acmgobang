#include "mtdfengine.h"
#include "abengine.h"
#include "timer.h"

enum {
	MAX_SEARCH_DEPTH = 2
};

ReturnValue mtdf(Configuration v, int firstguess, int depth);

ReturnValue id_mtdf(Configuration v){
	ReturnValue f;
	f.value=0;
	int depth=1;
	resetTimer();
	// TODO read from configuration and 
	// use heuristics
	for (depth=1; depth<MAX_SEARCH_DEPTH; ++depth) {
		f=mtdf(v, f.value, depth);
		if (tickTimer())
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
		if (g.value==lb)
			beta=g.value+1;
		else
			beta=g.value;
		g=alphaBeta(v, beta-1, beta, depth);
		if (g.value<beta)
			ub=g.value;
		else
			lb=g.value;
	} while (ub>=lb);
	return g;
}
