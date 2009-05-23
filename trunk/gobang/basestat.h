#ifndef _BASESTAT_H_
#define _BASESTAT_H_

#include "basetypes.h"

enum _DIRECTION {
	N, E, W, S, NW, NE, SW, SE
};
typedef enum _DIRECTION DIR;

int getConsecutive(Configuration v, PEBBLE_COLOR c, Move *m, DIR d);

int getClosestOpponent(Configuration v, PEBBLE_COLOR c, Move *m, DIR d);

#endif
