#include <stdlib.h> 
#include <stdbool.h>
#include <stdio.h>
#include "ParseFuncArguments.h"
#include "GrepText.h"

int main(int argc, char *argv[]) {
	GrepFuncArguments currInputArgs;
	InitCurrentArgs(&currInputArgs);

	bool isFileBasedGrep = IsFileBasedGrep(argv[argc - 2]);

	SetFileToGrepFromCurrentArgs(argv[argc - 1], &currInputArgs);

	char* wordToGrep = isFileBasedGrep ? argv[argc - 2] : argv[argc - 1];
	SetWordToGrepFromCurrentArgs(wordToGrep, &currInputArgs);

	GoOverTextForGrep(currInputArgs, isFileBasedGrep);

	ReleaseArgsMemory(&currInputArgs);
	return 0;
}



