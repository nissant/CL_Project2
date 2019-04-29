#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "GrepText.h"

bool InputWordInLine(const GrepFuncArguments currInputArgs, char* lineFromFile) {
	int lineLength  = strlen(lineFromFile);
	char* endOfLine = lineFromFile + lineLength;
	char* linePtr;

	for (linePtr = lineFromFile; linePtr < endOfLine; linePtr++) {
		if (strncmp(currInputArgs.wordToGrep, lineFromFile,
			        (size_t)currInputArgs.wordToGrepLength) == 0) {
			return true;
		}
	}
	return false;
}

void GoOverTextForGrep(const GrepFuncArguments currInputArgs, bool isFileBasedGrep) {
	char* newLineFromFile     = NULL;
	size_t numOfBytesFromFile = 0;

	while (getline(&newLineFromFile, &numOfBytesFromFile,
		   isFileBasedGrep? currInputArgs.fileToGrepPtr : stdin) != -1) {
		if (InputWordInLine(currInputArgs, newLineFromFile)) {
			printf("%s", newLineFromFile);
		}
	}
	free(newLineFromFile);
}