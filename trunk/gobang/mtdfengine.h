#ifndef _MTDFENGINE_H_
#define _MTDFENGINE_H_

#include "basetypes.h"
#include "enginetypes.h"

/**
 * begin iterative deepening mtd(f) search 
 * from the specified configuration, and 
 * return the search value
 */
ReturnValue id_mtdf(Configuration v, ReturnValue *ret);

#endif
