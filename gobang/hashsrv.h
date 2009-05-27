#ifndef _HASHSRV_H_
#define _HASHSRV_H_

#include "basetypes.h"

void initializeHashService();

int calcHash(Configuration v, int size);

void updateHash(Configuration v, int x, int y, PEBBLE_COLOR c);

#endif
