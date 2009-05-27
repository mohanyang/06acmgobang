#include <stdlib.h>
#include <time.h>
#include "hashsrv.h"

int _zobrist[2][16][16];

void initializeHashService(){
	int i,j,k;
	srand(time(0));
	for (i=0; i<2; ++i)
		for (j=0; j<16; ++j)
			for (k=0; k<16; ++k)
				_zobrist[i][j][k]=(rand() << 16) | rand();
}

int calcHash(Configuration v, int size){
	int i;
	unsigned int res=0;
	for (i=0; i<15; ++i)
		res=(res * 4099)+v->hboard[i];
	return res % size;
}

void updateHash(Configuration v, int x, int y, PEBBLE_COLOR c){
}

