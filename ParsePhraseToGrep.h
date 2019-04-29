#ifndef __PARSE_PHRASE_TO_GREP_H
#define __PARSE_PHRASE_TO_GREP_H

#include <stdbool.h>

enum TypeOfPartialPhrase { STRING, EVERY_CHAR, TWO_STR_OPTIONS, CHAR_RANGE };

union FirstString {
  char* phraseToGrep;
  char* firstOptionToGrep;
  char rangeLowLimit;
};

union SecondString {
  char* secondOptionToGrep;
  char rangeHighLimit;
};

typedef struct PartialPhraseToGrep {
  enum TypeOfPartialPhrase type;
  union FirstString firstStr;
  union SecondString secondStr;
  struct PartialPhraseToGrep* next;
} PartialPhraseToGrep;

void ParsePhraseToGrepAndSetInLinkedList(char* Phrase, PartialPhraseToGrep** firstInPartsList, bool isRegExp);
void FreePhrasePartsLinkedList(PartialPhraseToGrep* phrasePartsList);
#endif