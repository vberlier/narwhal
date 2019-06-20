
CC = gcc
OFLAGS = -g -Og
DFLAGS = -D_XOPEN_SOURCE=700
ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer
CFLAGS = -Wall -Wextra -std=c11 $(DFLAGS) $(OFLAGS) $(ASAN_FLAGS)

LIB_FLAGS = -lnarwhal


.PHONY: all clean

all: run_tests

clean:
	rm -f run_tests

run_tests: run_tests.c tmpdir.c
	$(CC) $(CFLAGS) $(LIB_FLAGS) $^ -o $@
