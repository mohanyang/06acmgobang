#include <stdlib.h>
#include "transposition.h"

struct HNode{
	int hconf[16], vconf[16];
	int lb, ub;
	Move move;
	int depth, step;
	// next in the hash list
	int next;
};

enum {
	// TODO use different table size
	MAX_TABLE_SIZE = 10000;
	MAX_HASH_SIZE = 10000;
};

// TODO finalize
// TODO consider zohrist

/**
 * an array that stores the pointer to 
 * hashnodes, according to the depth,
 * the index is the key
 */
int *pointer;
struct HNode *container;
int containersize=0;
/**
 * temporary configuration used to calculate
 * flip and rotate
 */
Configuration tempconf;
Move *tempmove;

int getHash(Configuration v){
	int i, key=0;
	for (i=0; i<15; ++i)
		key ^= v->hboard[i];
	key = ~key + (key << 15);
	key = key ^ (key >>> 12);
	key = key + (key << 2);
	key = key ^ (key >>> 4);
	key = key * 2057;
	key = key ^ (key >>> 16);
	return key % MAX_HASH_SIZE;
}

void hashInitialize(){
	int i;
	pointer=malloc(MAX_HASH_SIZE*sizeof(int));
	memset(pointer, 0, sizeof(int)*MAX_HASH_SIZE);
	container=malloc(MAX_TABLE_SIZE*sizeof(struct HNode));
	memset(container, 0, sizeof(struct HNode)*MAX_TABLE_SIZE);
	tempconf=allocConfiguration();
	tempmove=malloc(sizeof(Move));
}

HashRetVal retrieve(Configuration v){
	// TODO optimize v so that do not 
	// need to calculate hash every time
	int key=getHash(v);
	if (pointer[key]!=0){
		// TODO iterate the list
		struct HNode *ptr=&(container[pointer[key]]);
		HashRetVal ret;
		ret.lowerbound=ptr->lb;
		ret.upperbound=ptr->ub;
		ret.mv=ptr->move;
		return ret;
	}
	else
		return NULL;
}

void store(Configuration v, Move m){
	int key=getHash(v);
	int i, j, k;
	if (v->depth>MAX_TRANSPOSITION_DEPTH)
		return NULL;
	else {
		memcpy(*tempconf, *v, sizeof(*Configuration));
		memcpy(*tempmove, m, sizeof(Move));
		for (i=0; i<2; ++i) {
			flipVertical(tempconf);
			flipMoveVertical(tempmove);
			for (j=0; j<2; ++j) {
				flipHorizontal(tempconf);
				flipMoveHorizontal(tempmove);
				for (k=0; k<4; ++k) {
					rotateBoard(tempconf);
					rotateMove(tempmove);
					saveConfiguration(tempconf, tempmove);
				}
			}
		}
	}
}

void flipMoveHorizontal(Move *m){
	m->x=(15-1-m->x);
}

void flipMoveVertical(Move *m){
	m->y=(15-1-m->y);
}

void rotateMove(Move *m){
	int tmp=m->y;
	m->y=(15-1-m->x);
	m->x=tmp;
}

void saveConfiguration(Configuration v, Move *m){
	int key=getHash(v);
	if (pointer[key]!=0) {
		// TODO currently, we just check
		// if we can overwrite (i.e., the 
		// step of the found is not as deep as the 
		// current)
		int p=pointer[key];
		if (container[p]->step<v->step) {
			struct HNode *ptr=&(container[containersize]);
			memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
			memcpy(ptr->vconf, v->vboard, sizeof(int)*16);
			ptr->lb=v->lowerbound;
			ptr->ub=v->upperbound;
			ptr->move=v->mv;
			ptr->depth=v->depth;
			ptr->step=v->step;
		}
	}
	else {
		++containersize;
		struct HNode *ptr=&(container[containersize]);
		memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
		memcpy(ptr->vconf, v->vboard, sizeof(int)*16);
		ptr->lb=v->lowerbound;
		ptr->ub=v->upperbound;
		ptr->move=v->mv;
		ptr->depth=v->depth;
		ptr->step=v->step;
		ptr->next=0;
	}
}
