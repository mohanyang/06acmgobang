#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "evaluator.h"
#include "advstat.h"

#define MAX_PATTERNS 1024
#define PATTERN_LENGTH 8
#define MAX_STATE 1024
#define MAX_MATCH 100

static char pattern[MAX_PATTERNS][PATTERN_LENGTH];
static int plen[MAX_PATTERNS];
static int ptype[MAX_PATTERNS];
static int nPattern;

static int trie[MAX_STATE][4];
static int trie_pi[MAX_STATE];
static int rel_pattern[MAX_STATE];
//static int rel_value[MAX_STATE];
//static int rel_next[MAX_STATE];
static int nState, nList;

void add_pattern(const char *pat, int ty) {
  ++nPattern;
  plen[nPattern] = strlen(pat);
  ptype[nPattern] = ty;
  int i;
  for (i = 0; i < plen[nPattern]; ++i) {
    switch (pat[i]) {
    case '.' : 
      pattern[nPattern][i] = NONE;
      break;
    case 'b' :
      pattern[nPattern][i] = BLACK;
      break;
    case 'w' :
      pattern[nPattern][i] = WHITE;
      break;
    case 'x' :
      pattern[nPattern][i] = FORBID;
      break;
    }
  }
}

int withoutX(const char *pat) {
  const char *p;
  for (p = pat; *p != 0; ++p)
    if (*p == 'x')
      return 0;
  return 1;
}

void reverse(char *pat) {
  int len = strlen(pat), i;
  char t;
  for (i = 0; i < len-1-i; ++i) {
    t = pat[i];
    pat[i] = pat[len-1-i];
    pat[len-1-i] = t;
  }
}

void exchange(char *pat) {
  char *p;
  for (p = pat; *p != 0; ++p) {
    if (*p == 'b')
      *p = 'w';
    else if (*p == 'w')
      *p = 'b';
  }
}

void expand_pattern(const char *pat, int ty) {
  char p[PATTERN_LENGTH];
  int palindrome = 1;
  add_pattern(pat, ty);
  strcpy(p, pat);
  reverse(p);
  if (strcmp(pat, p) != 0) {
    palindrome = 0;
    add_pattern(p, ty);
  }
  if (withoutX(p)) {
    exchange(p);
    add_pattern(p, -ty);
    if (!palindrome) {
      reverse(p);
      add_pattern(p, -ty);
    }
  }
}

//void add_related_pattern(int state, int pat) {
//  ++nList;
//  rel_value[nList] = pat;
//  rel_next[nList] = rel_pattern[state];
//  rel_pattern[state] = nList;
//}

void build_state_machine() {
  int i, j, k, s;
  nState = 1;
  nList = 0;
  memset(trie, 0, sizeof(trie));
  memset(trie_pi, 0, sizeof(trie_pi));
  memset(rel_pattern, 0, sizeof(rel_pattern));
  for (i = 1; i <= nPattern; ++i) {
    for (s = 1, j = 0; j < plen[i]; ++j) {
      k = pattern[i][j];
      if (trie[s][k] == 0) {
	trie[s][k] = ++nState;
      }
      s = trie[s][k];
    }
    rel_pattern[s] = i;
    //    add_related_pattern(s, i);
  }

  int q[MAX_STATE];
  int l, r;
  for (l = r = i = 0; i < 4; ++i) {
    if (trie[1][i]) {
      trie_pi[trie[1][i]] = 1;
      q[r++] = trie[1][i];
    } else {
      trie[1][i] = 1;
    }
  }

  for (; l < r; ++l) {
    i = q[l];
    if (rel_pattern[i] == 0 && rel_pattern[trie_pi[i]] != 0) {
      rel_pattern[i] = rel_pattern[trie_pi[i]];
      //    for (j = rel_pattern[trie_pi[i]]; j != 0; j = rel_next[j]) {
      //      add_related_pattern(i, rel_value[j]);
    }
    for (j = 0; j < 4; ++j) {
      if (trie[i][j]) {
	trie_pi[trie[i][j]] = trie[trie_pi[i]][j];
	q[r++] = trie[i][j];
      } else {
	trie[i][j] = trie[trie_pi[i]][j];
      }
    }
  }
}

void initializeEvaluate() {
  FILE *file = fopen("pattern.conf", "r");
  char line[1000];
  nPattern = 0;
  while (fgets(line, 1000, file) != NULL) {
    char *first;
    for (first = line; *first == ' ' || *first == '\t'; ++first);
    if (*first == 0 || *first == '\n' || *first == '#')
      continue;
    first = strtok(line, " \t");
    int ty = atoi(strtok(NULL, " \t"));
    expand_pattern(first, ty);
  }
  fclose(file);

  build_state_machine();
}

struct interval {
  int left, right, type;
} match_result[MAX_MATCH];
int nMatch;
AdvancedStat astat;

/*
void match(Configuration v, int y0, int x0, int dy, int dx, int n) {
  int state, i, j, pid, y = y0, x = x0;
  nMatch = 0;
  for (state = 1, i = 0; i < n; ++i, y += dy, x += dx) {
    state = trie[state][v->data[y][x]];
    pid = rel_pattern[state];
    if (pid) {
      int left = i - plen[pid] + 1;
      while (nMatch > 0 && match_result[nMatch].left >= left)
	--nMatch;
      ++nMatch;
      match_result[nMatch].left = left;
      match_result[nMatch].right = i;
      match_result[nMatch].type = ptype[pid];
    }
  }

  for (i = 1; i <= nMatch; ++i) {
    int ty = abs(match_result[i].type);
    int owner = match_result[i].type < 0;
    ++astat.stat[ty][owner];
    for (j = match_result[i].left, y = y0 + j * dy, x = x0 + j * dx; j <= match_result[i].right; ++j, y += dy, x += dx) {
#ifdef TEST_PATTERN
      printf("%d %d %d %d\n", y0+j*dy, x0+j*dx, ty, owner);
#endif
      ++(v->statistics[y][x][ty][owner]);
    }
  }
}
*/

#define match_state_machine(v, y0, x0, dy, dx, n) {			\
    nMatch = 0;								\
    for (state = 1, i = 0; i < n; ++i, y += dy, x += dx) {		\
      state = trie[state][v->data[y][x]];				\
      pid = rel_pattern[state];						\
      if (pid) {							\
	int left = i - plen[pid] + 1;					\
	while (nMatch > 0 && match_result[nMatch].left >= left)		\
	  --nMatch;							\
	++nMatch;							\
	match_result[nMatch].left = left;				\
	match_result[nMatch].right = i;					\
	match_result[nMatch].type = ptype[pid];				\
      }									\
    }									\
  }

#define match(v, y0, x0, dy, dx, n) {					\
    int state, i, pid, y = y0, x = x0;					\
    match_state_machine(v, y0, x0, dy, dx, n);				\
    for (i = 1; i <= nMatch; ++i) {					\
      int ty = abs(match_result[i].type);				\
      int owner = match_result[i].type < 0;				\
      ++astat.stat[ty][owner];						\
    }									\
  }

#define match_and_mark(v, y0, x0, dy, dx, n) {				\
    int state, i, j, pid, y = y0, x = x0;				\
    match_state_machine(v, y0, x0, dy, dx, n);				\
    for (i = 1; i <= nMatch; ++i) {					\
      int ty = abs(match_result[i].type);				\
      int owner = match_result[i].type < 0;				\
      for (j = match_result[i].left, y = y0 + j * dy, x = x0 + j * dx; j <= match_result[i].right; ++j, y += dy, x += dx) { \
	++(v->statistics[y][x][ty][owner]);				\
      }									\
    }									\
  }

/*
#define adjust(score, color) ((color) == BLACK ? (score) : -(score))

int _getScore(AdvancedStat *info, PEBBLE_COLOR c) {
  
  int now = getOppositePlayer(c);

  if (info->stat[FIVE][c])
    return adjust(now, -INFINITY);
  if (info->stat[FIVE][now])
    return adjust(now, INFINITY);
  if (info->stat[ACTIVE_FOUR][now] || info->stat[AFOUR][now])
    return adjust(now, INFINITY - 1);
  if (info->stat[ACTIVE_FOUR][c])
    return adjust(now, -INFINITY + 1);
  if (info->stat[AFOUR][c] && info->stat[ACTIVE_THREE][c])
    return adjust(now, -INFINITY + 1);

  int score = 0;
  score -= info->stat[AFOUR][c] * (INFINITY / 2 - 1);
  score += info->stat[ACTIVE_THREE][now] * (INFINITY / 2 - 1);
  score -= info->stat[ACTIVE_THREE][  c] * (INFINITY / 2 - 1);
  score += info->stat[SLEEPY_THREE][now] * (INFINITY / 10);
  score -= info->stat[SLEEPY_THREE][  c] * (INFINITY / 10);
  score += info->stat[ACTIVE_TWO][now] * (INFINITY / 20);
  score -= info->stat[ACTIVE_TWO][  c] * (INFINITY / 20);

  if (score >  INFINITY - 10) score =  INFINITY - 10;
  if (score < -INFINITY + 10) score = -INFINITY + 10;
  return adjust(now, score) + info->assoc->localPriority;
}
*/

int evaluateBoard(Configuration v, PEBBLE_COLOR c) {
  memset(&astat, 0, sizeof(astat));

  //  for (y = 0; y < 15; ++y) {
  //    match(v, y, 0, 0, 1, 15);
  //  }
  match(v,  0, 0, 0, 1, 15);
  match(v,  1, 0, 0, 1, 15);
  match(v,  2, 0, 0, 1, 15);
  match(v,  3, 0, 0, 1, 15);
  match(v,  4, 0, 0, 1, 15);
  match(v,  5, 0, 0, 1, 15);
  match(v,  6, 0, 0, 1, 15);
  match(v,  7, 0, 0, 1, 15);
  match(v,  8, 0, 0, 1, 15);
  match(v,  9, 0, 0, 1, 15);
  match(v, 10, 0, 0, 1, 15);
  match(v, 11, 0, 0, 1, 15);
  match(v, 12, 0, 0, 1, 15);
  match(v, 13, 0, 0, 1, 15);
  match(v, 14, 0, 0, 1, 15);

  //  for (x = 0; x < 15; ++x) {
  //    match(v, 0, x, 1, 0, 15);
  //  }
  match(v, 0,  0, 1, 0, 15);
  match(v, 0,  1, 1, 0, 15);
  match(v, 0,  2, 1, 0, 15);
  match(v, 0,  3, 1, 0, 15);
  match(v, 0,  4, 1, 0, 15);
  match(v, 0,  5, 1, 0, 15);
  match(v, 0,  6, 1, 0, 15);
  match(v, 0,  7, 1, 0, 15);
  match(v, 0,  8, 1, 0, 15);
  match(v, 0,  9, 1, 0, 15);
  match(v, 0, 10, 1, 0, 15);
  match(v, 0, 11, 1, 0, 15);
  match(v, 0, 12, 1, 0, 15);
  match(v, 0, 13, 1, 0, 15);
  match(v, 0, 14, 1, 0, 15);
  
  //  for (y = 0; y < 11; ++y) {
  //    match(v, y, 0, 1, 1, 15 - y);
  //  }
  match(v,  0, 0, 1, 1, 15);
  match(v,  1, 0, 1, 1, 14);
  match(v,  2, 0, 1, 1, 13);
  match(v,  3, 0, 1, 1, 12);
  match(v,  4, 0, 1, 1, 11);
  match(v,  5, 0, 1, 1, 10);
  match(v,  6, 0, 1, 1,  9);
  match(v,  7, 0, 1, 1,  8);
  match(v,  8, 0, 1, 1,  7);
  match(v,  9, 0, 1, 1,  6);
  match(v, 10, 0, 1, 1,  5);

  //  for (x = 1; x < 11; ++x) {
  //    match(v, 0, x, 1, 1, 15 - x);
  //  }
  match(v, 0,  1, 1, 1, 14);
  match(v, 0,  2, 1, 1, 13);
  match(v, 0,  3, 1, 1, 12);
  match(v, 0,  4, 1, 1, 11);
  match(v, 0,  5, 1, 1, 10);
  match(v, 0,  6, 1, 1,  9);
  match(v, 0,  7, 1, 1,  8);
  match(v, 0,  8, 1, 1,  7);
  match(v, 0,  9, 1, 1,  6);
  match(v, 0, 10, 1, 1,  5);
  
  //  for (y = 4; y < 15; ++y) {
  //    match(v, y, 0, -1, 1, y + 1);
  //  }
  match(v,  4, 0, -1, 1,  5);
  match(v,  5, 0, -1, 1,  6);
  match(v,  6, 0, -1, 1,  7);
  match(v,  7, 0, -1, 1,  8);
  match(v,  8, 0, -1, 1,  9);
  match(v,  9, 0, -1, 1, 10);
  match(v, 10, 0, -1, 1, 11);
  match(v, 11, 0, -1, 1, 12);
  match(v, 12, 0, -1, 1, 13);
  match(v, 13, 0, -1, 1, 14);
  match(v, 14, 0, -1, 1, 15);

  //  for (x = 1; x < 11; ++x) {
  //    match(v, 14, x, -1, 1, 15 - x);
  //  }
  match(v, 14,  1, -1, 1, 14);
  match(v, 14,  2, -1, 1, 13);
  match(v, 14,  3, -1, 1, 12);
  match(v, 14,  4, -1, 1, 11);
  match(v, 14,  5, -1, 1, 10);
  match(v, 14,  6, -1, 1,  9);
  match(v, 14,  7, -1, 1,  8);
  match(v, 14,  8, -1, 1,  7);
  match(v, 14,  9, -1, 1,  6);
  match(v, 14, 10, -1, 1,  5);

  astat.assoc=v;
#ifdef TEST_PATTERN
  return 0;
#else
  return getScore(&astat, c);
#endif

}

void calculateStat(Configuration v) {
  memset(v->statistics, 0, sizeof(v->statistics));

  //  for (y = 0; y < 15; ++y) {
  //    match_and_mark(v, y, 0, 0, 1, 15);
  //  }
  match_and_mark(v,  0, 0, 0, 1, 15);
  match_and_mark(v,  1, 0, 0, 1, 15);
  match_and_mark(v,  2, 0, 0, 1, 15);
  match_and_mark(v,  3, 0, 0, 1, 15);
  match_and_mark(v,  4, 0, 0, 1, 15);
  match_and_mark(v,  5, 0, 0, 1, 15);
  match_and_mark(v,  6, 0, 0, 1, 15);
  match_and_mark(v,  7, 0, 0, 1, 15);
  match_and_mark(v,  8, 0, 0, 1, 15);
  match_and_mark(v,  9, 0, 0, 1, 15);
  match_and_mark(v, 10, 0, 0, 1, 15);
  match_and_mark(v, 11, 0, 0, 1, 15);
  match_and_mark(v, 12, 0, 0, 1, 15);
  match_and_mark(v, 13, 0, 0, 1, 15);
  match_and_mark(v, 14, 0, 0, 1, 15);

  //  for (x = 0; x < 15; ++x) {
  //    match_and_mark(v, 0, x, 1, 0, 15);
  //  }
  match_and_mark(v, 0,  0, 1, 0, 15);
  match_and_mark(v, 0,  1, 1, 0, 15);
  match_and_mark(v, 0,  2, 1, 0, 15);
  match_and_mark(v, 0,  3, 1, 0, 15);
  match_and_mark(v, 0,  4, 1, 0, 15);
  match_and_mark(v, 0,  5, 1, 0, 15);
  match_and_mark(v, 0,  6, 1, 0, 15);
  match_and_mark(v, 0,  7, 1, 0, 15);
  match_and_mark(v, 0,  8, 1, 0, 15);
  match_and_mark(v, 0,  9, 1, 0, 15);
  match_and_mark(v, 0, 10, 1, 0, 15);
  match_and_mark(v, 0, 11, 1, 0, 15);
  match_and_mark(v, 0, 12, 1, 0, 15);
  match_and_mark(v, 0, 13, 1, 0, 15);
  match_and_mark(v, 0, 14, 1, 0, 15);
  
  //  for (y = 0; y < 11; ++y) {
  //    match_and_mark(v, y, 0, 1, 1, 15 - y);
  //  }
  match_and_mark(v,  0, 0, 1, 1, 15);
  match_and_mark(v,  1, 0, 1, 1, 14);
  match_and_mark(v,  2, 0, 1, 1, 13);
  match_and_mark(v,  3, 0, 1, 1, 12);
  match_and_mark(v,  4, 0, 1, 1, 11);
  match_and_mark(v,  5, 0, 1, 1, 10);
  match_and_mark(v,  6, 0, 1, 1,  9);
  match_and_mark(v,  7, 0, 1, 1,  8);
  match_and_mark(v,  8, 0, 1, 1,  7);
  match_and_mark(v,  9, 0, 1, 1,  6);
  match_and_mark(v, 10, 0, 1, 1,  5);

  //  for (x = 1; x < 11; ++x) {
  //    match_and_mark(v, 0, x, 1, 1, 15 - x);
  //  }
  match_and_mark(v, 0,  1, 1, 1, 14);
  match_and_mark(v, 0,  2, 1, 1, 13);
  match_and_mark(v, 0,  3, 1, 1, 12);
  match_and_mark(v, 0,  4, 1, 1, 11);
  match_and_mark(v, 0,  5, 1, 1, 10);
  match_and_mark(v, 0,  6, 1, 1,  9);
  match_and_mark(v, 0,  7, 1, 1,  8);
  match_and_mark(v, 0,  8, 1, 1,  7);
  match_and_mark(v, 0,  9, 1, 1,  6);
  match_and_mark(v, 0, 10, 1, 1,  5);
  
  //  for (y = 4; y < 15; ++y) {
  //    match_and_mark(v, y, 0, -1, 1, y + 1);
  //  }
  match_and_mark(v,  4, 0, -1, 1,  5);
  match_and_mark(v,  5, 0, -1, 1,  6);
  match_and_mark(v,  6, 0, -1, 1,  7);
  match_and_mark(v,  7, 0, -1, 1,  8);
  match_and_mark(v,  8, 0, -1, 1,  9);
  match_and_mark(v,  9, 0, -1, 1, 10);
  match_and_mark(v, 10, 0, -1, 1, 11);
  match_and_mark(v, 11, 0, -1, 1, 12);
  match_and_mark(v, 12, 0, -1, 1, 13);
  match_and_mark(v, 13, 0, -1, 1, 14);
  match_and_mark(v, 14, 0, -1, 1, 15);

  //  for (x = 1; x < 11; ++x) {
  //    match_and_mark(v, 14, x, -1, 1, 15 - x);
  //  }
  match_and_mark(v, 14,  1, -1, 1, 14);
  match_and_mark(v, 14,  2, -1, 1, 13);
  match_and_mark(v, 14,  3, -1, 1, 12);
  match_and_mark(v, 14,  4, -1, 1, 11);
  match_and_mark(v, 14,  5, -1, 1, 10);
  match_and_mark(v, 14,  6, -1, 1,  9);
  match_and_mark(v, 14,  7, -1, 1,  8);
  match_and_mark(v, 14,  8, -1, 1,  7);
  match_and_mark(v, 14,  9, -1, 1,  6);
  match_and_mark(v, 14, 10, -1, 1,  5);

}

#ifdef TEST_PATTERN

int main() {
  initializeEvaluate();
  printf("%d\n", nPattern);
  int i, j;
  for (i = 1; i <= nPattern; ++i) {
    for (j = 0; j < plen[i]; ++j) {
      switch (pattern[i][j]) {
      case NONE:
	printf(".");
	break;
      case BLACK:
	printf("b");
	break;
      case WHITE:
	printf("w");
	break;
      case FORBID:
	printf("x");
	break;
      }
    }
    printf("\t%d\n", ptype[i]);
  }

  struct BaseNode node;
  /*
  memset(&node, 0, sizeof(node));
  node.data[0][0] = 0;
  node.data[0][1] = 2;
  node.data[0][2] = 2;
  node.data[0][3] = 0;
  node.data[0][4] = 2;
  node.data[0][5] = 2;

  node.data[7][7] = 1;
  node.data[8][8] = 1;
  node.data[10][10] = 1;

  evaluateBoard(&node, 1);
  */

  memset(&node, 0, sizeof(node));
  node.data[7][7] = 1;
  node.data[7][8] = 1;
  node.data[7][9] = 1;
  calculateStat(&node);
  printf("%d\n", node.statistics[7][5][ACTIVE_THREE][0]);

  return 0;
}

#endif


#ifdef EXPORT_GRAPH

char *getLabel(int node, char *label) {
  if (node == 1)
    return "START";
  else if (!rel_pattern[node])
    return "";
  int pid = rel_pattern[node];
  int i;
  for (i = 0; i < plen[pid]; ++i) {
    switch (pattern[pid][i]) {
    case BLACK :
      label[i] = 'b';
      break;
    case WHITE :
      label[i] = 'w';
      break;
    default :
      label[i] = '.';
    }
  }
  label[plen[pid]] = 0;
  return label;
}

int main() {
  int i;
  char label[100];
  initializeEvaluate();
  for (i = 1; i <= nState; ++i) {
    printf("node%d [label=\"%s\", peripheries=%d]\n", i, getLabel(i, label), 1+(i==1||rel_pattern[i]));
  }
  for (i = 1; i <= nState; ++i) {
    if (trie[i][NONE])
      printf("node%d -> node%d [label=\".\"]\n", i, trie[i][NONE]);
    if (trie[i][BLACK])
      printf("node%d -> node%d [label=\"b\"]\n", i, trie[i][BLACK]);
    if (trie[i][WHITE])
      printf("node%d -> node%d [label=\"w\"]\n", i, trie[i][WHITE]);
  }
  return 0;
}

#endif
