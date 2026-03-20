# RTL Generator Makefile (w64devkit compatible)

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SOURCES = $(SRCDIR)/main.c $(SRCDIR)/generator.c $(SRCDIR)/template.c
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))
TARGET = $(BINDIR)/rtl-gen.exe

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

$(TARGET): $(BINDIR) $(OBJDIR) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: $(TARGET)
	$(TARGET)

.PHONY: all clean run