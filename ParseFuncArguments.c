#include "ParseFuncArguments.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void InitCurrentArgs(GrepFuncArguments* currInputArgs)
{
  int argsIndex;
  currInputArgs->fileToGrepPtr = NULL;
  currInputArgs->numOfExtraLinesToShow = 0;
  currInputArgs->linkedListPhrase = NULL;
  for (argsIndex = 0; argsIndex < NUM_OF_GREP_POSSIBLES_PARAMS; argsIndex++) {
    currInputArgs->validGrepParams[argsIndex] = false;
  }
}

#define PARAM_LENGTH 2

bool IsParamOfGrep(const char* argument)
{
  if ((strlen(argument) == PARAM_LENGTH) && (*argument == '-')) {
    return true;
  }
  return false;
}

#define ASCII_FOR_ZERO '0'
#define ASCII_FOR_NINE '9'

bool IsANumber(const char* argument)
{
  int indexInStr = 0;
  while (indexInStr < (int)strlen(argument)) {
    if (argument[indexInStr] < ASCII_FOR_ZERO || argument[indexInStr] > ASCII_FOR_NINE) {
      return false;
    }
    indexInStr++;
  }
  return true;
}

#define TWO_ARGS 2
#define THREE_ARGS 3

bool IsFileBasedGrep(char* argv[], int argc)
{
  char* secondToLastArg = argv[argc - TWO_ARGS];
  char* thirdToLastArg = (argc > TWO_ARGS) ? argv[argc - THREE_ARGS] : NULL;

  if ((strcmp(secondToLastArg, "my_grep") == 0 || strcmp(secondToLastArg, "./my_grep") == 0) && (argc == TWO_ARGS)) {
    return false;
  }
  if (IsParamOfGrep(secondToLastArg)) {
    return false;
  }
  if ((argc > 2) && IsANumber(secondToLastArg) && (strcmp(thirdToLastArg, "-A") == 0)) {
    return false;
  }
  return true;
}

void SetFileToGrepFromCurrentArgs(const char* filePath, GrepFuncArguments* currInputArgs)
{
  currInputArgs->fileToGrepPtr = fopen(filePath, "r");
}

void SetValidGrepParamsFields(GrepFuncArguments* currInputArgs, char* mainArgs[], int numOfRegExpArgs)
{
  int paramIndex;
  for (paramIndex = 1; paramIndex <= numOfRegExpArgs; paramIndex++) {
    if (IsParamOfGrep(mainArgs[paramIndex])) {
      switch (*(mainArgs[paramIndex] + 1)) {
        case 'A':
          currInputArgs->validGrepParams[CAPITAL_A] = true;
          break;
        case 'b':
          currInputArgs->validGrepParams[LOWER_B] = true;
          break;
        case 'c':
          currInputArgs->validGrepParams[LOWER_C] = true;
          break;
        case 'i':
          currInputArgs->validGrepParams[LOWER_I] = true;
          break;
        case 'n':
          currInputArgs->validGrepParams[LOWER_N] = true;
          break;
        case 'v':
          currInputArgs->validGrepParams[LOWER_V] = true;
          break;
        case 'x':
          currInputArgs->validGrepParams[LOWER_X] = true;
          break;
        case 'E':
          currInputArgs->validGrepParams[CAPITAL_E] = true;
          break;
      }
    } else if (currInputArgs->validGrepParams[CAPITAL_A]) {
      currInputArgs->numOfExtraLinesToShow = atoi(mainArgs[paramIndex]);
    }
  }
}

#define ONE_ARG 1

void ParseArgumentsAndSetInStruct(char* argv[], int argc, GrepFuncArguments* currInputArgs, bool isFileBasedGrep)
{
  InitCurrentArgs(currInputArgs);

  if (isFileBasedGrep) {
    SetFileToGrepFromCurrentArgs(argv[argc - ONE_ARG], currInputArgs);
  }

  int numOfCmdParams = isFileBasedGrep ? argc - THREE_ARGS : argc - TWO_ARGS;
  SetValidGrepParamsFields(currInputArgs, argv, numOfCmdParams);

  char* phraseToGrep = isFileBasedGrep ? argv[argc - TWO_ARGS] : argv[argc - ONE_ARG];
  bool isRegExp = currInputArgs->validGrepParams[CAPITAL_E];
  ParsePhraseToGrepAndSetInLinkedList(phraseToGrep, &currInputArgs->linkedListPhrase, isRegExp);
}

void ReleaseArgsMemory(GrepFuncArguments* currInputArgs)
{
  if (currInputArgs->fileToGrepPtr != NULL) {
    fclose(currInputArgs->fileToGrepPtr);
  }
  FreePhrasePartsLinkedList(currInputArgs->linkedListPhrase);
}