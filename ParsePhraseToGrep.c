#include "ParsePhraseToGrep.h"
#include <stdlib.h>
#include <string.h>

bool IsRegExpressionSymbol(char symbol)
{
  return (symbol == '.') || (symbol == '\\') || (symbol == '|') || (symbol == '[') || (symbol == ']') ||
         (symbol == '(') || (symbol == ')') || (symbol == '{') || (symbol == '}');
}

bool IsCloseBrackets(char symbol) { return (symbol == ')') || (symbol == ']'); }

void RemoveBackslashBeforeRegExpSymbols(char* phrase)
{
  char* tempPhrase = (char*)malloc((strlen(phrase) + 1) * sizeof(char));
  char* tempPhraseStart = tempPhrase;
  char* currChar = phrase;
  int phraseLen = strlen(phrase);

  while ((tempPhrase - tempPhraseStart) < phraseLen) {
    if ((*currChar == '\\') && IsRegExpressionSymbol(*(currChar + 1))) {
      currChar++;
      phraseLen--;
    }
    *tempPhrase = *currChar;
    tempPhrase++;
    currChar++;
  }
  strncpy(phrase, tempPhraseStart, phraseLen);
  *(phrase + phraseLen) = '\0';
  free(tempPhraseStart);
}

#define BACKSLASH_COUPLE_LEN 2

char* GetStartOfNextPhrasePartIfExists(char* startOfCurrPhrasePart)
{
  char* currChar = startOfCurrPhrasePart;
  currChar = (*currChar == '\\') ? currChar + BACKSLASH_COUPLE_LEN : currChar + 1;

  while (strlen(currChar) != 0) {
    if (*currChar == '\\') {
      currChar += BACKSLASH_COUPLE_LEN;
    }
    if (IsCloseBrackets(*currChar)) {
      return (strlen(currChar) == 1) ? NULL : currChar + 1;
    } else if (IsRegExpressionSymbol(*currChar) && (*currChar != '|')) {
      return currChar;
    } else {
      currChar++;
    }
  }
  return NULL;
}

void SetStringAndRemoveBackSlash(char** string, const char* phrase, int lenOfPhrase)
{
  *string = (char*)malloc((lenOfPhrase + 1) * sizeof(char));
  strncpy(*string, phrase, lenOfPhrase);
  *(*string + lenOfPhrase) = '\0';
  RemoveBackslashBeforeRegExpSymbols(*string);
}

#define FIRST_BRACKET_INDENT 1
#define REDUNDANT_CHARS 3

void SetPartialPhraseInCaseOfRoundBrackets(PartialPhraseToGrep* part, const char* currentPhrase, int lenOfCurrPhrase)
{
  part->type = TWO_STR_OPTIONS;

  int lenOffirstStr = strchr(currentPhrase, '|') - (currentPhrase + FIRST_BRACKET_INDENT);
  int lenOfsecondStr = lenOfCurrPhrase - (lenOffirstStr + REDUNDANT_CHARS);

  SetStringAndRemoveBackSlash(&part->firstStr.firstOptionToGrep, currentPhrase + FIRST_BRACKET_INDENT, lenOffirstStr);
  SetStringAndRemoveBackSlash(&part->secondStr.secondOptionToGrep, currentPhrase + lenOffirstStr + 2, lenOfsecondStr);
}

#define HIGH_LIMIT_INDENTATION 3

void SetPartialPhraseInCaseOfSquareBrackets(PartialPhraseToGrep* part, const char* currentPhrase)
{
  part->type = CHAR_RANGE;

  part->firstStr.rangeLowLimit = *(currentPhrase + FIRST_BRACKET_INDENT);
  part->secondStr.rangeHighLimit = *(currentPhrase + HIGH_LIMIT_INDENTATION);
}

void SetPartialPhraseInCaseOfAString(PartialPhraseToGrep* part, const char* currentPhrase, int lenOfCurrPhrase)
{
  part->type = STRING;
  SetStringAndRemoveBackSlash(&part->firstStr.phraseToGrep, currentPhrase, lenOfCurrPhrase);
}

bool FirstCharIsDot(const char* phrasePart) { return (*phrasePart == '.'); }

bool FirstCharIsRoundBrackets(const char* phrasePart) { return (*phrasePart == '('); }

bool FirstCharIsSquareBrackets(const char* phrasePart) { return (*phrasePart == '['); }

void ParsePhraseToGrepAndSetInLinkedList(char* Phrase, PartialPhraseToGrep** firstInPartsList, bool isRegExp)
{
  char* currentPhrasePart = Phrase;
  char* nextPhrasePart = Phrase;

  *firstInPartsList = (PartialPhraseToGrep*)malloc(sizeof(PartialPhraseToGrep));
  PartialPhraseToGrep* currPartInLinkedList = *firstInPartsList;

  while (currentPhrasePart != NULL) {
    nextPhrasePart = (isRegExp) ? GetStartOfNextPhrasePartIfExists(currentPhrasePart) : NULL;
    int phraseLen =
        (nextPhrasePart != NULL) ? (unsigned int)(nextPhrasePart - currentPhrasePart) : strlen(currentPhrasePart);

    if (isRegExp && FirstCharIsDot(currentPhrasePart)) {
      currPartInLinkedList->type = EVERY_CHAR;
    } else if (isRegExp && FirstCharIsRoundBrackets(currentPhrasePart)) {
      SetPartialPhraseInCaseOfRoundBrackets(currPartInLinkedList, currentPhrasePart, phraseLen);
    } else if (isRegExp && FirstCharIsSquareBrackets(currentPhrasePart)) {
      SetPartialPhraseInCaseOfSquareBrackets(currPartInLinkedList, currentPhrasePart);
    } else {
      SetPartialPhraseInCaseOfAString(currPartInLinkedList, currentPhrasePart, phraseLen);
    }

    currPartInLinkedList->next =
        (nextPhrasePart != NULL) ? (PartialPhraseToGrep*)malloc(sizeof(PartialPhraseToGrep)) : NULL;
    currPartInLinkedList = currPartInLinkedList->next;
    currentPhrasePart = nextPhrasePart;
  }
}

void FreePhrasePartsLinkedList(PartialPhraseToGrep* phrasePartsList)
{
  while (phrasePartsList) {
    PartialPhraseToGrep* nextPart = phrasePartsList->next;
    switch (phrasePartsList->type) {
      case TWO_STR_OPTIONS:
        free(phrasePartsList->firstStr.firstOptionToGrep);
        free(phrasePartsList->secondStr.secondOptionToGrep);
        break;
      case STRING:
        free(phrasePartsList->firstStr.phraseToGrep);
        break;
      default:
        break;
    }
    free(phrasePartsList);
    phrasePartsList = nextPart;
  }
}