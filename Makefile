CC ?= gcc
PKG_CONFIG ?= pkg-config
CSTD ?= c11
CFLAGS ?= -g
CPPFLAGS ?=
LDFLAGS ?=

WARNINGS := -Wall -Wextra -Wpedantic
COMMON_CFLAGS := -std=$(CSTD) $(WARNINGS)

CONSOLE_BIN := chain
RAYLIB_BIN := raylib

CONSOLE_SRCS := main.c game.c
CONSOLE_OBJS := $(CONSOLE_SRCS:.c=.o)
RAYLIB_SRCS := raylib.c
RAYLIB_OBJS := $(RAYLIB_SRCS:.c=.o)

UNAME_S := $(shell uname -s 2>/dev/null)
RAYLIB_CFLAGS := $(shell $(PKG_CONFIG) --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell $(PKG_CONFIG) --libs raylib 2>/dev/null)

ifeq ($(strip $(RAYLIB_LIBS)),)
ifeq ($(OS),Windows_NT)
RAYLIB_LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
else ifeq ($(UNAME_S),Darwin)
RAYLIB_LIBS := -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else ifeq ($(UNAME_S),Linux)
RAYLIB_LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
else
RAYLIB_LIBS := -lraylib
endif
endif

.PHONY: all clean clang

all: $(CONSOLE_BIN) $(RAYLIB_BIN)

$(CONSOLE_BIN): $(CONSOLE_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(RAYLIB_BIN): $(RAYLIB_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(RAYLIB_LIBS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(COMMON_CFLAGS) $(CFLAGS) -c $< -o $@

main.o game.o: game.h
raylib.o: CPPFLAGS += $(RAYLIB_CFLAGS)

clang:
	$(MAKE) clean
	$(MAKE) CC=clang all

clean:
	rm -f $(CONSOLE_OBJS) $(RAYLIB_OBJS) $(CONSOLE_BIN) $(RAYLIB_BIN)
