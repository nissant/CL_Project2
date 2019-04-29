#include "ParseFuncArguments.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct NumericalValuesForGrepRun {
  size_t numOfBytesFromFile;
  int lineNumber;
  int numOfMatchingLines;
  int numOfBytesFromFileBeginning;
  int extraLinesToPrint;
  int lastLineInBlock;
} NumericalValuesForGrepRun;

bool CompareStringsByFirstStrLength(const char* str1, const char* str2, bool useCaseSensitive)
{
  if (useCaseSensitive) {
    return (0 == strncasecmp(str1, str2, strlen(str1)));
  } else {
    return (0 == strncmp(str1, str2, strlen(str1)));
  }
}

bool IsPhraseInIndent(const PartialPhraseToGrep* phrasePart, char* lineFromFile, bool useCaseSensitive)
{
  if (phrasePart == NULL) {
    return true;
  }
  if (*lineFromFile == '\n' || *lineFromFile == '\0') {
    return false;
  }

  char* newLineFromFile = lineFromFile;
  switch (phrasePart->type) {
    case TWO_STR_OPTIONS:
      if (CompareStringsByFirstStrLength(phrasePart->firstStr.firstOptionToGrep, lineFromFile, useCaseSensitive) &&
          IsPhraseInIndent(phrasePart->next, lineFromFile + strlen(phrasePart->firstStr.firstOptionToGrep),
                           useCaseSensitive)) {
        return true;
      }
      if (CompareStringsByFirstStrLength(phrasePart->secondStr.secondOptionToGrep, lineFromFile, useCaseSensitive)) {
        return IsPhraseInIndent(phrasePart->next, lineFromFile + strlen(phrasePart->secondStr.secondOptionToGrep),
                                useCaseSensitive);
      }
      return false;
    case STRING:
      if (!CompareStringsByFirstStrLength(phrasePart->firstStr.phraseToGrep, lineFromFile, useCaseSensitive)) {
        return false;
      }
      newLineFromFile += strlen(phrasePart->firstStr.phraseToGrep);
      break;
    case CHAR_RANGE:
      if (*lineFromFile > phrasePart->secondStr.rangeHighLimit || *lineFromFile < phrasePart->firstStr.rangeLowLimit) {
        return false;
      }
    case EVERY_CHAR:
      newLineFromFile++;
  }
  return IsPhraseInIndent(phrasePart->next, newLineFromFile, useCaseSensitive);
}

char* GetEndOfLine(char* line)
{
  int lineLength = strlen(line);
  return (line + lineLength);
}

bool IsInputPhraseInLine(const GrepFuncArguments currInputArgs, char* lineFromFile)
{
  char* endOfLine = GetEndOfLine(lineFromFile);
  char* linePtr;

  for (linePtr = lineFromFile; linePtr < endOfLine; linePtr++) {
    if (IsPhraseInIndent(currInputArgs.linkedListPhrase, linePtr, currInputArgs.validGrepParams[LOWER_I])) {
      return true;
    }
  }
  return false;
}

bool InputPhraseIsTheLine(const GrepFuncArguments currInputArgs, char* lineFromFile)
{
  return IsPhraseInIndent(currInputArgs.linkedListPhrase, lineFromFile, currInputArgs.validGrepParams[LOWER_I]);
}

bool IsEmptyLine(char* line)
{
  char* endOfLine = GetEndOfLine(line);
  char* linePtr;

  for (linePtr = line; linePtr < endOfLine; linePtr++) {
    if (*linePtr != '\n' || *linePtr != ' ' || *linePtr != '\t') {
      return false;
    }
  }
  return true;
}

bool DoesLineMatchesRequests(char* lineFromFile, const GrepFuncArguments currInputArgs)
{
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
               const GrepFuncArguments currInputArgs)
{
  if (currInputArgs.validGrepParams[LOWER_N]) {
    printf("%d%c", lineNumber, separator);
  }
  if (currInputArgs.validGrepParams[LOWER_B]) {
    printf("%d%c", numOfBytesFromFileBeginning, separator);
  }
  printf("%s", lineFromFile);
}

#define NOT_VALID_LINE_NUMBER -1

void InitNumericalValuesForGrepRun(NumericalValuesForGrepRun* numericVals)
{
  numericVals->numOfBytesFromFile = 0;
  numericVals->lineNumber = 0;
  numericVals->numOfMatchingLines = 0;
  numericVals->numOfBytesFromFileBeginning = 0;
  numericVals->extraLinesToPrint = 0;
  numericVals->lastLineInBlock = NOT_VALID_LINE_NUMBER;
}

void CheckAndSetIfPrevBlockEnded(NumericalValuesForGrepRun* values)
{
  if ((values->lastLineInBlock != NOT_VALID_LINE_NUMBER) && (values->lastLineInBlock != (values->lineNumber - 1))) {
    printf("--\n");
  }
  values->lastLineInBlock = NOT_VALID_LINE_NUMBER;
}

void IncreaseLineNumberByOne(NumericalValuesForGrepRun* values) { values->lineNumber++; }

void IncreaseNumOfMatchingLinesByOne(NumericalValuesForGrepRun* values) { values->numOfMatchingLines++; }

void DecreaseExtraLinesToPrintByOne(NumericalValuesForGrepRun* values) { values->extraLinesToPrint--; }

void SetPossibleLastLineInBlock(NumericalValuesForGrepRun* values, int possibleLastLine)
{
  values->lastLineInBlock = possibleLastLine;
}

void AddToNumOfBytesFromFileBeginning(NumericalValuesForGrepRun* values, int numOfBytes)
{
  values->numOfBytesFromFileBeginning += numOfBytes;
}

void GoOverTextForGrepAndPrint(const GrepFuncArguments currInputArgs, bool isFileBasedGrep)
{
  char* newLineFromFile = NULL;
  NumericalValuesForGrepRun values;
  InitNumericalValuesForGrepRun(&values);

  while (getline(&newLineFromFile, &values.numOfBytesFromFile, isFileBasedGrep ? currInputArgs.fileToGrepPtr : stdin) !=
         -1) {
    IncreaseLineNumberByOne(&values);
    if (DoesLineMatchesRequests(newLineFromFile, currInputArgs)) {
      if (!currInputArgs.validGrepParams[LOWER_C]) {
        CheckAndSetIfPrevBlockEnded(&values);
        PrintLine(values.lineNumber, values.numOfBytesFromFileBeginning, newLineFromFile, ':', currInputArgs);
        values.extraLinesToPrint = currInputArgs.numOfExtraLinesToShow;
      }
      IncreaseNumOfMatchingLinesByOne(&values);
    } else if (values.extraLinesToPrint) {
      PrintLine(values.lineNumber, values.numOfBytesFromFileBeginning, newLineFromFile, '-', currInputArgs);
      DecreaseExtraLinesToPrintByOne(&values);
      if (values.extraLinesToPrint == 0) {
        SetPossibleLastLineInBlock(&values, values.lineNumber);
      }
    }
    AddToNumOfBytesFromFileBeginning(&values, (int)strlen(newLineFromFile));
  }
  if (currInputArgs.validGrepParams[LOWER_C]) {
    printf("%d\n", values.numOfMatchingLines);
  }
  free(newLineFromFile);
}

int main(int argc, char* argv[])
{
  GrepFuncArguments currInputArgs;

  bool isFileBasedGrep = IsFileBasedGrep(argv, argc);

  ParseArgumentsAndSetInStruct(argv, argc, &currInputArgs, isFileBasedGrep);

  GoOverTextForGrepAndPrint(currInputArgs, isFileBasedGrep);

  ReleaseArgsMemory(&currInputArgs);
  return 0;
}
