#include "basestat.h"

/**
 * a temporary position type to store dir offset
 */
Move _tempdir;

void applyDir(DIR d, Move *m);
int inBoard(Move *pos, Move *dir, int step);

int getConsecutive(Configuration v, PEBBLE_COLOR c, Move *m, DIR d){
	applyDir(d, &_tempdir);
	int count=0;
	int i;
	while (1) {
		if (inBoard(m, &_tempdir, count+1)==0)
			break;
		if (getColor(v, m->x+(count+1)*(_tempdir.x),
			m->y+(count+1)*(_tempdir.y))==c) {
			++count;
		}
		else
			break;
	}
	return count;
}

int getClosestOpponent(Configuration v, PEBBLE_COLOR c, Move *m, DIR d){
	PEBBLE_COLOR target=getOppositePlayer(c);
	applyDir(d, &_tempdir);
	int distance=1;
	while (1) {
		if (inBoard(m, &_tempdir, distance)==0)
			break;
		if (getColor(v, m->x+distance*_tempdir.x,
			m->y+distance*_tempdir.y)==target)
			return distance;
		++distance;
	}
	return distance+1;
}

void applyDir(DIR d, Move *m){
	switch (d) {
		case N:
			m->x=0;
			m->y=1;
			return;
		case S:
			m->x=0;
			m->y=-1;
			return;
		case E:
			m->x=1;
			m->y=0;
			return;
		case W:
			m->x=-1;
			m->y=0;
			return;
		case NE:
			m->x=1;
			m->y=1;
			return;
		case NW:
			m->x=1;
			m->y=-1;
			return;
		case SW:
			m->x=-1;
			m->y=-1;
			return;
		case SE:
			m->x=-1;
			m->y=1;
			return;
		default:
			return;
	}
}

int inBoard(Move *pos, Move *dir, int step){
	int tx=pos->x+step*dir->x;
	int ty=pos->y+step*dir->y;
	if (tx>=0 && tx<15 && ty>=0 && ty<15)
		return 1;
	else
		return 0;
}
