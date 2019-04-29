#include <stdlib.h> 
#include <stdbool.h>
#include <string.h>
#include "ParseFuncArguments.h"

void InitCurrentArgs(GrepFuncArguments* currInputArgs) {
	currInputArgs->fileToGrepPtr = NULL;
	currInputArgs->phraseToGrep = NULL;
	currInputArgs->numOfExtraLinesToShow = 0;
	int index;
	for (index = 0; index < NUM_OF_GREP_POSSIBLES_PARAMS; index++) {
		currInputArgs->validGrepParams[index] = false;
	}
}

#define REG_EXPRESSION_ARG_LENGTH 2

bool IsParamOfGrep(const char* argument) {
	if ((strlen(argument) == REG_EXPRESSION_ARG_LENGTH) && (*argument == '-')) { return true; }
	return false;
}

bool IsFileBasedGrep(const char* secondToLastMainArg) {
	if (strcmp(secondToLastMainArg, "my_grep") == 0)   { return false; }
	if (strcmp(secondToLastMainArg, "./my_grep") == 0) { return false; }
	if (IsParamOfGrep(secondToLastMainArg))            { return false; }
	return true;
}

bool SetFileToGrepFromCurrentArgs(const char* filePath, GrepFuncArguments* currInputArgs) {
	currInputArgs->fileToGrepPtr = fopen(filePath, "r");
	return currInputArgs->fileToGrepPtr == NULL ? false : true;
}

bool IsRegExpressionSymbol(char symbol) {
	return (symbol == '.') || (symbol == '\\') || (symbol == '|') || (symbol == '[') || (symbol == ']')
		|| (symbol == '(') || (symbol == ')')  || (symbol == '{') || (symbol == '}');
}

void RemoveBackslashBeforeRegExpSymbols(GrepFuncArguments* currInputArgs) {
	char* tempPhrase = (char*)malloc((currInputArgs->phraseToGrepLength + 1) * sizeof(char));
	char* tempPhraseStart = tempPhrase;
	char* currChar = currInputArgs->phraseToGrep;

	while ((tempPhrase - tempPhraseStart) < (currInputArgs->phraseToGrepLength)) {
		if ((*currChar == '\\') && IsRegExpressionSymbol(*(currChar + 1))) {
			currChar++;
			currInputArgs->phraseToGrepLength--;
		}
		*tempPhrase = *currChar;
		tempPhrase++;
		currChar++;
	}
	strncpy(currInputArgs->phraseToGrep, tempPhraseStart, currInputArgs->phraseToGrepLength);
	free(tempPhraseStart);
}

void SetPhraseToGrepFromCurrentArgs(const char* Phrase, GrepFuncArguments* currInputArgs) {
	int lenOfPhraseToGrep = strlen(Phrase);
	
	currInputArgs->phraseToGrepLength = lenOfPhraseToGrep;
	currInputArgs->phraseToGrep = (char*)malloc((lenOfPhraseToGrep + 1) * sizeof(char));
	
	strcpy(currInputArgs->phraseToGrep, Phrase);

	RemoveBackslashBeforeRegExpSymbols(currInputArgs);
}

void SetValidGrepParamsFields(GrepFuncArguments* currInputArgs, char *mainArgs[], int numOfRegExpArgs) {
	int index;
	for (index = 1; index <= numOfRegExpArgs; index++) {
		if (IsParamOfGrep(mainArgs[index])) {
			switch (*(mainArgs[index] + 1)) {
			case 'A': currInputArgs->validGrepParams[CAPITAL_A] = true; break;
			case 'b': currInputArgs->validGrepParams[LOWER_B] = true; break;
			case 'c': currInputArgs->validGrepParams[LOWER_C] = true; break;
			case 'i': currInputArgs->validGrepParams[LOWER_I] = true; break;
			case 'n': currInputArgs->validGrepParams[LOWER_N] = true; break;
			case 'v': currInputArgs->validGrepParams[LOWER_V] = true; break;
			case 'x': currInputArgs->validGrepParams[LOWER_X] = true; break;
			case 'E': currInputArgs->validGrepParams[CAPITAL_E] = true; break;
			}
		} else if (currInputArgs->validGrepParams[CAPITAL_A]) {
			currInputArgs->numOfExtraLinesToShow = atoi(mainArgs[index]);
		}
	}
}

void ParseArgumentsAndSetInStruct(char *argv[], int argc, GrepFuncArguments* currInputArgs,
	                              bool isFileBasedGrep) {
	SetFileToGrepFromCurrentArgs(argv[argc - 1], currInputArgs);
	
	char* phraseToGrep = isFileBasedGrep ? argv[argc - 2] : argv[argc - 1];
	SetPhraseToGrepFromCurrentArgs(phraseToGrep, currInputArgs);

	int numOfCmdParams = isFileBasedGrep ? argc - 3 : argc - 2;
	SetValidGrepParamsFields(currInputArgs, argv, numOfCmdParams);
}

void ReleaseArgsMemory(GrepFuncArguments* currInputArgs) {
	if (currInputArgs->fileToGrepPtr != NULL) { fclose(currInputArgs->fileToGrepPtr); }
	free(currInputArgs->phraseToGrep);
}