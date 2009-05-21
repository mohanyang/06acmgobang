#ifndef _BASETYPES_H_
#define _BASETYPES_H_

enum NODETYPE {
	MAXNODE, MINNODE
};

enum PEBBLE_COLOR {
	NONE=0, BLACK=1, WHITE=2
};

struct BaseNode;

typedef struct BaseNode *Configuration;

struct Position {
	int x, int y;
};

typedef struct Position Move;

void initializeBaseType();

void applyMove(Configuration v, PEBBLE_COLOR col, Move m);

void undoMove(Configuration v, Move m);

Configuration allocConfiguration();

PEBBLE_COLOR getColor(Configuration v, int x, int y);

/**
 * put a pebble at the specified position
 * the caller should ensure that the position is originally clear
 */
void putPebble(Configuration v, int x, int y, PEBBLE_COLOR col);

void removePebble(Configuration v, int x, int y);

NODETYPE getType(Configuration v);

void setType(Configuration v, NODETYPE t);

void increaseDepth(Configuration v);

void decreaseDepth(Configuration v);

int getDepth(Configuration v);

void setUB(Configuration v, int u);

int getUB(Configuration v);

void setLB(Configuration v, int l);

int getLB(Configuration v);

/**
 * flip the src configuration vertically and store it into the 
 * dest configuration
 */
void flipVertical(Configuration src, Configuration dest);

/**
 * flip the src configuration horizontally and store it into the 
 * dest configuration
 */
void flipHorizontal(Configuration src, Configuration dest);

#endif
