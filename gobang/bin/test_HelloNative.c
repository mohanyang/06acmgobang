#include "test_HelloNative.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_test_HelloNative_greeting(JNIEnv *env, jclass cl)
{
	printf("Hello Native World!\n");
}
