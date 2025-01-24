# Compiler settings
CC=gcc
CC_WIN=x86_64-w64-mingw32-gcc
CFLAGS=-Wall -Wextra -Werror -g -O0
INCLUDES=-Iinclude

# Platform-specific settings
LDFLAGS_LINUX=-lSDL2
LDFLAGS_WIN=-lmingw32 -lSDL2main -lSDL2

# Directories
SOURCEDIR=src
BINDIR=bin
OBJDIR=obj
TARGET_LINUX=$(BINDIR)/main
TARGET_WIN=$(BINDIR)/main.exe

# Objects
SRCS=$(wildcard $(SOURCEDIR)/*.c)
OBJS=$(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS_WIN=$(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%_win.o,$(SRCS))

# Default target
linux: $(TARGET_LINUX)

release: CFLAGS=-Wall -O2
release: linux windows
	strip $(TARGET_LINUX)
	strip $(TARGET_WIN)

# Windows target
windows: $(TARGET_WIN)

static: LDFLAGS_WIN=-lmingw32 -Wl,-Bstatic -lSDL2main -lSDL2 -Wl,-Bdynamic -lversion -lwinmm -lgdi32 -lsetupapi -limm32 -lole32 -loleaut32
static: windows
	rm -f $(BINDIR)/SDL2.dll

$(TARGET_LINUX): $(OBJS) | $(BINDIR)
	$(CC) -o $@ $^ $(LDFLAGS_LINUX)

$(TARGET_WIN): $(OBJS_WIN) | $(BINDIR) $(BINDIR)/SDL2.dll
	$(CC_WIN) -o $@ $^ $(LDFLAGS_WIN)

$(BINDIR)/SDL2.dll: /usr/x86_64-w64-mingw32/bin/SDL2.dll
	cp $< $@

$(OBJDIR)/%.o: $(SOURCEDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%_win.o: $(SOURCEDIR)/%.c | $(OBJDIR)
	$(CC_WIN) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

cleanwin:
	rm -rf $(TARGET_WIN) $(OBJS_WIN)

cleanlinux:
	rm -rf $(TARGET_LINUX) $(OBJS)

.PHONY: clean linux windows release cleanwin cleanlinux static
