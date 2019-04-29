#include "ParseFuncArguments.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

void InitGrepArgs(GrepFuncArguments* currInputArgs)
{
  int argsIndex;
  currInputArgs->fileToGrepPtr = NULL;
  currInputArgs->numOfExtraLinesToShow = 0;
  currInputArgs->linkedListPhrase = NULL;
  for (argsIndex = 0; argsIndex < NUM_OF_GREP_POSSIBLES_PARAMS; argsIndex++) {
    currInputArgs->validGrepParams[argsIndex] = false;
  }
}

bool IsRecievedArgsAsExpected(const GrepFuncArguments expected, const GrepFuncArguments recieved,
                              bool isFileBasedGrepRecieved, bool isFileBasedGrepExpected)
{
  int argsIndex;
  if (isFileBasedGrepExpected != isFileBasedGrepRecieved) {
    return false;
  }
  if (isFileBasedGrepExpected && (recieved.fileToGrepPtr == NULL)) {
    return false;
  }
  if (expected.numOfExtraLinesToShow != recieved.numOfExtraLinesToShow) {
    return false;
  }
  if (expected.linkedListPhrase->type != recieved.linkedListPhrase->type) {
    return false;
  }
  if (strcmp(expected.linkedListPhrase->firstStr.phraseToGrep, recieved.linkedListPhrase->firstStr.phraseToGrep) != 0) {
    return false;
  }
  for (argsIndex = 0; argsIndex < NUM_OF_GREP_POSSIBLES_PARAMS; argsIndex++) {
    if (expected.validGrepParams[argsIndex] != recieved.validGrepParams[argsIndex]) {
      return false;
    }
  }
  return true;
}

#define NUM_OF_ARGS_FOR_FIRST_TEST 3

static void TestNoArgumentButPhraseAndFile()
{
  char* argvToTest[] = {"./my_grep", "wordToGrep", "ForTheTests/FILE"};
  GrepFuncArguments recievedFromFuncGrepArgs;

  GrepFuncArguments expectedGrepArgs;
  InitGrepArgs(&expectedGrepArgs);
  PartialPhraseToGrep expectedPartialPhrase;
  expectedPartialPhrase.type = STRING;
  expectedPartialPhrase.firstStr.phraseToGrep = "wordToGrep\0";
  expectedGrepArgs.linkedListPhrase = &expectedPartialPhrase;

  ParseArgumentsAndSetInStruct(argvToTest, NUM_OF_ARGS_FOR_FIRST_TEST, &recievedFromFuncGrepArgs, true);
  bool isFileBasedGrepRecieved = IsFileBasedGrep(argvToTest, NUM_OF_ARGS_FOR_FIRST_TEST);

  assert(IsRecievedArgsAsExpected(expectedGrepArgs, recievedFromFuncGrepArgs, isFileBasedGrepRecieved, true) &&
         "TestNoArgumentButPhraseAndFile()");

  ReleaseArgsMemory(&recievedFromFuncGrepArgs);
}

#define NUM_OF_ARGS_FOR_SECOND_TEST 4

static void TestCapitalAArgumentNoFile()
{
  char* argvToTest[] = {"./my_grep", "-A", "2", "wordToGrep"};
  GrepFuncArguments recievedFromFuncGrepArgs;

  GrepFuncArguments expectedGrepArgs;
  InitGrepArgs(&expectedGrepArgs);
  PartialPhraseToGrep expectedPartialPhrase;
  expectedPartialPhrase.type = STRING;
  expectedPartialPhrase.firstStr.phraseToGrep = "wordToGrep\0";
  expectedGrepArgs.linkedListPhrase = &expectedPartialPhrase;
  expectedGrepArgs.validGrepParams[CAPITAL_A] = true;
  expectedGrepArgs.numOfExtraLinesToShow = 2;

  ParseArgumentsAndSetInStruct(argvToTest, NUM_OF_ARGS_FOR_SECOND_TEST, &recievedFromFuncGrepArgs, false);
  bool isFileBasedGrepRecieved = IsFileBasedGrep(argvToTest, NUM_OF_ARGS_FOR_SECOND_TEST);

  assert(IsRecievedArgsAsExpected(expectedGrepArgs, recievedFromFuncGrepArgs, isFileBasedGrepRecieved, false) &&
         "TestCapitalAArgumentNoFile()");
}

#define NUM_OF_ARGS_FOR_THIRD_TEST 7

static void TestMultipleArguments()
{
  char* argvToTest[] = {"./my_grep", "-A", "2", "-n", "-x", "wordToGrep", "ForTheTests/FILE"};
  GrepFuncArguments recievedFromFuncGrepArgs;

  GrepFuncArguments expectedGrepArgs;
  InitGrepArgs(&expectedGrepArgs);
  PartialPhraseToGrep expectedPartialPhrase;
  expectedPartialPhrase.type = STRING;
  expectedPartialPhrase.firstStr.phraseToGrep = "wordToGrep\0";
  expectedGrepArgs.linkedListPhrase = &expectedPartialPhrase;
  expectedGrepArgs.validGrepParams[CAPITAL_A] = true;
  expectedGrepArgs.numOfExtraLinesToShow = 2;
  expectedGrepArgs.validGrepParams[LOWER_N] = true;
  expectedGrepArgs.validGrepParams[LOWER_X] = true;

  ParseArgumentsAndSetInStruct(argvToTest, NUM_OF_ARGS_FOR_THIRD_TEST, &recievedFromFuncGrepArgs, true);
  bool isFileBasedGrepRecieved = IsFileBasedGrep(argvToTest, NUM_OF_ARGS_FOR_THIRD_TEST);

  assert(IsRecievedArgsAsExpected(expectedGrepArgs, recievedFromFuncGrepArgs, isFileBasedGrepRecieved, true) &&
         "TestMultipleArguments()");

  ReleaseArgsMemory(&recievedFromFuncGrepArgs);
}

int main()
{
  TestNoArgumentButPhraseAndFile();
  TestCapitalAArgumentNoFile();
  TestMultipleArguments();
  return 0;
}