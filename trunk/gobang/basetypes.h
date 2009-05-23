#ifndef _BASETYPES_H_
#define _BASETYPES_H_

enum _NODETYPE {
	// black=maxnode, white=minnode
	MAXNODE, MINNODE
};
typedef enum _NODETYPE NODETYPE;

enum _PEBBLE_COLOR {
	NONE=0, BLACK=1, WHITE=2
};
typedef enum _PEBBLE_COLOR PEBBLE_COLOR;

struct BaseNode {
	int depth;
	NODETYPE type;
	/**
	 * hboard: stores the horizontal lines
	 * vboard: stores the vertical lines
	 * lines are divided into two parts, the more significant
	 * section is for white, and the less significant
	 * section is for black
	 */
	int *hboard, *vboard;
	int lowerbound, upperbound;
	// TODO calculate hash when possible
	int step;
	int hash;
};

typedef struct BaseNode *Configuration;

struct Position {
	int x;
	int y;
};

typedef struct Position Move;

PEBBLE_COLOR getOppositePlayer(PEBBLE_COLOR p);

void initializeBaseType();

void applyMove(Configuration v, Move m);

void undoMove(Configuration v, Move m);

Configuration allocConfiguration();

/**
 * initialize the current configuration,
 * and make the current player p
 */
void initializeConfiguration(Configuration v, PEBBLE_COLOR p);

PEBBLE_COLOR getColor(Configuration v, int x, int y);

PEBBLE_COLOR getMover(Configuration v);

void setMover(Configuration v, PEBBLE_COLOR c);

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
 * flip the src configuration vertically and store 
 * it into the dest configuration
 */
void flipVertical(Configuration src, Configuration dest);

void selfFlipVertical(Configuration v);

/**
 * flip the src configuration horizontally and 
 * store it into the dest configuration
 */
void flipHorizontal(Configuration src, Configuration dest);

void selfFlipHorizontal(Configuration v);

void rotateBoard(Configuration src, Configuration dest);

void selfRotateBoard(Configuration v);

#endif
