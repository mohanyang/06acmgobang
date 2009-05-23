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
	printf("alphabeta a=%d b=%d d=%d\n", alpha, beta, depth);
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
	if (depth==0) {
		ret.value=evaluate(v, &(ret.move));
//		printf("eval %d\n", ret.value);
	}
	else if (getType(v)==MAXNODE) {
		printf("*\n");
		ChildIterator itr=getExpansion(v);
		printf("e: %d %d %d\n", itr->current.x, itr->current.y, depth);
		int a=alpha;
		ret.value=-INFINITY;
		ret.move=itr->current;
		while (itr!=NULL && ret.value<beta) {
			if (tickTimer()==0)
				break;
			applyMove(v, itr->current);
//			printBoard(v);
			ret.value=max(ret.value, alphaBeta(v, a, beta, depth-1).value);
			undoMove(v, itr->current);
//			printBoard(v);
			printf("a=%d ret.value=%d\n", a, ret.value);
			if (a>ret.value){
				a=ret.value;
				ret.move=itr->current;
				printf("current ret.move=%d %d\n", ret.move.x, ret.move.y);
			}
			getNext(&itr);
//			printf("e: %d %d\n", itr->current.x, itr->current.y);
		}
	}
	else {
		printf("-\n");
		ChildIterator itr=getExpansion(v);
		printf("n: %d %d\n", itr->current.x, itr->current.y);
		int b=beta;
		ret.value=INFINITY;
		ret.move=itr->current;
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
			getNext(&itr);
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
