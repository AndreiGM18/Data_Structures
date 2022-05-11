// Copyright 2022 Mitran Andrei-Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dll_func.h"
#include "cards_func.h"

int main(void)
{
	/*
	* Creates the list that storess the decks.
	*/
	dll_t *list_decks = dll_create(sizeof(dll_t));

	/*
	* Gets the entire line from stdin.
	*/
	char line[LINE_SIZE];
	while (fgets(line, LINE_SIZE, stdin)) {
		int len = strlen(line);

		if (line[len - 1] == '\n')
			line[len - 1] = '\0';

		/*
		* Breaks down the line into different arguments.
		*/
		int argc = 0;
		char argv[NR_ARGS][ARG_SIZE];
		break_down_line(line, &argc, argv);

		/*
		* Compares the first argument to check if it is a valid command.
		* If it is, but it has too few or too many following arguments,
		* it is considered invalid.
		* If it is invalid, it prints an error message.
		* However, if it is valid, it enters the specified function.
		* All following required arguments after the first one are
		* turned into integers.
		*/
		if (!strcmp(argv[0], "ADD_DECK")) {
			if (argc != 2) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint n = (uint)atoi(argv[1]);
				add_deck(list_decks, n, list_decks->size);
			}
		} else if (!strcmp(argv[0], "DEL_DECK")) {
			if (argc != 2) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint index;
				index = (uint)atoi(argv[1]);
				del_deck(list_decks, index, 1);
			}
		} else if (!strcmp(argv[0], "DEL_CARD")) {
			if (argc != 3) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint i_deck, i_card;
				i_deck = (uint)atoi(argv[1]);
				i_card = (uint)atoi(argv[2]);
				del_card(list_decks, i_deck, i_card);
			}
		} else if (!strcmp(argv[0], "ADD_CARDS")) {
			if (argc != 3) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint i_deck, n;
				i_deck = (uint)atoi(argv[1]);
				n = (uint)atoi(argv[2]);
				add_cards(list_decks, i_deck, n);
			}
		} else if (!strcmp(argv[0], "DECK_NUMBER")) {
			if (argc != 1) {
				printf("Invalid command. Please try again.\n");
			} else {
				deck_number(list_decks);
			}
		} else if (!strcmp(argv[0], "DECK_LEN")) {
			if (argc != 2) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint index;
				index = (uint)atoi(argv[1]);
				deck_len(list_decks, index);
			}
		} else if (!strcmp(argv[0], "SHUFFLE_DECK")) {
			if (argc != 2) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint index;
				index = (uint)atoi(argv[1]);
				shuffle_deck(list_decks, index);
			}
		} else if (!strcmp(argv[0], "MERGE_DECKS")) {
			if (argc != 3) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint i1, i2;
				i1 = (uint)atoi(argv[1]);
				i2 = (uint)atoi(argv[2]);
				merge_decks(list_decks, i1, i2);
			}
		} else if (!strcmp(argv[0], "SPLIT_DECK")) {
			if (argc != 3) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint index, split;
				index = (uint)atoi(argv[1]);
				split = (uint)atoi(argv[2]);
				split_deck(list_decks, index, split);
			}
		} else if (!strcmp(argv[0], "REVERSE_DECK")) {
			if (argc != 2) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint index;
				index = (uint)atoi(argv[1]);
				reverse_deck(list_decks, index);
			}
		} else if (!strcmp(argv[0], "SORT_DECK")) {
			if (argc != 2) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint index;
				index = (uint)atoi(argv[1]);
				sort_deck(list_decks, index);
			}
		} else if (!strcmp(argv[0], "SHOW_DECK")) {
			if (argc != 2) {
				printf("Invalid command. Please try again.\n");
			} else {
				uint index;
				index = (uint)atoi(argv[1]);
				show_deck(list_decks, index);
			}
		} else if (!strcmp(argv[0], "SHOW_ALL")) {
			if (argc != 1) {
				printf("Invalid command. Please try again.\n");
			} else {
				show_all(list_decks);
			}
		} else if (!strcmp(argv[0], "EXIT")) {
			if (argc != 1) {
				printf("Invalid command. Please try again.\n");
			} else {
				free_all(list_decks);
				/*
				* Breaks the while loop.
				*/
				break;
			}
		} else {
			printf("Invalid command. Please try again.\n");
		}
	}
	return 0;
}
