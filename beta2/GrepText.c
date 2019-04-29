#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "GrepText.h"

bool IsInputPhraseInLine(const GrepFuncArguments currInputArgs, char* lineFromFile) {
	int lineLength  = strlen(lineFromFile);
	char* endOfLine = lineFromFile + lineLength;
	char* linePtr;

	for (linePtr = lineFromFile; linePtr < endOfLine; linePtr++) {
		if (currInputArgs.validGrepParams[LOWER_I]?
			(strncasecmp(currInputArgs.phraseToGrep, linePtr, (size_t)currInputArgs.phraseToGrepLength) == 0) :
			(strncmp(currInputArgs.phraseToGrep, linePtr, (size_t)currInputArgs.phraseToGrepLength) == 0)) {
			return true;
		}
	}
	return false;
}

bool InputPhraseIsTheLine(const GrepFuncArguments currInputArgs, char* lineFromFile) {
	return currInputArgs.validGrepParams[LOWER_I] ?
		(strncasecmp(currInputArgs.phraseToGrep, lineFromFile, (size_t)currInputArgs.phraseToGrepLength) == 0) :
		(strncmp(currInputArgs.phraseToGrep, lineFromFile, (size_t)currInputArgs.phraseToGrepLength) == 0);
}

bool IsEmptyLine(char* line) {
	int lineLength = strlen(line);
	char* endOfLine = line + lineLength;
	char* linePtr;

	for (linePtr = line; linePtr < endOfLine; linePtr++) {
		if (*linePtr != '\n' || *linePtr != ' ' || *linePtr != '\t') { return false; }
	}
	return true;
}

bool DoesLineMatchesRequests(char* lineFromFile, const GrepFuncArguments currInputArgs) {
	if (currInputArgs.validGrepParams[LOWER_X]) {
		if (!InputPhraseIsTheLine(currInputArgs, lineFromFile)) {
			return false;
		}
	} else if (currInputArgs.validGrepParams[LOWER_V]) {
		if (IsInputPhraseInLine(currInputArgs, lineFromFile) || IsEmptyLine(lineFromFile)) {
			return false;
		}
	} else if (!IsInputPhraseInLine(currInputArgs, lineFromFile)) { 
		return false;
	}
	return true;
}

void PrintLine(int lineNumber, int numOfBytesFromFileBeginning, char* lineFromFile, char separator,
	           const GrepFuncArguments currInputArgs) {
	if (currInputArgs.validGrepParams[LOWER_N]) { printf("%d%c", lineNumber, separator); }
	if (currInputArgs.validGrepParams[LOWER_B]) { printf("%d%c", numOfBytesFromFileBeginning, separator); }
	printf("%s", lineFromFile);
}

void GoOverTextForGrepAndPrint(const GrepFuncArguments currInputArgs, bool isFileBasedGrep) {
	char* newLineFromFile     = NULL;
	size_t numOfBytesFromFile = 0;
	int lineNumber            = 0;
	int numOfMatchingLines    = 0;
	int numOfBytesFromFileBeginning = 0;
	int extraLinesToPrint = 0;

	while (getline(&newLineFromFile, &numOfBytesFromFile,
		   isFileBasedGrep? currInputArgs.fileToGrepPtr : stdin) != -1) {
		lineNumber++;
		if (DoesLineMatchesRequests(newLineFromFile, currInputArgs)) {
			if (!currInputArgs.validGrepParams[LOWER_C]) {
				PrintLine(lineNumber, numOfBytesFromFileBeginning, newLineFromFile, ':', currInputArgs);
				extraLinesToPrint = currInputArgs.numOfExtraLinesToShow;
			}
			numOfMatchingLines++;
		} else if (extraLinesToPrint) {
			PrintLine(lineNumber, numOfBytesFromFileBeginning, newLineFromFile, '-', currInputArgs);
			extraLinesToPrint--;
		}
		numOfBytesFromFileBeginning += strlen(newLineFromFile);
	}
	if (currInputArgs.validGrepParams[LOWER_C]) { printf("%d\n", numOfMatchingLines); }
	free(newLineFromFile);
}
