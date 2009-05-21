#include <stdlib.h>
#include "basetypes.h"

int _reverse[1 << 16]={0};

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

PEBBLE_COLOR getMover(Configuration v){
	return v->mover;
}

void setMover(Configuration v, PEBBLE_COLOR c){
	v->mover=c;
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

void applyMove(Configuration v, PEBBLE_COLOR col, Move m){
	putPebble(v, m.x, m.y, col);
}

void undoMove(Configuration v, Move m){
	removePebble(v, m.x, m.y);
}
