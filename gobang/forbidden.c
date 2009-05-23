#include "forbidden.c"

enum DIRECTION {
	N, E, W, S, NW, NE, SW, SE
};

int doubleThree(Configuration v, Move m){
	// count how many live three are there
	// in all four directions
}

int doubleFour(Configuration v, Move m){
}

int overline(Configuration v, Move m){
}

/*
obtain how many consecutive same colors
where the closest opponent color is.
*/
int getConsecutive(Configuration v, Move *m, PEBBLE_COLOR c, DIRECTION d){
	// TODO use pointer array
	int temp;
	switch (d){
		case N:
			
			break;
		case E:
			break;
		case W:
			break;
		case S:
			break;
		case NW:
			break;
		case NE:
			break;
		case SW:
			break;
		case SE:
			break;
		default:
			return 0;
	}
}

int getClosestOpponent(Configuration v, Move *m, PEBBLE_COLOR c, DIRECTION d){
}
