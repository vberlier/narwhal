
CC = gcc
OFLAGS = -g -Og
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer
CFLAGS = -Wall -Wextra -std=c11 $(OFLAGS) $(ASAN_FLAGS)

NARWHAL_DIST = ../../dist

INC_FLAGS = -I$(NARWHAL_DIST)


.PHONY: all clean

all: run_tests

clean:
	rm -f run_tests

run_tests: run_tests.c $(NARWHAL_DIST)/narwhal.c
	$(CC) $(CFLAGS) $(INC_FLAGS) $^ -o $@
