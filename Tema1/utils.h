// Copyright 2022 Mitran Andrei-Gabriel

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <errno.h>

/* useful macro for handling error codes */
#define DIE(assertion, call_description)		\
	do {										\
		if (assertion) {						\
			fprintf(stderr, "(%s, %d): ",		\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);						\
		}										\
	} while (0)

#define uint unsigned int

#define LINE_SIZE 256
#define ARG_SIZE 15
#define NR_ARGS 5
#define SYMB_SIZE 10

typedef struct dll_node_t
{
	void* data;
	struct dll_node_t* next;
	struct dll_node_t* prev;
} dll_node_t;

typedef struct dll_t
{
	dll_node_t* head;
	unsigned int data_size;
	unsigned int size;
} dll_t;

typedef struct card_info_t {
	void *value;
	void *symbol;
} card_info_t;

#endif  // UTILS_H_
