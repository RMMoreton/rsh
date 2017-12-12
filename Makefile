CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -std=c99

.PHONY: clean

all: shell

shell: shell.c shell.h
	$(CC) $(CFLAGS) shell.c -o shell

clean:
	rm shell
