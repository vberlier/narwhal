LIB_NAME := narwhal
TEST_NAME := run_tests

VERSION := $(strip $(shell cat VERSION))

SRC_DIR := src
TEST_DIR := test
BUILD_DIR := build
DIST_DIR := dist

LIB_FILE = lib$(LIB_NAME).so

BUILD_INCLUDE = $(BUILD_DIR)/include
BUILD_LIB = $(BUILD_DIR)/lib
BUILD_OBJ = $(BUILD_DIR)/obj
BUILD_AMALGAMATION = $(BUILD_DIR)/amalgamation

SHARED_LIB = $(BUILD_LIB)/$(LIB_FILE)
TEST_EXEC = $(BUILD_DIR)/$(TEST_NAME)

AMALGAMATED_SOURCE = $(BUILD_AMALGAMATION)/$(LIB_NAME).c
AMALGAMATED_HEADER = $(BUILD_AMALGAMATION)/$(LIB_NAME).h
AMALGAMATED_SOURCE_CONFIG = $(AMALGAMATED_SOURCE).json
AMALGAMATED_HEADER_CONFIG = $(AMALGAMATED_HEADER).json
AMALGAMATED_SOURCE_PROLOGUE = $(AMALGAMATED_SOURCE:%.c=%.prologue.c)
AMALGAMATED_HEADER_PROLOGUE = $(AMALGAMATED_HEADER:%.h=%.prologue.h)

CC ?= gcc
OFLAGS = -O3
CFLAGS = -Wall -Wextra -Wconversion -std=c11 -fPIC $(OFLAGS) $(ASAN_FLAGS)
DFLAGS = -D_XOPEN_SOURCE=700
LDFLAGS = -fuse-ld=gold

SRCS = $(shell find $(SRC_DIR) -name *.c | LC_ALL=C sort -z)
OBJS = $(SRCS:%.c=$(BUILD_OBJ)/%.o)
DEPS = $(OBJS:.o=.d)

TEST_SRCS = $(shell find $(TEST_DIR) -name *.c | LC_ALL=C sort -z)
TEST_OBJS = $(TEST_SRCS:%.c=$(BUILD_OBJ)/%.o)
TEST_DEPS = $(TEST_OBJS:.o=.d)

HEADERS = $(shell find $(SRC_DIR) -name *.h | LC_ALL=C sort -z)
SHARED_HEADERS = $(HEADERS:$(SRC_DIR)/%.h=$(BUILD_INCLUDE)/%.h)

INC_DIRS = $(SRC_DIR) $(TEST_DIR)
INC_FLAGS = $(addprefix -I,$(INC_DIRS))
CPPFLAGS = $(INC_FLAGS) $(DFLAGS) -MMD -MP


ifeq ($(DEBUG),1)
	OFLAGS = -g -Og
	ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer
	BUILD_DIR := $(BUILD_DIR)/debug
endif


DESTDIR ?= /usr
INSTALL_INCLUDE = $(DESTDIR)/include
INSTALL_LIB = $(DESTDIR)/lib


.PHONY: all install uninstall all_tests test format release clean

all: $(SHARED_LIB) $(SHARED_HEADERS) $(AMALGAMATED_SOURCE) $(AMALGAMATED_HEADER)

install: all uninstall
	install -d $(INSTALL_INCLUDE)
	cp -r $(BUILD_INCLUDE)/$(LIB_NAME) $(INSTALL_INCLUDE)
	install -t $(INSTALL_LIB) -D $(SHARED_LIB)

uninstall:
	rm -rf $(INSTALL_INCLUDE)/$(LIB_NAME)
	rm -f $(INSTALL_LIB)/$(LIB_FILE)

all_tests: $(TEST_EXEC)

test: all_tests
	@$(TEST_EXEC)

format:
	clang-format -i $(SRCS) $(HEADERS) $$(find $(TEST_DIR) examples -name *.c) $$(find $(TEST_DIR) examples -name *.h)

release:
	code -w VERSION
	[ "$$(printf "%s" "$$(< VERSION)")" != "$(VERSION)" ] && $(MAKE) || (echo "Aborting" && exit 1)
	rm -rf $(DIST_DIR)
	mkdir -p $(DIST_DIR)
	cp $(AMALGAMATED_SOURCE) $(AMALGAMATED_HEADER) $(DIST_DIR)
	git add .
	git commit -m "Version $$(printf "%s" "$$(< VERSION)")"
	git tag "v$$(printf "%s" "$$(< VERSION)")"
	git push origin master --tags

clean:
	rm -rf $(BUILD_DIR)


$(SHARED_LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -shared $(OBJS) -o $@

$(BUILD_OBJ)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_INCLUDE)/%.h: $(SRC_DIR)/%.h
	@mkdir -p $(dir $@)
	cp $< $@

$(TEST_EXEC): $(OBJS) $(TEST_OBJS)
	$(CC) $(LDFLAGS) $(ASAN_FLAGS) $(OBJS) $(TEST_OBJS) -o $@


-include $(DEPS) $(TEST_DEPS)


AMALGAMATE_PY = $(BUILD_AMALGAMATION)/amalgamate.py

$(AMALGAMATE_PY):
	@mkdir -p $(dir $@)
	curl https://raw.githubusercontent.com/edlund/amalgamate/master/amalgamate.py -o $@
	chmod +x $@

generate-config = echo '{"project":"$2","target":"$2","sources":["$3"],"include_paths":["$(SRC_DIR)"]}' | sed 's/ /","/g' > $1

generate-prologue = (echo "/*"; echo "Narwhal v$(VERSION) (https://github.com/vberlier/narwhal)"; echo "$2"; echo ""; echo "Generated with amalgamate.py (https://github.com/edlund/amalgamate)"; echo ""; cat $3; echo "*/"; echo "") > $1

$(AMALGAMATED_SOURCE_CONFIG): $(SRCS) $(HEADERS)
	$(call generate-config,$@,$(AMALGAMATED_SOURCE),$(SRCS))

$(AMALGAMATED_HEADER_CONFIG): $(HEADERS)
	$(call generate-config,$@,$(AMALGAMATED_HEADER),$(SRC_DIR)/$(LIB_NAME)/$(LIB_NAME).h)

$(AMALGAMATED_SOURCE_PROLOGUE): LICENSE VERSION
	$(call generate-prologue,$@,"Amalgamated source file",$<)
	echo "#define _XOPEN_SOURCE 700" >> $@
	echo "" >> $@

$(AMALGAMATED_HEADER_PROLOGUE): LICENSE VERSION
	$(call generate-prologue,$@,"Amalgamated header file",$<)

$(AMALGAMATED_SOURCE): $(AMALGAMATE_PY) $(AMALGAMATED_SOURCE_CONFIG) $(AMALGAMATED_SOURCE_PROLOGUE)
	$(AMALGAMATE_PY) -c $(AMALGAMATED_SOURCE_CONFIG) -s . -p $(AMALGAMATED_SOURCE_PROLOGUE)

$(AMALGAMATED_HEADER): $(AMALGAMATE_PY) $(AMALGAMATED_HEADER_CONFIG) $(AMALGAMATED_HEADER_PROLOGUE)
	$(AMALGAMATE_PY) -c $(AMALGAMATED_HEADER_CONFIG) -s . -p $(AMALGAMATED_HEADER_PROLOGUE)
