#ifndef __PARSE_FUNC_ARGUMENTS_H
#define __PARSE_FUNC_ARGUMENTS_H

#include <stdio.h>

typedef struct GrepFuncArguments {
	FILE* fileToGrepPtr;
	char* wordToGrep;
	int wordToGrepLength;
} GrepFuncArguments;

void InitCurrentArgs(GrepFuncArguments* currInputArgs);
bool IsFileBasedGrep(const char* secondToLastMainArg);
bool SetFileToGrepFromCurrentArgs(const char* filePath, GrepFuncArguments* currInputArgs);
void SetWordToGrepFromCurrentArgs(const char* word, GrepFuncArguments* currInputArgs);
void ReleaseArgsMemory(GrepFuncArguments* currInputArgs);
#endif