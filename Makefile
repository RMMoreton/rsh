# This Makefile is for rsh, Rob's shell.

# Set the compiler and some default flags.
CC := gcc
CFLAGS := -Wall -Wpedantic -Wextra -std=c99

# If we want to debug, add the -g flag to CFLAGS.
SHELL_DEBUG ?= TRUE
ifeq ($(SHELL_DEBUG),TRUE)
	CFLAGS += -g
endif

# Set the names of various files we'll compile.
EXECUTABLE := rsh
OBJECTS := shell_base.o shell_string.o

# Calls to `make` generate the executable.
all : $(EXECUTABLE)

# Creating the executable relies on the object files. To compile the final
# product, just link all the objects together and call it $(EXECUTABLE).
$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# If we're running a rule for a `.o` file, add `-c` to the CFLAGS.
# All `.o` files rely on appropriate `.c` and `.h` files. To compile them,
# run the compiler with appropriate flags, output to the correct `.o` file ($@),
# and use the appropriate source file ($<).
%.o : CFLAGS += -c
%.o : %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

# Remove the executable and the objects.
clean :
	rm -f $(EXECUTABLE) $(OBJECTS)

.PHONY: clean
