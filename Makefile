
LIB_NAME := unicorn

SRC_DIR := src
BUILD_DIR := build

LIB_FILE = lib$(LIB_NAME).so

BUILD_INCLUDE = $(BUILD_DIR)/include
BUILD_LIB = $(BUILD_DIR)/lib
BUILD_OBJ = $(BUILD_DIR)/obj
SHARED_LIB = $(BUILD_LIB)/$(LIB_FILE)

CC = gcc
OFLAGS = -O3
CFLAGS = -Wall -Wextra -std=c11 -fPIC $(OFLAGS) $(ASAN_FLAGS)
DFLAGS = -D_XOPEN_SOURCE=700

SRCS = $(shell find $(SRC_DIR) -name *.c | LC_ALL=C sort -z)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_OBJ)/%.o)
DEPS = $(OBJS:.o=.d)

HEADERS = $(shell find $(SRC_DIR) -name *.h | LC_ALL=C sort -z)
SHARED_HEADERS = $(HEADERS:$(SRC_DIR)/%.h=$(BUILD_INCLUDE)/%.h)

INC_DIRS = $(SRC_DIR)
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


.PHONY: all install uninstall clean

all: $(SHARED_LIB) $(SHARED_HEADERS)

install: all
	install -d $(INSTALL_INCLUDE)
	cp -r $(BUILD_INCLUDE)/$(LIB_NAME) $(INSTALL_INCLUDE)
	install -t $(INSTALL_LIB) -D $(SHARED_LIB)

uninstall:
	rm -rf $(INSTALL_INCLUDE)/$(LIB_NAME)
	rm -f $(INSTALL_LIB)/$(LIB_FILE)

clean:
	rm -rf $(BUILD_DIR)


$(SHARED_LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -shared $(OBJS) -o $@

$(BUILD_OBJ)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_INCLUDE)/%.h: $(SRC_DIR)/%.h
	@mkdir -p $(dir $@)
	cp $< $@


-include $(DEPS)
