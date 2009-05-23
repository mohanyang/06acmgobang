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
	ReturnValue temp;
	ret.alpha=alpha;
	ret.beta=beta;
	if (s!=NULL) {
		printf("retrieved.\n");
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
		ret.value=evaluateBoard(v, getMover(v));
		printf("eval %d\n", ret.value);
	}
	else if (getType(v)==MAXNODE) {
		printf("*\n");
		ChildIterator itr=getExpansion(v);
		printf("e: %d %d %d\n", getCurrent(itr).x, getCurrent(itr).y, depth);
		int a=alpha;
		ret.value=-INFINITY;
		ret.move=getCurrent(itr);
		while (itr!=NULL && ret.value<beta) {
			if (tickTimer()==0)
				break;
			applyMove(v, getCurrent(itr));
//			printBoard(v);
			temp=alphaBeta(v, a, beta, depth-1);
			if (temp.value>ret.value) {
				ret.value=temp.value;
				ret.move=getCurrent(itr);
			}
			undoMove(v, getCurrent(itr));
//			printBoard(v);
			printf("a=%d ret.value=%d\n", a, ret.value);
			if (a>ret.value){
				a=ret.value;
			}
			getNext(&itr);
//			printf("e: %d %d\n", itr->current.x, itr->current.y);
		}
	}
	else {
		printf("-\n");
		ChildIterator itr=getExpansion(v);
		printf("n: %d %d %d\n", getCurrent(itr).x, getCurrent(itr).y, depth);
		int b=beta;
		ret.value=INFINITY;
		ret.move=getCurrent(itr);
		while (itr!=NULL && ret.value>alpha) {
			if (tickTimer()==0)
				break;
			applyMove(v, getCurrent(itr));
			temp=alphaBeta(v, alpha, b, depth-1);
			if (temp.value<ret.value){
				ret.value=temp.value;
				ret.move=getCurrent(itr);
			}
			undoMove(v, getCurrent(itr));
			if (b<ret.value){
				b=ret.value;
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
