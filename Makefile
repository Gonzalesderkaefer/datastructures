# Project name
PRJNAME := utils

# Compiler
CC := cc

# Compile flags
CFLAGS := -Werror -Wall -Wextra -pedantic -std=c99 -g 

# Source directory
SRCDIR := src

# Build files go here
BUILDDIR := build

# Export dir
TARDIR := target

# Directory of header files for the source files
HEADDIR := include/ds

# Test directory
TESTDIR := test

# Test build dir
TESTBUILDDIR := build/test

# Test source files
TESTSRC := $(shell find $(TESTDIR) -type f -name *.c)

# All source files
SOURCES := $(shell find $(SRCDIR) -type f -name *.c)

# Derive source files for unity build from sources
UNITYSRC := $(patsubst $(SRCDIR)/%,$(UNITY_TARDIR)/%,$(SOURCES))

# Derive object files from source files
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.c=.o))

# Derive test object files from test source files
TESTOBJECTS := $(patsubst $(TESTDIR)/%,$(TESTBUILDDIR)/%,$(TESTSRC:.c=.o))

# Derive Header files from source files
HEADERS := $(patsubst $(SRCDIR)/%,$(HEADDIR)/%,$(SOURCES:.c=.h))

test: $(OBJECTS) $(BUILDDIR)/main.o
	$(CC) $(CFLAGS) $^ -o run.out
	@echo ""
	@./run.out
	@rm run.out



testvg: $(OBJECTS) $(BUILDDIR)/main.o
	$(CC) $(CFLAGS) $^ -o run.out
	@echo ""
	@valgrind ./run.out
	@rm run.out



$(BUILDDIR)/main.o: $(TESTSRC)
	@$(CC) $(CFLAGS) -c $(TESTDIR)/main.c -o $@


# target for test source files
# $(TESTBUILDDIR)/%.o: $(TESTDIR)/%.c
# 	@echo "Building $(shell basename $@)"
# 	@mkdir -p $(shell dirname $@)
# 	$(CC) $(CFLAGS) -c $< -o $@

# Default target for source files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HEADDIR)/%.h
	@echo "Building $(shell basename $@)"
	@mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf build/*
	rm -rf target/*
	$(shell test -e run.out && rm run.out)
