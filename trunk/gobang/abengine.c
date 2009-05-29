#include <stdio.h>
#include <stdlib.h>
#include "basetypes.h"
#include "abengine.h"
#include "transposition.h"
#include "evaluator.h"
#include "expansion.h"
#include "timer.h"
#include "moveheuristic.h"

enum {
	DEBUG_STACK = 0
};

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
		printf("\t");
}

ReturnValue alphaBeta(Configuration v, int alpha, int beta, int depth){
	int counter=0;
	if (DEBUG_STACK){
		printstack();
		printf("alphabeta a=%d b=%d d=%d\n", alpha, beta, depth);
// 		printBoardNonBlock(v);
	}
	++stackcount;
	HashRetVal s;
	s=retrieve(v);
	ReturnValue ret;
	ReturnValue temp;
	ret.alpha=alpha;
	ret.beta=beta;
	if (s!=NULL) {
		switch (s->type){
			case EXACT:
				if (s->lowerbound>=beta) {
					ret.value=s->lowerbound;
					ret.move=s->mv;
					--stackcount;
					if (DEBUG_STACK){
						printstack();
						printf("hit exact 1\n");
					}
					return ret;
				}
				if (s->upperbound<=alpha) {
					ret.value=s->upperbound;
					ret.move=s->mv;
					--stackcount;
					if (DEBUG_STACK){
						printstack();
						printf("hit exact 2\n");
					}
					return ret;
				}
				ret.alpha=max(alpha, s->lowerbound);
				ret.beta=min(beta, s->upperbound);
				ret.move=s->mv;
				break;
			case FAIL_LOW:
				if (s->upperbound<=alpha) {
					ret.value=s->upperbound;
					ret.move=s->mv;
					--stackcount;
					if (DEBUG_STACK){
						printstack();
						printf("hit fail low\n");
					}
					return ret;
				}
				break;
			case FAIL_HIGH:
				if (s->lowerbound>=beta) {
					ret.value=s->lowerbound;
					ret.move=s->mv;
					--stackcount;
					if (DEBUG_STACK){
						printstack();
						printf("hit fail high\n");
					}
					return ret;
				}
				break;
			default:
				break;
		}
		free(s);
	}
	if (depth==0) {
		ChildIterator itr=getExpansion(v);
		ret.value=getCurrentValue(itr);
		ret.move=getCurrent(itr);
		releaseChildIterator(itr);
		updateMoveHeuristic(v, ret.move.x, ret.move.y, ret.value);
		if (DEBUG_STACK){
			printstack();
			printf("eval %d\n", ret.value);
			printstack();
			printf("(%d, %d)\n", ret.move.x, ret.move.y);
// 			printBoardNonBlock(v);
		}
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
			applyMove(v, getCurrent(itr));
//			printBoardNonBlock(v);
			
			if (DEBUG_STACK){
				printstack();
				printf("black trying %d %d\n", 
					   getCurrent(itr).x, getCurrent(itr).y);
			}
			
			temp=alphaBeta(v, a, beta, depth-1);
			updateMoveHeuristic(v, temp.move.x,
								temp.move.y, temp.value);
			if (DEBUG_STACK){
				printstack();
				printf("black try %d %d, result=%d\n", 
					   getCurrent(itr).x, getCurrent(itr).y,
								  temp.value);
			}
			
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
			if (ret.value>=INFINITY+10)
				break;
			// TODO to be verified
/*			if (temp.value<=-INFINITY){
				++counter;
			}
			if (counter>2)
				break;*/
			getNext(&itr);
//			printf("e: %d %d\n", itr->current.x, itr->current.y);
		}
		if (DEBUG_STACK){
			if (ret.value>=beta){
				printstack();
				printf("pruned\n");
			}
		}
		releaseChildIterator(itr);
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
			
			if (DEBUG_STACK){
				printstack();
				printf("white trying %d %d\n", 
					   getCurrent(itr).x, getCurrent(itr).y);
			}
			
			temp=alphaBeta(v, alpha, b, depth-1);
			updateMoveHeuristic(v, temp.move.x,
								temp.move.y, temp.value);
			
			if (DEBUG_STACK){
				printstack();
				printf("white try %d %d, result=%d\n", 
					   getCurrent(itr).x, getCurrent(itr).y,
								  temp.value);
			}
			
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
			if (ret.value<=-INFINITY-10)
				break;
			// TODO to be verified
/*			if (temp.value>=INFINITY){
				++counter;
			}
			if (counter>2)
				break;*/
			getNext(&itr);
		}
		if (DEBUG_STACK){
			if (ret.value>alpha){
				printstack();
				printf("pruned\n");
			}
		}
		releaseChildIterator(itr);
	}

	if (ret.value<=alpha) {
		/* fail low */
		v->upperbound=ret.value;
		
		if (depth>0)
			store(v, ret.move, FAIL_LOW);
		
		/* Fail low result implies an upper bound */
	}
	else if (ret.value>alpha && ret.value<beta) {
		/* accurate window */
		v->upperbound=v->lowerbound=ret.value;
		if (depth>0)
			store(v, ret.move, EXACT);
	}
	else {
		/* fail high */
		v->lowerbound=ret.value;
		
		if (depth>0)
			store(v, ret.move, FAIL_HIGH);
		
	}
/*	printstack();
	printf("ab return %d (%d,%d))\n", ret.value, ret.move.x,
		  ret.move.y);*/
//	getchar();
	--stackcount;
	return ret;
}
