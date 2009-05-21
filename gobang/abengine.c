#include <stdlib.h>
#include "basetypes.h"
#include "abengine.c"
#include "transposition.h"
#include "evaluator.h"
#include "expansion.h"

int max(int a, int b){
	return a>b?a:b;
}

int min(int a, int b){
	return a<b?a:b;
}

ReturnValue alphaBeta(Configuration v, int alpha, int beta, int depth){
	HashRetVal s=retrieve(v);
	ReturnValue ret;
	ret.alpha=alpha;
	ret.beta=beta;
	if (s!=NULL) {
		if (s->lowerbound>=beta) {
			ret.value=getLB(v);
			ret.move=s->mv;
			return ret;
		}
		if (s->upperbound<=alpha) {
			ret.value=getUB(v);
			ret.move=s->mv;
			return ret;
		}
		ret.alpha=max(alpha, getLB(s));
		ret.beta=min(beta, getUB(s));
	}
	if (depth==0)
		ret.value=evaluate(v, &(ret.move));
	else if (getType(v)==MAXNODE) {
		ChildIterator itr=getExpansion(v);
		int a=alpha;
		ret.value=-INFINITY;
		while (itr!=NULL && ret.value<beta) {
			applyMove(v, itr->current);
			ret.value=max(ret.value, alphaBeta(v, a, beta, depth-1));
			undoMove(v, itr->current);
			a=max(a, ret.value);
			getNext(v, itr);
		}
	}
	else {
		ChildIterator itr=getExpansion(v);
		int b=beta;
		ret.value=INFINITY;
		while (itr!=NULL && ret.value>alpha) {
			applyMove(v, itr->current);
			ret.value=min(ret.value, alphaBeta(v, alpha, b, depth-1));
			undoMove(v, itr->current);
			b=min(b, ret.value);
			getNext(v, itr);
		}
	}

	if (ret.value<=alpha) {
		/* Fail low result implies an upper bound */
		n.upperbound := g; store n.upperbound;
	}
	else if (ret.value>alpha && ret.value<beta) {
		/* Found an accurate minimax value - will not occur if called with zero window */
		n.lowerbound := g; n.upperbound := g; store n.lowerbound, n.upperbound;
	}
	else {
		n.lowerbound := g; store n.lowerbound;
	}
	return retval;
}