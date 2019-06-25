ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer
CFLAGS = -Wall -Wextra -std=c11 -g -Og $(ASAN_FLAGS)

NARWHAL_DIST = ../../dist

INC_FLAGS = -I$(NARWHAL_DIST)

.PHONY: test clean

test: run_tests
	./$<

clean:
	rm -f run_tests

run_tests: main.c $(NARWHAL_DIST)/narwhal.c
	gcc $(CFLAGS) $(INC_FLAGS) $^ -o $@
