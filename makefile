MAKEFLAGS += --no-print-directory
# Variables for method of compilation and compilation flags
CC = @gcc
CFLAGS = -Wall -Werror -g

# Linking flags - LDFLAGS
LDFLAGS=
.PHONY: clean rebuild all test 

OBJS = MainGrepText.o ParseFuncArguments.o ParsePhraseToGrep.o
OBJS_TEST1 = ParsePhraseToGrep.o ParsePhraseToGrep_test.o
OBJS_TEST2 = ParseFuncArguments.o ParsePhraseToGrep.o ParseFuncArguments_test.o

TARGET = my_grep

# Receipies:
#all:
all : $(OBJS)
	@$(CC) $(OBJS) -o $(TARGET)
	
clean:
		@rm -f *$(OBJS)
		@rm -f $(TARGET)
		@rm -f $(OBJS_TEST1)
		@rm -f $(OBJS_TEST2)		
run: $(TARGET)
	./$(TARGET)

test:
	./run_all.sh

ParsePhraseToGrep_test: $(OBJS_TEST1)
	@$(CC) $(OBJS_TEST1) -o ParsePhraseToGrep_test

ParseFuncArguments_test: $(OBJS_TEST2)
	@$(CC) $(OBJS_TEST2) -o ParseFuncArguments_test
	
# .o files dependencies
MainGrepText.o: ParseFuncArguments.h
ParseFuncArguments.o: ParseFuncArguments.h ParsePhraseToGrep.h
ParsePhraseToGrep.o: ParsePhraseToGrep.h
