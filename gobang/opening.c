#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "opening.h"

#define MAX_NODE 320000
#define MAX_EDGE 320000

#define COOR_TO_ID(y, x) ((y)*15+(x))

static int trie[MAX_NODE];
static int node[MAX_EDGE];
static int next[MAX_EDGE];
static int edge_id[MAX_EDGE];
static int childs[MAX_NODE];
static int nNode, nEdge;

static
int has_child(int state, int y, int x) {
  int i, k = COOR_TO_ID(y, x);
  for (i = trie[state]; i; i = next[i])
    if (edge_id[i] == k)
      return 1;
  return 0;
}

int get_child_count(int state) {
  return childs[state];
}

#ifdef TEST_OPENING

static
void print_child(int state) {
  int i;
  for (i = trie[state]; i; i = next[i])
    printf("%d %d\n", edge_id[i]/15, edge_id[i]%15);
}

#endif

int move_opening(int state, int y, int x) {
  int i, k = COOR_TO_ID(y, x);
  for (i = trie[state]; i; i = next[i]) {
    if (edge_id[i] == k)
      return node[i];
  }
  return 0;
}

static
void add_opening(int *y, int *x, int len) {
  int s = 1, i, k;
  //  for (i = 0; i < len; ++i) {
  //    printf("(%d, %d) ", y[i], x[i]);
  //  }
  //  printf("\n");
  for (i = 0; i < len; ++i) {
    k = COOR_TO_ID(y[i], x[i]);
    if (!has_child(s, y[i], x[i])) {
      node[++nEdge] = ++nNode;
      edge_id[nEdge] = k;
      next[nEdge] = trie[s];
      trie[s] = nEdge;
      ++childs[s];
    }
    s = move_opening(s, y[i], x[i]);
  }
}

static
void rotate(int *y, int *x, int len) {
  int i;
  for (i = 0; i < len; ++i) {
    int ty = y[i] - 7, tx = x[i] - 7;
    y[i] = 7 - tx;
    x[i] = 7 + ty;
  }
}

static
void reflect(int *y, int *x, int len) {
  int i;
  for (i = 0; i < len; ++i) {
    x[i] = 14 - x[i];
  }
}

static
void expand(int *y, int *x, int len) {
  add_opening(y, x, len);
  rotate(y, x, len);
  add_opening(y, x, len);
  rotate(y, x, len);
  add_opening(y, x, len);
  rotate(y, x, len);
  add_opening(y, x, len);
  reflect(y, x, len);
  add_opening(y, x, len);
  rotate(y, x, len);
  add_opening(y, x, len);
  rotate(y, x, len);
  add_opening(y, x, len);
  rotate(y, x, len);
  add_opening(y, x, len);
}

void initialize_opening() {
  FILE *file = fopen("open.txt", "r");
  char line[200];
  int pos_y[100];
  int pos_x[100];
  nNode = 1;
  nEdge = 0;
  while (fscanf(file, "%s", line) == 1) {
    int len = strlen(line), i;
    for (i = 0; i < len; i += 2) {
      pos_y[i>>1] = line[i]-'A';
      pos_x[i>>1] = line[i+1] - 'a';
    }
    expand(pos_y, pos_x, len >> 1);
  }
  fclose(file);
}

#ifdef TEST_OPENING

int main() {
  initialize_opening();
  printf("%d %d\n", nNode, nEdge);
  int state = 1, y, x;
  print_child(1);
  printf("~~~~~~~~~~~~\n");
  state = move_opening(state, 7, 7);
  for (y = 0; y < 15; ++y)
    for (x = 0; x < 15; ++x)
      if (has_child(state, y, x))
	printf("%d %d\n", y, x);
  return 0;
}

#endif
