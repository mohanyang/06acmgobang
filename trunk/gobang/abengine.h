#ifndef _ABENGINE_H_
#define _ABENGINE_H_

#include "basetypes.h"
#include "enginetypes.h"

/**
 * the alpha-beta search engine
 */

ReturnValue alphaBeta(Configuration v, int alpha, int beta, int depth);

#endif