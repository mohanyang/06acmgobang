#include <stdlib.h>
#include "transposition.h"

struct HNode{
	int depth;
	int value;
	int hconf[16], vconf[16];
	Move move;
	// next in the hash list
	int next;
};

enum {
	MAX_TRANSPOSITION_DEPTH = 8;
	MAX_TABLE_SIZE = 1000;
};

int* pointer[MAX_TRANSPOSITION_DEPTH];
struct HNode *container;

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
	int key=getHash(v);
	if (v->depth>MAX_TRANSPOSITION_DEPTH)
		return NULL;
	else {
		
	}
}