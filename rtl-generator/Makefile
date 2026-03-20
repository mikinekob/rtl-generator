# Makefile for RTL Generator

# Compiler
CC = gcc  

# Directories
SRCDIR = src  
BINDIR = bin  

# Flags
CFLAGS = -Wall -Wextra

# Source files
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Output binary name
OUTPUT = $(BINDIR)/rtl_generator

# Build target
all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# Clean target
clean:
	rm -f $(BINDIR)/*