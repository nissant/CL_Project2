#include "ParsePhraseToGrep.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

bool IsTwoPartialPhrasesIdentical(const PartialPhraseToGrep partialPhrase1, const PartialPhraseToGrep partialPhrase2)
{
  if (partialPhrase1.type != partialPhrase2.type) {
    return false;
  }
  switch (partialPhrase1.type) {
    case STRING:
      return (strcmp(partialPhrase1.firstStr.phraseToGrep, partialPhrase2.firstStr.phraseToGrep) == 0);
    case TWO_STR_OPTIONS:
      return ((strcmp(partialPhrase1.firstStr.firstOptionToGrep, partialPhrase2.firstStr.firstOptionToGrep) == 0) &&
              (strcmp(partialPhrase1.secondStr.secondOptionToGrep, partialPhrase2.secondStr.secondOptionToGrep) == 0));
    case CHAR_RANGE:
      return ((partialPhrase1.firstStr.rangeLowLimit == partialPhrase2.firstStr.rangeLowLimit) &&
              (partialPhrase1.secondStr.rangeHighLimit == partialPhrase2.secondStr.rangeHighLimit));
    default:
      return true;
  }
}

bool IsTwoPartialPhrasesLinkedListsIdentical(PartialPhraseToGrep* partialPhrase1, PartialPhraseToGrep* partialPhrase2)
{
  PartialPhraseToGrep* currPartialPhrase1 = partialPhrase1;
  PartialPhraseToGrep* currPartialPhrase2 = partialPhrase2;
  while (currPartialPhrase1 != NULL && currPartialPhrase2 != NULL) {
    if (!IsTwoPartialPhrasesIdentical(*currPartialPhrase1, *currPartialPhrase2)) {
      return false;
    }
    currPartialPhrase1 = currPartialPhrase1->next;
    currPartialPhrase2 = currPartialPhrase2->next;
  }
  if (currPartialPhrase1 == NULL && currPartialPhrase2 == NULL) {
    return true;
  } else {
    return false;
  }
}

void SetExpectedPartialPhraseString(PartialPhraseToGrep* partialPhrase, char* str, PartialPhraseToGrep* next)
{
  partialPhrase->type = STRING;
  partialPhrase->firstStr.phraseToGrep = str;
  partialPhrase->next = next;
}

void SetExpectedPartialPhraseRoundBrackets(PartialPhraseToGrep* partialPhrase, char* str1, char* str2,
                                           PartialPhraseToGrep* next)
{
  partialPhrase->type = TWO_STR_OPTIONS;
  partialPhrase->firstStr.firstOptionToGrep = str1;
  partialPhrase->secondStr.secondOptionToGrep = str2;
  partialPhrase->next = next;
}

void SetExpectedPartialPhraseSquareBrackets(PartialPhraseToGrep* partialPhrase, char lowLimit, char HighLimit,
                                            PartialPhraseToGrep* next)
{
  partialPhrase->type = CHAR_RANGE;
  partialPhrase->firstStr.rangeLowLimit = lowLimit;
  partialPhrase->secondStr.rangeHighLimit = HighLimit;
  partialPhrase->next = next;
}

static void TestOnlyString()
{
  char* stringToTest = "ProjectByNissanAndAdi";
  PartialPhraseToGrep* recievedFromFuncPartialPhrase;

  PartialPhraseToGrep expectedPartialPhrase;
  SetExpectedPartialPhraseString(&expectedPartialPhrase, "ProjectByNissanAndAdi\0", NULL);

  ParsePhraseToGrepAndSetInLinkedList(stringToTest, &recievedFromFuncPartialPhrase, false);

  assert(IsTwoPartialPhrasesLinkedListsIdentical(recievedFromFuncPartialPhrase, &expectedPartialPhrase) &&
         "TestOnlyString()");

  FreePhrasePartsLinkedList(recievedFromFuncPartialPhrase);
}

static void TestOnlyStringBackslashed()
{
  char* stringToTest = "ProjectBy\\.NissanAndAdi";
  PartialPhraseToGrep* recievedFromFuncPartialPhrase;

  PartialPhraseToGrep expectedPartialPhrase;
  SetExpectedPartialPhraseString(&expectedPartialPhrase, "ProjectBy.NissanAndAdi\0", NULL);

  ParsePhraseToGrepAndSetInLinkedList(stringToTest, &recievedFromFuncPartialPhrase, false);

  assert(IsTwoPartialPhrasesLinkedListsIdentical(recievedFromFuncPartialPhrase, &expectedPartialPhrase) &&
         "TestOnlyStringBackslashed()");

  FreePhrasePartsLinkedList(recievedFromFuncPartialPhrase);
}

static void TestRegExpDot()
{
  char* stringToTest = ".";
  PartialPhraseToGrep* recievedFromFuncPartialPhrase;

  PartialPhraseToGrep expectedPartialPhrase;
  expectedPartialPhrase.type = EVERY_CHAR;
  expectedPartialPhrase.next = NULL;

  ParsePhraseToGrepAndSetInLinkedList(stringToTest, &recievedFromFuncPartialPhrase, true);

  assert(IsTwoPartialPhrasesLinkedListsIdentical(recievedFromFuncPartialPhrase, &expectedPartialPhrase) &&
         "TestRegExpDot()");

  FreePhrasePartsLinkedList(recievedFromFuncPartialPhrase);
}

static void TestRegExpRoundBrackets()
{
  char* stringToTest = "(Project|2)";
  PartialPhraseToGrep* recievedFromFuncPartialPhrase;

  PartialPhraseToGrep expectedPartialPhrase;
  SetExpectedPartialPhraseRoundBrackets(&expectedPartialPhrase, "Project\0", "2\0", NULL);

  ParsePhraseToGrepAndSetInLinkedList(stringToTest, &recievedFromFuncPartialPhrase, true);

  assert(IsTwoPartialPhrasesLinkedListsIdentical(recievedFromFuncPartialPhrase, &expectedPartialPhrase) &&
         "TestRegExpRoundBrackets()");

  FreePhrasePartsLinkedList(recievedFromFuncPartialPhrase);
}

static void TestRegExpSquareBrackets()
{
  char* stringToTest = "[a-z]";
  PartialPhraseToGrep* recievedFromFuncPartialPhrase;

  PartialPhraseToGrep expectedPartialPhrase;
  SetExpectedPartialPhraseSquareBrackets(&expectedPartialPhrase, 'a', 'z', NULL);

  ParsePhraseToGrepAndSetInLinkedList(stringToTest, &recievedFromFuncPartialPhrase, true);

  assert(IsTwoPartialPhrasesLinkedListsIdentical(recievedFromFuncPartialPhrase, &expectedPartialPhrase) &&
         "TestRegExpSquareBrackets()");

  FreePhrasePartsLinkedList(recievedFromFuncPartialPhrase);
}

static void TestRegExpMultiplePhraseParts()
{
  char* stringToTest = "Project2(Project|2)[a-z].";
  PartialPhraseToGrep* recievedFromFuncPartialPhrase;

  PartialPhraseToGrep expectedPartialPhraseStr;
  PartialPhraseToGrep expectedPartialPhraseRound;
  PartialPhraseToGrep expectedPartialPhraseSquare;
  PartialPhraseToGrep expectedPartialPhraseDot;
  SetExpectedPartialPhraseString(&expectedPartialPhraseStr, "Project2\0", &expectedPartialPhraseRound);
  SetExpectedPartialPhraseRoundBrackets(&expectedPartialPhraseRound, "Project\0", "2\0", &expectedPartialPhraseSquare);
  SetExpectedPartialPhraseSquareBrackets(&expectedPartialPhraseSquare, 'a', 'z', &expectedPartialPhraseDot);
  expectedPartialPhraseDot.type = EVERY_CHAR;
  expectedPartialPhraseDot.next = NULL;

  ParsePhraseToGrepAndSetInLinkedList(stringToTest, &recievedFromFuncPartialPhrase, true);

  assert(IsTwoPartialPhrasesLinkedListsIdentical(recievedFromFuncPartialPhrase, &expectedPartialPhraseStr) &&
         "TestRegExpMultiplePhraseParts()");

  FreePhrasePartsLinkedList(recievedFromFuncPartialPhrase);
}

int main()
{
  TestOnlyString();
  TestOnlyStringBackslashed();
  TestRegExpDot();
  TestRegExpRoundBrackets();
  TestRegExpSquareBrackets();
  TestRegExpMultiplePhraseParts();
  return 0;
}