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

void match(Configuration v, int y0, int x0, int dy, int dx, int n) {
  int state, i, j, pid, y = y0, x = x0;
  nMatch = 0;
  for (state = 1, i = 0; i < n; ++i, y += dy, x += dx) {
    state = trie[state][v->data[y][x]];
    pid = rel_pattern[state];
    if (pid) {
      match_result[nMatch].left = i - plen[pid];
      match_result[nMatch].right = i;
      match_result[nMatch].type = ptype[pid];
      ++nMatch;
    }
  }

  for (i = 0; i < nMatch; ++i) {
    for (j = i + 1; j < nMatch; ++j) {
      if (match_result[j].left <= match_result[i].left)
	break;
    }
    if (j == nMatch) {
      int ty = abs(match_result[i].type);
      int owner = match_result[i].type < 0;
      ++astat.stat[ty][owner];
      for (j = match_result[i].left; j <= match_result[i].right; ++j) {
	++v->statistics[y0+j*dy][x0+j*dx][ty][owner];
      }
    }
  }
}

int evaluateBoard(Configuration v, PEBBLE_COLOR c) {
  int y, x;
  memset(v->statistics, 0, sizeof(v->statistics));
  memset(&astat, 0, sizeof(astat));

  for (y = 0; y < 15; ++y) {
    match(v, y, 0, 0, 1, 15);
  }
  for (x = 0; x < 15; ++x) {
    match(v, 0, x, 1, 0, 15);
  }

  for (y = 0; y < 11; ++y) {
    match(v, y, 0, 1, 1, 15 - y);
  }
  for (x = 1; x < 11; ++x) {
    match(v, 0, x, 1, 1, 15 - x);
  }

  for (y = 4; y < 15; ++y) {
    match(v, y, 0, -1, 1, y + 1);
  }
  for (x = 1; x < 11; ++x) {
    match(v, 14, x, -1, 1, 15 - x);
  }

  astat.assoc=v;
  return getScore(&astat, c);

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

  return 0;
}

#endif
