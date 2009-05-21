#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "basetypes.h"

/**
 * initialize the whole search engine, including hashset, 
 * history, etc.
 */
void initialize();

/**
 * search below the current configuration
 */
void search(Configuration v);

#endif
