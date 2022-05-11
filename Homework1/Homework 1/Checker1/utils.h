// Copyright 2022 Mitran Andrei-Gabriel

#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <errno.h>

/*
* Useful macro for handling error codes
*/
#define DIE(assertion, call_description)		\
	do {										\
		if (assertion) {						\
			fprintf(stderr, "(%s, %d): ",		\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);						\
		}										\
	} while (0)

/*
* Useful defines
*/
#define uint unsigned int
#define LINE_SIZE 256
#define ARG_SIZE 15
#define NR_ARGS 5
#define SYMB_SIZE 10

/*
* The node structure:
* 1. the node's data (an int, a string, a struct etc.)
* 2. a pointer to the next node
* 3. a pointer to the previous node
*/
typedef struct dll_node_t
{
	void* data;
	struct dll_node_t* next;
	struct dll_node_t* prev;
} dll_node_t;

/*
* The list structure:
* 1. a pointer to the first node of the list (the list's head)
* 2. a node's data's size (1 for a char, 4 for an int etc.)
* 3. the list's size (how many nodes there are in the list)
*/
typedef struct dll_t
{
	dll_node_t* head;
	unsigned int data_size;
	unsigned int size;
} dll_t;

/*
* The card structure:
* 1. a pointer to the card's value
* 2. a pointer to the card's symbol
*/
typedef struct card_info_t {
	void *value;
	void *symbol;
} card_info_t;

#endif  // UTILS_H_
