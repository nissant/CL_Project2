#ifndef __PARSE_FUNC_ARGUMENTS_H
#define __PARSE_FUNC_ARGUMENTS_H

#include <stdio.h>

#define NUM_OF_GREP_POSSIBLES_PARAMS 8

typedef struct GrepFuncArguments {
	FILE* fileToGrepPtr;
	char* phraseToGrep;
	int phraseToGrepLength;
	int numOfExtraLinesToShow;
	bool validGrepParams[NUM_OF_GREP_POSSIBLES_PARAMS];
} GrepFuncArguments;

enum GrepParams {
	CAPITAL_A = 0,
	LOWER_B,
	LOWER_C,
	LOWER_I,
	LOWER_N,
	LOWER_V,
	LOWER_X,
	CAPITAL_E
};

void InitCurrentArgs(GrepFuncArguments* currInputArgs);
bool IsFileBasedGrep(const char* secondToLastMainArg);
void ParseArgumentsAndSetInStruct(char *argv[], int argc, GrepFuncArguments* currInputArgs,
	                              bool isFileBasedGrep);
void ReleaseArgsMemory(GrepFuncArguments* currInputArgs);
#endif