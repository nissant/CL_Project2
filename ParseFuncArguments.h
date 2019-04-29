#ifndef __PARSE_FUNC_ARGUMENTS_H
#define __PARSE_FUNC_ARGUMENTS_H

#include "ParsePhraseToGrep.h"
#include <stdbool.h>
#include <stdio.h>

#define NUM_OF_GREP_POSSIBLES_PARAMS 8

typedef struct GrepFuncArguments {
  FILE* fileToGrepPtr;
  char* phraseToGrep;
  PartialPhraseToGrep* linkedListPhrase;
  int phraseToGrepLength;
  int numOfExtraLinesToShow;
  bool validGrepParams[NUM_OF_GREP_POSSIBLES_PARAMS];
} GrepFuncArguments;

enum GrepParams { CAPITAL_A = 0, LOWER_B, LOWER_C, LOWER_I, LOWER_N, LOWER_V, LOWER_X, CAPITAL_E };

bool IsFileBasedGrep(char* argv[], int argc);
void ParseArgumentsAndSetInStruct(char* argv[], int argc, GrepFuncArguments* currInputArgs, bool isFileBasedGrep);
void ReleaseArgsMemory(GrepFuncArguments* currInputArgs);
#endif