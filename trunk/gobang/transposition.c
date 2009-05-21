#include <stdlib.h>
#include "transposition.h"

struct HNode{
	int hconf[16], vconf[16];
	int lb, ub;
	Move move;
	// next in the hash list
	int next;
};

enum {
	MAX_TRANSPOSITION_DEPTH = 8;
	// TODO use different table size
	MAX_TABLE_SIZE = 1000;
};

int* pointer[MAX_TRANSPOSITION_DEPTH];
struct HNode *container;
int containersize=0;

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
	return key;
}

void hashInitialize(){
	int i;
	for (i=0; i<MAX_TRANSPOSITION_DEPTH; ++i){
		// TODO use different hash size
		pointer[i]=malloc(1000*sizeof(int));
		memset(pointer[i], 0, sizeof(int)*1000);
	}
	container=malloc(1000*sizeof(struct HNode));
	memset(container, 0, sizeof(struct HNode)*1000);
}

HashRetVal retrieve(Configuration v){
	// TODO optimize v so that do not 
	// need to calculate hash every time
	int key=getHash(v);
	if (v->depth>MAX_TRANSPOSITION_DEPTH)
		return NULL;
	else {
		if (pointer[v->depth][key % MAX_TABLE_SIZE]!=0){
			// TODO iterate the list
			struct HNode *ptr=&(container[pointer[v->depth][key % MAX_TABLE_SIZE]]);
			HashRetVal ret;
			ret.lowerbound=ptr->lb;
			ret.upperbound=ptr->ub;
			ret.mv=ptr->move;
			return ret;
		}
		else
			return NULL;
	}
}

void store(Configuration v, Move m){
	int key=getHash(v);
	if (v->depth>MAX_TRANSPOSITION_DEPTH)
		return NULL;
	else {
		if (pointer[v->depth][key % MAX_TABLE_SIZE]!=0){
			// TODO
			// current: ignore
		}
		else {
			// allocate space for a new node
			++containersize;
			struct HNode *target;
			target=&(container[containersize]);
			target->lb=v->lowerbound;
			target->ub=v->upperbound;
			target->move=v->mv;
			memcpy(target->hconf, v->hboard, 16*sizeof(int));
			memcpy(target->vconf, v->vboard, 16*sizeof(int));
			// TODO make linked list
		}
	}
}