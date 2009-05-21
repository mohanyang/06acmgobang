#include "mtdfengine.h"
#include "abengine.h"

enum {
	MAX_SEARCH_DEPTH = 8;
};

ReturnValue id_mtdf(Configuration v){
	ReturnValue f;
	f.value=0;
	int depth=1;
	// TODO read from configuration and 
	// use heuristics
	for (depth=1; depth<MAX_SEARCH_DEPTH; ++i) {
		f=mtdf(v, f.value, depth);
		// TODO stop time
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
	int alpha, beta;
	do {
		if (g.value==lb)
			beta=g.value+1;
		else
			beta=g.value;
		g.value=alphaBeta(v, beta-1, beta, depth);
		if (g.value<beta)
			ub=g;
		else
			lb=g;
	} while (ub>=lb);
	return f;
}
