#include "gobang_JNIAdapter.h"
#include "engine.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_gobang_JNIAdapter_reset(JNIEnv *env, jobject cl)
{
	initializeEngie();
}


JNIEXPORT jint JNICALL Java_gobang_JNIAdapter_GenerateChessInfo(JNIEnv *env, jobject cl)
{
	jint ret = (jint)generate();
	return ret;
}

JNIEXPORT void JNICALL Java_gobang_JNIAdapter_playChess(JNIEnv *env, jobject cl, jint info)
{
	playchess(info);
}
