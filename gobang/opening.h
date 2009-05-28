#ifndef _OPENING_H_
#define _OPENING_H_

#define BEGINNING_STATE 1

void initialize_opening();

int move_opening(int state, int y, int x);

int get_child_count(int state);

#endif
