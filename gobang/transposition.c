#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "hashsrv.h"
#include "transposition.h"

struct HNode{
	int hconf[16];
// 	int vconf[16];
	int lb, ub;
	Move move;
	HashNodeType type;
	int depth, step;
	// next in the hash list
	int next;
};

typedef struct HNode HashNode;

enum {
	// TODO use different table size
	MAX_TABLE_SIZE = 200000,
	MAX_HASH_SIZE = 200000,
	DEBUG_TRANSPOS = 0
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

void hashInitialize(){
	pointer=malloc(MAX_HASH_SIZE*sizeof(int));
	memset((void*)pointer, 0, sizeof(int)*MAX_HASH_SIZE);
	container=malloc(MAX_TABLE_SIZE*sizeof(struct HNode));
	memset((void*)container, 0, sizeof(struct HNode)*MAX_TABLE_SIZE);
	tempconf=allocConfiguration();
	tempmove=malloc(sizeof(Move));
}

HashRetVal retrieve(Configuration v){
	// TODO optimize v so that do not 
	// need to calculate hash every time
	// TODO how to deal with fail lo and fail hi nodes
  //        return NULL;
	int key=calcHash(v, MAX_TABLE_SIZE);
	if (DEBUG_TRANSPOS){
		printf("===retrieving===\n");
		printf("key=%d\n", key);
		printf("hit stat %d %d\n", hitcount, misscount);
	}
	key=pointer[key];
	while (key!=0) {
		if (compare(v, key)==0){
			// hit!
			struct HNode *ptr=&(container[key]);
			// TODO repository
			if (ptr->depth>=v->depth) {
				if (DEBUG_TRANSPOS){
					printf("hit depth %d >= %d\n", ptr->depth, v->depth);
					printf(">>>>>>>>>>>\n");
					printBoardNonBlock(v);
					printf("<<<<<<<<<<<\n");
				}
				HashRetVal ret=malloc(sizeof(struct HashRet));
				ret->lowerbound=ptr->lb;
				ret->upperbound=ptr->ub;
				ret->mv=ptr->move;
				ret->type=ptr->type;
				++hitcount;
// 				printf("===retrieved===\n");
				return ret;
			}
		}
		key=container[key].next;
	}
	++misscount;
	return NULL;
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

void saveConfiguration(Configuration v, Move *m, HashNodeType type){
	int key=calcHash(v, MAX_TABLE_SIZE);
	int idx;
	if (DEBUG_TRANSPOS){
		printf("===storing %d / %d ~ %d %d===\n", key, v->lowerbound, 
			   v->upperbound, type);
		printBoardNonBlock(v);
		printf("======\n");
	}
	
	if (pointer[key]!=0) {
		idx=pointer[key];
		while (idx && compare(v, idx)!=0){
			idx=container[idx].next;
		}
		if (idx==0){
			// add one at beginning
			if (containersize>=MAX_TABLE_SIZE-1)
				return;
			++containersize;
			if (containersize % 1000==0)
				printf("container reached %d\n", containersize);
			struct HNode *ptr=&(container[containersize]);
			memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
// 			memcpy(ptr->vconf, v->vboard, sizeof(int)*16);
			ptr->lb=v->lowerbound;
			ptr->ub=v->upperbound;
			ptr->move=*m;
			ptr->depth=v->depth;
			ptr->step=v->step;
			ptr->next=0;
			ptr->type=type;
			pointer[key]=containersize;
		}
		else {
			struct HNode *ptr=&(container[key]);
			ptr->lb=v->lowerbound;
			ptr->ub=v->upperbound;
			ptr->move=*m;
			ptr->depth=v->depth;
			ptr->step=v->step;
			ptr->type=type;
		}
	}
	else {
		if (containersize>=MAX_TABLE_SIZE-1)
			return;
		++containersize;
		if (containersize % 1000==0)
			printf("container reached %d\n", containersize);
		struct HNode *ptr=&(container[containersize]);
		memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
// 		memcpy(ptr->vconf, v->vboard, sizeof(int)*16);
		ptr->lb=v->lowerbound;
		ptr->ub=v->upperbound;
		ptr->move=*m;
		ptr->depth=v->depth;
		ptr->step=v->step;
		ptr->next=0;
		ptr->type=type;
		pointer[key]=containersize;
	}
}

void store(Configuration v, Move m, HashNodeType type){
// 	return;
	int i, j, k;
	memcpy((void*)(tempconf), (void*)v, sizeof(struct BaseNode));
	memcpy((void*)tempmove, (void*)&m, sizeof(Move));
/*	for (i=0; i<2; ++i) {
		selfFlipVertical(tempconf);
		flipMoveVertical(tempmove);
		for (j=0; j<2; ++j) {
			selfFlipHorizontal(tempconf);
			flipMoveHorizontal(tempmove);
			for (k=0; k<2; ++k) {
				selfRotateBoard(tempconf);
				rotateMove(tempmove);*/
//				printf("%d %d %d\n", i, j, k);
				saveConfiguration(tempconf, tempmove, type);
/*				printf("saving\n");
				printf(">>>>>>>>>>>\n");
				printBoardNonBlock(v);
				printf("<<<<<<<<<<<\n");*/
/*			}
		}
	}*/
}
