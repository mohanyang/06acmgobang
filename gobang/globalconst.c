#include "globalconst.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initializeConstant(){
	FILE *fp=fopen("gobang.conf", "r");
	// TODO
	fclose(fp);
}
