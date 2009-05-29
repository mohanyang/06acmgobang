#include "globalconst.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initializeConstant(){
  FILE *fp=fopen("gobang.conf", "r");
  
  char line[200];
  while (fgets(line, 200, fp) != NULL) {
    char *first;
    for (first = line; *first == ' ' || *first == '\t'; ++first);
    if (*first == 0 || *first == '\n' || *first == '#')
      continue;
    char *key, *value;
    key = strtok(line, " \t");
    value = strtok(NULL, " \t");
    value = strtok(NULL, " \t");
    if (strcmp(key, "timelimit") == 0) {
      const_tls = atoi(value) / 1000;
      const_tlm = atoi(value) % 1000;
    } else if (strcmp(key, "allowforbidden") == 0) {
      const_forbidden = strcmp(value, "true") == 0;
    }
  }
  
  fclose(fp);
}

int timeLimitSecond() {
	return const_tls;
}

int timeLimitMillis() {
	return const_tlm;
}

int allowForbidden() {
	return const_forbidden;
}
