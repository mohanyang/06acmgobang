#include <stdlib.h>
#include "historyheuristic.h"

typedef struct {
	int key;
	int round;
} HistoryNode;

HistoryNode _historycontainer[300];
int _historypointer[300];
int _staticroundcounter;

void alignToRound(int idx){
	_historycontainer[idx].key=
			((_historycontainer[idx].key >> 
			(16+_staticroundcounter
				-_historycontainer[idx].round)) << 16) 
			| (_historycontainer[idx].key && ((1 << 16)-1));
	_historycontainer[idx].round=_staticroundcounter;
}

int compareHistoryNode(const void *x, const void *y){
	alignToRound(*((int*)x));
	alignToRound(*((int*)y));
	return _historycontainer[*((int*)x)].key-
			_historycontainer[*((int*)y)].key;
}

void initializeHistoryHeuristic(){
	int i,j;
	_staticroundcounter=0;
	for (i=0; i<15; ++i)
		for (j=0; j<15; ++j) {
			_historycontainer[i*15+j].key=(i << 8) | j;
			_historycontainer[i*15+j].round=0;
			_historypointer[i*15+j]=i*15+j;
		}
}

void refresh(){
	int i;
	for (i=0; i<225; ++i)
		alignToRound(i);
	qsort(_historypointer, 225, sizeof(int), compareHistoryNode);
}

int getHistoryScore(int idx){
	return _historycontainer[_historypointer[idx]].key >> 16;
}

int getHistoryPosition(int idx){
	return _historycontainer[_historypointer[idx]].key 
			& ((1 << 16) - 1);
}

void updateHistory(Move m, int depth){
	int t=_historycontainer[m.x*15+m.y].key;
	_historycontainer[m.x*15+m.y].key
			=(((t >> 16) | (1 << depth)) << 16) 
			| (t & ((1 << 16)-1));
}

void increaseHistoryRound(){
	// TODO optimize
	// can use heap
	refresh();
	++_staticroundcounter;
}
