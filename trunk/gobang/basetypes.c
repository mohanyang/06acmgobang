#include <stdlib.h>
#include "basetypes.h"
#include "enginetypes.h"

int _reverse[1 << 16]={0};

PEBBLE_COLOR getOppositePlayer(PEBBLE_COLOR p){
	switch (p) {
		case WHITE:
			return BLACK;
		case BLACK:
			return WHITE;
		default:
			return NONE;
	}
}

NODETYPE getOppositeType(NODETYPE type){
	if (type==MAXNODE)
		return MINNODE;
	else
		return MAXNODE;
}

void initializeBaseType(){
	int i, j, k, l;
	for (i=1; i<(1<<16); ++i)
		if (_reverse[i]==0){
			j=0;
			l=i;
			for (k=0; k<16; ++k) {
				j=(j << 1) | (l & 1);
				l >>= 1;
			}
			_reverse[i]=j;
			_reverse[j]=i;
		}
}

void initializeConfiguration(Configuration v, PEBBLE_COLOR p){
	// TODO
	v->depth=0;
	v->type=MAXNODE;
	v->hboard=malloc(sizeof(int)*16);
	v->vboard=malloc(sizeof(int)*16);
	memset((void*)((v->hboard)), 0, sizeof(int)*16);
	memset((void*)((v->vboard)), 0, sizeof(int)*16);
	v->lowerbound=-INFINITY;
	v->upperbound=INFINITY;
}

PEBBLE_COLOR getMover(Configuration v){
	if (v->type==MAXNODE)
		return BLACK;
	else
		return WHITE;
}

void setMover(Configuration v, PEBBLE_COLOR c){
	if (c==BLACK)
		v->type=MAXNODE;
	else
		v->type=MINNODE;
}

PEBBLE_COLOR getColor(Configuration v, int x, int y){
	if ((v->vboard[y] & (1 << x))!=0)
		return BLACK;
	else if ((v->vboard[y] & (1 << (x+16)))!=0)
		return WHITE;
	else
		return NONE;
}

void putPebble(Configuration v, int x, int y, PEBBLE_COLOR col){
	if (col==BLACK){
		v->vboard[y] |= (1 << x);
		v->hboard[x] |= (1 << y);
	}
	else if (col==WHITE){
		v->vboard[y] |= (1 << (x+16));
		v->hboard[x] |= (1 << (y+16));
	}
}

void removePebble(Configuration v, int x, int y){
	v->vboard[y] &= ~((1 << x) | (1 << (x+16)));
	v->hboard[x] &= ~((1 << y) | (1 << (y+16)));
}

void increaseDepth(Configuration v){
	++(v->depth);
}

void decreaseDepth(Configuration v){
	--(v->depth);
}

int getDepth(Configuration v){
	return v->depth;
}

void setType(Configuration v, NODETYPE t){
	v->type=t;
}

NODETYPE getType(Configuration v){
	return v->type;
}

Configuration allocConfiguration(){
	Configuration ret=malloc(sizeof(struct BaseNode));
	memset(ret, 0, sizeof(struct BaseNode));
	ret->hboard=malloc(sizeof(int)*16);
	ret->vboard=malloc(sizeof(int)*16);
	memset(ret->hboard, 0, sizeof(int)*16);
	memset(ret->vboard, 0, sizeof(int)*16);
	return ret;
}

void setUB(Configuration v, int u){
	v->upperbound=u;
}

int getUB(Configuration v){
	return v->upperbound;
}

void setLB(Configuration v, int l){
	v->lowerbound=l;
}

int getLB(Configuration v){
	return v->lowerbound;
}

/**
 * divide x into two halfs, and reverse each half separately
 */
int reverseLine(int x){
	int v1=_reverse[x % (1 << 16)] >> 1;
	int v2=_reverse[x >> 16] >> 1;
	return (v2 << 16) | v1;
}

/**
 * flip the src configuration vertically and store it into the 
 * dest configuration
 */
void flipVertical(Configuration src, Configuration dest){
	int i;
	for (i=0; i<15; ++i) {
		dest->vboard[i]=reverseLine(src->vboard[i]);
		dest->hboard[i]=src->hboard[15-i];
	}
}

void selfFlipVertical(Configuration v) {
	int i;
	int ptr;
	for (i=0; i<15; ++i) {
		v->vboard[i]=reverseLine(v->vboard[i]);
		if (i<7){
			ptr=v->hboard[i];
			v->hboard[i]=v->hboard[15-1-i];
			v->hboard[15-1-i]=ptr;
		}
	}
}

/**
 * flip the src configuration horizontally and store it into the 
 * dest configuration
 */
void flipHorizontal(Configuration src, Configuration dest){
	int i;
	for (i=0; i<15; ++i) {
		dest->hboard[i]=reverseLine(src->hboard[i]);
		dest->vboard[i]=src->vboard[15-i];
	}
}

void selfFlipHorizontal(Configuration v){
	int i;
	int ptr;
	for (i=0; i<15; ++i) {
		v->hboard[i]=reverseLine(v->hboard[i]);
		if (i<7) {
			ptr=v->vboard[i];
			v->vboard[i]=v->vboard[15-1-i];
			v->vboard[15-1-i]=ptr;
		}
	}
}

void applyMove(Configuration v, Move m){
	putPebble(v, m.x, m.y, getMover(v));
	++(v->step);
	v->type=getOppositeType(v->type);
}

void undoMove(Configuration v, Move m){
	removePebble(v, m.x, m.y);
	--(v->step);
	v->type=getOppositeType(v->type);
}

void rotateBoard(Configuration src, Configuration dest){
	memcpy(dest->hboard, src->vboard, sizeof(int)*16);
	memcpy(dest->vboard, src->hboard, sizeof(int)*16);
}

void selfRotateBoard(Configuration v){
	int *ptr=v->hboard;
	v->hboard=v->vboard;
	v->vboard=ptr;
}