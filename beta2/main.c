#include <stdlib.h> 
#include <stdbool.h>
#include <stdio.h>
#include "ParseFuncArguments.h"
#include "GrepText.h"

int main(int argc, char *argv[]) {
	GrepFuncArguments currInputArgs;
	InitCurrentArgs(&currInputArgs);

	bool isFileBasedGrep = IsFileBasedGrep(argv[argc - 2]);

	ParseArgumentsAndSetInStruct(argv, argc, &currInputArgs, isFileBasedGrep);
	
	GoOverTextForGrepAndPrint(currInputArgs, isFileBasedGrep);

	ReleaseArgsMemory(&currInputArgs);
	return 1;
}



