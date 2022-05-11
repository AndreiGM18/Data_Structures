# Copyright 2022 Mitran Andrei Gabriel

# Compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# Defining targets
TARGETS=main

build: $(TARGETS)

main: main.c
		$(CC) $(CFLAGS) -g main.c -o tema1 dll_func.c cards_func.c

pack:
		zip -FSr 313CA_MitranAndreiGabriel_Tema1.zip README Makefile *.c *.h

clean:
		rm -f $(TARGETS)

.PHONY: pack clean