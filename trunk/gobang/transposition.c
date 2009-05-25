#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "transposition.h"

struct HNode{
	int hconf[16], vconf[16];
	int lb, ub;
	Move move;
	int depth, step;
	// next in the hash list
	int next;
};

typedef struct HNode HashNode;

enum {
	// TODO use different table size
	MAX_TABLE_SIZE = 100000,
	MAX_HASH_SIZE = 100000
};

// TODO finalize

static int hitcount=0, misscount=0;

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

int _zohrist[2][16][16];

/**
 * returns 0 if configuration v and hasnode p
 * represents the same configuration
 */
int compare(Configuration v, int p){
	int i;
	for (i=0; i<15; ++i)
		if (v->hboard[i]!=container[p].hconf[i])
			return -1;
	return 0;
}

int getHash(Configuration v){
	int i, res=0;
	for (i=0; i<15; ++i)
		res=(res * 4099)+v->hboard[i];
	return res % MAX_HASH_SIZE;
}

void hashInitialize(){
	pointer=malloc(MAX_HASH_SIZE*sizeof(int));
	memset((void*)pointer, 0, sizeof(int)*MAX_HASH_SIZE);
	container=malloc(MAX_TABLE_SIZE*sizeof(struct HNode));
	memset((void*)container, 0, sizeof(struct HNode)*MAX_TABLE_SIZE);
	tempconf=allocConfiguration();
	tempmove=malloc(sizeof(Move));
	int i,j,k;
	srand(time(NULL));
	for (i=0; i<2; ++i)
		for (j=0; j<16; ++j)
			for (k=0; k<16; ++k)
				_zohrist[i][j][k]=(rand() << 16) | rand();
}

void updateHash(Configuration v, int x, int y, PEBBLE_COLOR c){
	switch (c){
		case BLACK:
			v->hash ^= _zohrist[0][x][y];
			break;
		case WHITE:
			v->hash ^= _zohrist[1][x][y];
			break;
		default:
			break;
	}
}

HashRetVal retrieve(Configuration v){
	// TODO optimize v so that do not 
	// need to calculate hash every time
	int key=getHash(v);
	/*
	if (hitcount % 1000==0){
		printf("%d %d\n", hitcount, misscount);
		getchar();
	}
	*/
	
	if (pointer[key]!=0){
		// TODO iterate the list
		struct HNode *ptr=&(container[pointer[key]]);
		if (ptr->step<v->step || ptr->depth<v->depth) {
		//	printf("miss\n");
			++misscount;
			return NULL;
		}
		++hitcount;
		HashRetVal ret=malloc(sizeof(struct HashRet));
		ret->lowerbound=ptr->lb;
		ret->upperbound=ptr->ub;
		ret->mv=ptr->move;
		/*
		printf("===retrieved %d===\n", key);
		printVertical(container[pointer[key]].vconf);
		printf("======\n");
		*/
		return ret;
	}
	else {
//		printf("miss\n");
		++misscount;
		return NULL;
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
	/*
	printf("===storing %d / %d ~ %d===\n", key, v->lowerbound, 
		  v->upperbound);
	printBoardNonBlock(v);
	printf("======\n");
	*/
	if (pointer[key]!=0) {
		// TODO currently, we just check
		// if we can overwrite (i.e., the 
		// step of the found is not as deep as the 
		// current)
		/*
		if (compare(v, pointer[key])!=0){
			printf("conflict detected!\n");
			int i;
			printBoardNonBlock(v);
			for (i=0; i<15; ++i)
				printf("%d: %u\n", i, v->vboard[i]);
			printf("%d\n", v->hash);
			printVertical(container[pointer[key]].vconf);
			for (i=0; i<15; ++i)
				printf("%d: %u\n", i, container[pointer[key]].vconf[i]);
			printf("%d\n", key);
			getchar();
			assert(compare(v, pointer[key])==0);
		}
		*/
		int p=pointer[key];
		if (container[p].step<v->step) {
			struct HNode *ptr=&(container[pointer[key]]);
			memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
			memcpy(ptr->vconf, v->vboard, sizeof(int)*16);
			ptr->lb=v->lowerbound;
			ptr->ub=v->upperbound;
			ptr->move=*m;
			ptr->depth=v->depth;
			ptr->step=v->step;
		}
	}
	else {
		if (containersize>=MAX_TABLE_SIZE)
			return;
		++containersize;
		if (containersize % 1000==0)
			printf("container reached %d\n", containersize);
		struct HNode *ptr=&(container[containersize]);
		memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
		memcpy(ptr->vconf, v->vboard, sizeof(int)*16);
		ptr->lb=v->lowerbound;
		ptr->ub=v->upperbound;
		ptr->move=*m;
		ptr->depth=v->depth;
		ptr->step=v->step;
		ptr->next=0;
		pointer[key]=containersize;
//		printf("pos=%d, key=%d\n", containersize,
//			  key);
	}
}

void store(Configuration v, Move m){
	int key=getHash(v);
	int i, j, k;
	memcpy((void*)(tempconf), (void*)v, sizeof(struct BaseNode));
	memcpy((void*)tempmove, (void*)&m, sizeof(Move));
	for (i=0; i<2; ++i) {
		selfFlipVertical(tempconf);
		flipMoveVertical(tempmove);
		for (j=0; j<2; ++j) {
			selfFlipHorizontal(tempconf);
			flipMoveHorizontal(tempmove);
			for (k=0; k<2; ++k) {
				selfRotateBoard(tempconf);
				rotateMove(tempmove);
//				printf("%d %d %d\n", i, j, k);
				saveConfiguration(tempconf, tempmove);
			}
		}
	}
}
