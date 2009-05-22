#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "basetypes.h"
#include "enginetypes.h"

/**
 * initialize the whole search engine, including hashset, 
 * history, etc.
 */
void initialize();

/**
 * search below the current configuration,
 * the root corresponds to player with
 * specified color
 */
ReturnValue search(Configuration v, PEBBLE_COLOR player);

#endif
