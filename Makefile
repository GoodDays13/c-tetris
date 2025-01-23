CC=gcc
CFLAGS=-Wall -Wextra -Werror -g
INCLUDES=-Iinclude
LDFLAGS=-lSDL2
SOURCEDIR=src
BINDIR=bin
OBJDIR=obj
TARGET=$(BINDIR)/main
OBJS := $(patsubst $(SOURCEDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SOURCEDIR)/*.c))

ALL: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SOURCEDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(BINDIR)
