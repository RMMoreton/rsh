CC=gcc
CFLAGS=-Wall -Wpedantic -Wextra -std=c99

.PHONY: clean

all: shell

shell: shell.c shell.h shell-base shell-string
	$(CC) $(CFLAGS) shell_base.o shell_string.o shell.c -o shell

shell-base: shell_base.c shell_base.h
	$(CC) $(CFLAGS) -c shell_base.c

shell-string: shell_string.c shell_string.h
	$(CC) $(CFLAGS) -c shell_string.c

clean:
	rm -f shell shell_base.o shell_string.o
