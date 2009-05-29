#include <stdlib.h>
#include <string.h>
#include "moveheuristic.h"
#include "hashsrv.h"

typedef struct {
	int hconf[16];
	int value;
	int next;
} MoveHNode;

enum {
	// TODO use different table size
	MAX_MOVE_TABLE_SIZE = 1987
};

// TODO finalize
// TODO what if container is full, deal with aging problem

/**
 * an array that stores the pointer to 
 * hashnodes, according to the depth,
 * the index is the key
 */
int *movepointer[15][15];
MoveHNode *movecontainer[15][15];
int movecontainersize[15][15];

/**
 * returns 0 if configuration v and hasnode p
 * represents the same configuration
 */
int compareMoveNode(Configuration v, int x, int y, int p){
	int i;
	for (i=0; i<15; ++i)
		if (v->hboard[i]!=movecontainer[x][y][p].hconf[i])
			return -1;
	return 0;
}

void initializeMoveHeuristic(){
	int i, j;
	memset(movecontainersize, 0, sizeof(movecontainersize));
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j) {
			movepointer[i][j]=malloc(MAX_MOVE_TABLE_SIZE
					*sizeof(int));
			memset((void*)movepointer[i][j], 0,
					sizeof(int)*MAX_MOVE_TABLE_SIZE);
			movecontainer[i][j]=malloc(MAX_MOVE_TABLE_SIZE
					*sizeof(MoveHNode));
			memset((void*)movecontainer[i][j], 0, 
					sizeof(MoveHNode)*MAX_MOVE_TABLE_SIZE);
		}
}

int getMoveEvaluate(Configuration v, int x, int y, int *status){
	// TODO optimize v so that do not 
	// need to calculate hash every time
/*	printf("===retrieving (%d, %d)===\n", x, y);
	printBoardNonBlock(v);*/
	*status=1;
	return 0;
	
	int key=calcHash(v, MAX_MOVE_TABLE_SIZE);
	key=movepointer[x][y][key];
	while (key!=0) {
		if (compareMoveNode(v, x, y, key)==0){
// 			printf("retrieved %d\n", movecontainer[x][y][key].value);
			*status=0;
			return movecontainer[x][y][key].value;
		}
		key=movecontainer[x][y][key].next;
	}
// 	printf("retrieve miss\n");
	*status=1;
	return 0;
}

void updateMoveHeuristic(Configuration v, int x, int y, int val){
	return;
	int key=calcHash(v, MAX_MOVE_TABLE_SIZE);
	int idx;
/*	printf("===saving (%d, %d)===%d===\n", x, y, val);
	printBoardNonBlock(v);
	printf("======\n");*/
	if (movepointer[x][y][key]!=0) {
		idx=movepointer[x][y][key];
		while (idx && compareMoveNode(v, x, y, idx)!=0){
			idx=movecontainer[x][y][idx].next;
		}
		if (idx==0){
			if (movecontainersize[x][y]>=MAX_MOVE_TABLE_SIZE)
				return;
			MoveHNode *ptr=&(movecontainer[x][y]
							[movecontainersize[x][y]]);
			memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
			ptr->value=val;
			ptr->next=movepointer[x][y][key];
			++movecontainersize[x][y];
		}
		else {
			movecontainer[x][y][idx].value=val;
		}
	}
	else {
		if (movecontainersize[x][y]>=MAX_MOVE_TABLE_SIZE)
			return;
		MoveHNode *ptr=&(movecontainer[x][y]
				[movecontainersize[x][y]]);
		memcpy(ptr->hconf, v->hboard, sizeof(int)*16);
		ptr->value=val;
		ptr->next=0;
		movepointer[x][y][key]=movecontainersize[x][y];
		++movecontainersize[x][y];
	}
}
