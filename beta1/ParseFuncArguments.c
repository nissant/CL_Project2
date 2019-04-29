#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#include "ParseFuncArguments.h"

void InitCurrentArgs(GrepFuncArguments* currInputArgs) {
	currInputArgs->fileToGrepPtr = NULL;
	currInputArgs->wordToGrep    = NULL;
}

#define REG_EXPRESSION_ARG_LENGTH 2

bool IsParamOfGrep(const char* argument) {
	if ((strlen(argument) == REG_EXPRESSION_ARG_LENGTH) && (*argument == '-')) { return true; }
	return false;
}

bool IsFileBasedGrep(const char* secondToLastMainArg) {
	if (strcmp(secondToLastMainArg, "my_grep")   == 0) { return false; }
	if (strcmp(secondToLastMainArg, "./my_grep") == 0) { return false; }
	if (IsParamOfGrep(secondToLastMainArg))            { return false; }
	return true;
}

bool SetFileToGrepFromCurrentArgs(const char* filePath, GrepFuncArguments* currInputArgs) {
	currInputArgs->fileToGrepPtr = fopen(filePath, "r");
	return currInputArgs->fileToGrepPtr == NULL ? false : true;
}

void SetWordToGrepFromCurrentArgs(const char* word, GrepFuncArguments* currInputArgs) {
	int lenOfWordToGrep = strlen(word);
	currInputArgs->wordToGrepLength = lenOfWordToGrep;
	currInputArgs->wordToGrep = (char *)malloc((lenOfWordToGrep + 1) * sizeof(char));
	strcpy(currInputArgs->wordToGrep, word); 
}

void ReleaseArgsMemory(GrepFuncArguments* currInputArgs) {
	if  (currInputArgs->fileToGrepPtr != NULL) { fclose(currInputArgs->fileToGrepPtr); }
	free(currInputArgs->wordToGrep);
}
