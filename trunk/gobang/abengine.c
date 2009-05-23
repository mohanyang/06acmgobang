#include <stdio.h>
#include <stdlib.h>
#include "basetypes.h"
#include "abengine.h"
#include "transposition.h"
#include "evaluator.h"
#include "expansion.h"
#include "timer.h"

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
		ret.alpha=max(alpha, s->lowerbound);
		ret.beta=min(beta, s->upperbound);
	}
	if (depth==0)
		ret.value=evaluate(v, &(ret.move));
	else if (getType(v)==MAXNODE) {
		ChildIterator itr=getExpansion(v);
		int a=alpha;
		ret.value=-INFINITY;
		while (itr!=NULL && ret.value<beta) {
			if (tickTimer()==0)
				break;
			applyMove(v, itr->current);
			ret.value=max(ret.value, alphaBeta(v, a, beta, depth-1).value);
			undoMove(v, itr->current);
			if (a>ret.value){
				a=ret.value;
				ret.move=itr->current;
			}
			getNext(itr);
		}
	}
	else {
		ChildIterator itr=getExpansion(v);
		int b=beta;
		ret.value=INFINITY;
		while (itr!=NULL && ret.value>alpha) {
			if (tickTimer()==0)
				break;
			applyMove(v, itr->current);
			ret.value=min(ret.value, alphaBeta(v, alpha, b, depth-1).value);
			undoMove(v, itr->current);
			if (b<ret.value){
				b=ret.value;
				ret.move=itr->current;
			}
			getNext(itr);
		}
	}

	if (ret.value<=alpha) {
		/* fail low */
		v->upperbound=ret.value;
		/* Fail low result implies an upper bound */
	}
	else if (ret.value>alpha && ret.value<beta) {
		/* accurate window */
		v->upperbound=v->lowerbound=ret.value;
	}
	else {
		/* fail high */
		v->lowerbound=ret.value;
	}
	store(v, ret.move);
	return ret;
}
