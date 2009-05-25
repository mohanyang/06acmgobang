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

static int stackcount=0;

void printstack(){
	int i;
	for (i=0; i<stackcount; ++i)
		printf(" ");
}

ReturnValue alphaBeta(Configuration v, int alpha, int beta, int depth){
//	printf("alphabeta a=%d b=%d d=%d\n", alpha, beta, depth);
	++stackcount;
	HashRetVal s=retrieve(v);
	ReturnValue ret;
	ReturnValue temp;
	ret.alpha=alpha;
	ret.beta=beta;
	if (s!=NULL) {
//		printf("retrieved.\n");
		if (s->lowerbound>=beta) {
			ret.value=getLB(v);
			ret.move=s->mv;
			--stackcount;
			return ret;
		}
		if (s->upperbound<=alpha) {
			ret.value=getUB(v);
			ret.move=s->mv;
			--stackcount;
			return ret;
		}
		ret.alpha=max(alpha, s->lowerbound);
		ret.beta=min(beta, s->upperbound);
		ret.move=s->mv;
	}
	if (depth==0) {
		EvalRetVal x=evaluateBoard(v, getMover(v));
		ret.value=x.value;
		ret.move=x.mv;
//		printf("eval %d\n", ret.value);
	}
	else if (getType(v)==MAXNODE) {
//		printf("*\n");
		ChildIterator itr=getExpansion(v);
//		printf("e: %d %d %d\n", getCurrent(itr).x, getCurrent(itr).y, depth);
		int a=alpha;
		ret.value=-INFINITY;
		ret.move=getCurrent(itr);
		while (itr!=NULL && ret.value<beta) {
//			printf("depth=%d, move=%d %d\n",
//				depth, getCurrent(itr).x, 
//				getCurrent(itr).y);
			if (tickTimer()==0)
				break;
			/*
			if (getCurrent(itr).x==5 && getCurrent(itr).y==5)
				printf("5 5\n");
			*/
			applyMove(v, getCurrent(itr));
//			printBoardNonBlock(v);
			/*
			printstack();
			printf("black trying %d %d\n", 
				   getCurrent(itr).x, getCurrent(itr).y);
			*/
			temp=alphaBeta(v, a, beta, depth-1);
			/*
			printstack();
			printf("black try %d %d, result=%d\n", 
				   getCurrent(itr).x, getCurrent(itr).y,
			 	temp.value);
			*/
			if (temp.value>ret.value) {
				ret.value=temp.value;
				ret.move=getCurrent(itr);
//				printf("current black move = %d %d\n", ret.move.x, ret.move.y);
			}
			undoMove(v, getCurrent(itr));
//			printBoard(v);
			if (a>ret.value){
				a=ret.value;
			}
//			printf("a=%d ret.value=%d\n", a, ret.value);
			getNext(&itr);
//			printf("e: %d %d\n", itr->current.x, itr->current.y);
		}
	}
	else {
//		printf("-\n");
		ChildIterator itr=getExpansion(v);
//		printf("n: %d %d %d\n", getCurrent(itr).x, getCurrent(itr).y, depth);
		int b=beta;
		ret.value=INFINITY;
		ret.move=getCurrent(itr);
		while (itr!=NULL && ret.value>alpha) {
			if (tickTimer()==0)
				break;
			applyMove(v, getCurrent(itr));
			/*
			printstack();
			printf("white trying %d %d\n", 
				   getCurrent(itr).x, getCurrent(itr).y);
			*/
			temp=alphaBeta(v, alpha, b, depth-1);
			/*
			printstack();
			printf("white try %d %d, result=%d\n", 
				   getCurrent(itr).x, getCurrent(itr).y,
					temp.value);
			*/
			if (temp.value<ret.value){
				ret.value=temp.value;
				ret.move=getCurrent(itr);
	//			printf("current white move = %d %d\n", ret.move.x, ret.move.y);
			}
			undoMove(v, getCurrent(itr));
			if (b<ret.value){
				b=ret.value;
			}
//			printf("a=%d ret.value=%d\n", b, ret.value);
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
		if (depth>0)
			store(v, ret.move);
	}
	else {
		/* fail high */
		v->lowerbound=ret.value;
	}
//	printf("ab return %d (%d,%d))\n", ret.value, ret.move.x,
//		  ret.move.y);
	--stackcount;
	return ret;
}
