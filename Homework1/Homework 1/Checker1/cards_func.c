// Copyright 2022 Mitran Andrei-Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dll_func.h"

/*
* Breaks down the read line from stdin into different arguments.
* Also gets how many arguments there are.
*/
void
break_down_line(char *line, int *argc, char argv[NR_ARGS][ARG_SIZE])
{
	char *tok = strtok(line, "\n ");

	while (tok) {
		strcpy(argv[(*argc)], tok);
		++(*argc);
		tok = strtok(NULL, "\n ");
	}
}

/*
* Checks if the symbol is a valid one.
*/
int
check_symbol(char *symbol)
{
	if (!strcmp(symbol, "HEART"))
		return 1;
	if (!strcmp(symbol, "SPADE"))
		return 1;
	if (!strcmp(symbol, "DIAMOND"))
		return 1;
	if (!strcmp(symbol, "CLUB"))
		return 1;
	return 0;
}

/*
* Adds a card to the specified deck at a specified position.
*/
void
add_card(dll_t *deck, int value, char *symbol, uint pos)
{
	/*
	* Creates a new card structure.
	*/
	card_info_t card_info;

	/*
	* Adds the card's value to the card structure.
	*/
	card_info.value = malloc(sizeof(int));
	DIE(!card_info.value, "card_info.value malloc failed");
	memcpy(card_info.value, &value, sizeof(int));

	/*
	* Adds the card's symbol to the card structure.
	*/
	card_info.symbol = malloc(SYMB_SIZE * sizeof(char));
	DIE(!card_info.symbol, "card_info.symbol malloc failed");
	memcpy(card_info.symbol, symbol, SYMB_SIZE * sizeof(char));

	/*
	* Adds the card at the specified position of the deck.
	*/
	dll_add_nth_node(deck, pos, &card_info);
}

/*
* Adds n cards at the end of the specified deck.
* Checks if the cards are valid before adding them.
*/
void
add_n_cards_at_end(dll_t *deck, int n)
{
	int value;
	char symbol[SYMB_SIZE];
	char line[LINE_SIZE];

	/*
	* Assumes there are at least n valid cards.
	*/
	while (n) {
		/*
		* Gets the line and breaks it down into arguments.
		*/
		fgets(line, LINE_SIZE, stdin);
		int len = strlen(line);

		if (line[len - 1] == '\n')
			line[len - 1] = '\0';

		int argc = 0;
		char argv[NR_ARGS][ARG_SIZE];

		break_down_line(line, &argc, argv);

		/*
		* If there are more than 2 arguments, the card is invalid.
		* Prints an error message.
		*/
		if (argc != 2) {
			printf("The provided card is not a valid one.\n");
		} else {
			/*
			* Gets the value and symbol from the arguments.
			*/
			value = atoi(argv[0]);
			strcpy(symbol, argv[1]);

			/*
			* Checks if the card is valid.
			* If it is, adds the the card at the end of the deck
			* and it subtracts 1 from n.
			*/
			if (!check_symbol(symbol) || value < 1 || value > 14) {
				printf("The provided card is not a valid one.\n");
			} else {
				add_card(deck, value, symbol, deck->size);
				--n;
			}
		}
	}
}

/*
* Adds a deck to the list of decks.
*/
void
add_deck(dll_t *list_decks, int n, uint pos)
{
	/*
	* Creates a new deck and adds n cards to it.
	*/
	dll_t *new_deck = dll_create(sizeof(card_info_t));
	add_n_cards_at_end(new_deck, n);

	/*
	* Adds the new deck to the list and prints a message.
	* Since more memory is allocated when adding a node,
	* the new deck list structure must be freed in order
	* to avoid memory leaks.
	*/
	if (new_deck) {
		dll_add_nth_node(list_decks, pos, new_deck);
		free(new_deck);
		printf("The deck was successfully created with %d cards.\n", n);
	}
}

/*
* Frees the memory allocated for a card (including the node itself).
* A card is a node for a deck.
*/
void
free_card(dll_node_t *card)
{
	free(((card_info_t *)card->data)->symbol);
	free(((card_info_t *)card->data)->value);
	free(card->data);
	free(card);
}

/*
* Frees the memory allocated for a deck (excluding the node itself).
* A deck is a node for the list of decks, as well as a list.
*/
void
free_deck(dll_t *deck)
{
	if (!deck || !deck->size)
		return;

	/*
	* Removes and frees all the cards from the deck.
	*/
	dll_node_t *it = dll_remove_nth_node(deck, 0);
	while (deck->size) {
		free_card(it);
		it = dll_remove_nth_node(deck, 0);
	}
	free_card(it);
}

/*
* Deletes a deck from the list of decks.
* The print parameter is helpful in order to call this function
* without printing the message.
*/
void
del_deck(dll_t *list_decks, int index, int print)
{
	if (!list_decks)
		return;

	/*
	* Checks if the deck's index is valid.
	*/
	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Removes and frees all the allocated memory for the deck (both as
		* a list in its own right and as a node in the list of decks).
		*/
		dll_node_t *deck_node = dll_remove_nth_node(list_decks, index);
		dll_t *deck = (dll_t *)deck_node->data;
		free_deck(deck);
		free(deck);
		free(deck_node);

		/*
		* Prints a message if specified to do so.
		*/
		if (print)
			printf("The deck %d was successfully deleted.\n", index);
	}
}

/*
* Deletes a specified card in a specified deck.
* If after deletion, the deck is empty, it is deleted from the list of
* decks.
*/
void
del_card(dll_t *list_decks, int i_deck, int i_card)
{
	if (!list_decks)
		return;

	/*
	* Checks if the deck's index is valid.
	*/
	if (i_deck >= (int)list_decks->size || i_deck < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Gets the specified deck.
		*/
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, i_deck)->data;

		/*
		* Checks if the card's index is valid.
		*/
		if (i_card >= (int)deck->size || i_card < 0) {
			printf("The provided index is out of bounds for deck %d.\n", i_deck);
		} else {
			/*
			* If after deletion, it would result in the deck being empty,
			* simply calls the del_deck() function.
			* If not, removes and frees the card from the deck.
			*/
			if (deck->size == 1) {
				del_deck(list_decks, i_deck, 0);
			} else {
				dll_node_t *card = dll_remove_nth_node(deck, i_card);
				free_card(card);
			}
			printf("The card was successfully deleted from deck %d.\n", i_deck);
		}
	}
}

/*
* Adds n cards to the specified deck.
*/
void
add_cards(dll_t *list_decks, int i_deck, uint n)
{
	if (!list_decks)
		return;

	/*
	* Checks if the deck's index is valid.
	*/
	if (i_deck >= (int)list_decks->size || i_deck < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Gets the specified deck, then adds n valid cards at the end of it.
		*/
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, i_deck)->data;
		add_n_cards_at_end(deck, n);
		printf("The cards were successfully added to deck %d.\n", i_deck);
	}
}

/*
* Gets the number of decks in the list of decks.
*/
void
deck_number(dll_t *list_decks)
{
	if (!list_decks)
		return;

	printf("The number of decks is %u.\n", list_decks->size);
}

/*
* Gets the number of cards in a specified deck.
*/
void
deck_len(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	/*
	* Checks if the deck's index is valid.
	*/
	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Gets the specified deck and prints the number of cards in the deck.
		*/
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		printf("The length of deck %d is %u.\n", index, deck->size);
	}
}

/*
* Copies a card from a specified position in a specified deck to a specified
* position in another specified deck.
*/
void
copy_card(dll_t *deck_dest, uint pos_dest, dll_t *deck_src, uint pos_src)
{
	/*
	* Gets the card from the specified position in the source deck.
	*/
	dll_node_t *card = dll_get_nth_node(deck_src, pos_src);

	/*
	* Creates copies of its value and symbol.
	*/
	int value = *(int *)((card_info_t *)(card->data))->value;
	char symbol[SYMB_SIZE];
	strcpy(symbol, (char *)((card_info_t *)(card->data))->symbol);

	/*
	* Adds the copy to the destination deck at a specified position.
	*/
	add_card(deck_dest, value, symbol, pos_dest);
}

/*
* Shuffles a specified deck.
* (Switches its first half with its second half.)
*/
void
shuffle_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	/*
	* Checks if the deck's index is valid.
	*/
	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Creates a new deck in order to put the reversed halfs of
		* the specified deck to be shuffled.
		*/
		dll_t *new_deck = dll_create(sizeof(card_info_t));
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		/*
		* Copies the second half of the original deck.
		*/
		for (uint i = deck->size / 2; i < deck->size; ++i) {
			copy_card(new_deck, new_deck->size, deck, i);
		}

		/*
		* Copies the first half of the original deck.
		*/
		for (uint i = 0; i < deck->size / 2; ++i) {
			copy_card(new_deck, new_deck->size, deck, i);
		}

		/*
		* Deletes the original deck, then it adds the new one.
		* Since more memory is allocated when adding a node,
		* the new deck list structure must be freed in order
		* to avoid memory leaks.
		*/
		del_deck(list_decks, index, 0);
		dll_add_nth_node(list_decks, index, new_deck);
		free(new_deck);

		printf("The deck %d was successfully shuffled.\n", index);
	}
}

/*
* Merges two decks and then adds the resulting deck at the end of
* the list of decks.
*/
void
merge_decks(dll_t *list_decks, int i1, int i2)
{
	if (!list_decks)
		return;

	int size = (int)list_decks->size;

	/*
	* Checks the decks' indexes.
	*/
	if (i1 >= size || i2 >= size || i1 < 0 || i2 < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Creates a new deck in order to put the cards of
		* the specified decks.
		*/
		dll_t *new_deck = dll_create(sizeof(card_info_t));

		int i_min, i_max;
		dll_node_t *deck1_node, *deck2_node;

		/*
		* Efficently gets the two decks and checks which
		* index is greater.
		*/
		if (i1 <= i2) {
			i_min = i1;
			i_max = i2;

			deck1_node = dll_get_nth_node(list_decks, i1);

			deck2_node = deck1_node;
			int i = i1;
			while (i < i2) {
				deck2_node = deck2_node->next;
				++i;
			}
		} else {
			i_min = i2;
			i_max = i1;

			deck2_node = dll_get_nth_node(list_decks, i2);

			deck1_node = deck2_node;
			int i = i2;
			while (i < i1) {
				deck1_node = deck1_node->next;
				++i;
			}
		}

		dll_t *deck1 = (dll_t *)deck1_node->data;
		dll_t *deck2 = (dll_t *)deck2_node->data;

		/*
		* Copies the first card from the first deck, then the first one
		* from the second deck, then the second card from the first deck,
		* then the second one from the second deck and so on.
		* If there are no cards left in one of the decks, it copies all of
		* the remaining cards in the other deck.
		*/
		if (deck1->size <= deck2->size) {
			uint i = 0;
			while (i < deck1->size) {
				copy_card(new_deck, new_deck->size, deck1, i);

				copy_card(new_deck, new_deck->size, deck2, i);

				++i;
			}
			while (i < deck2->size) {
				copy_card(new_deck, new_deck->size, deck2, i);

				++i;
			}
		} else {
			uint i = 0;
			while (i < deck2->size) {
				copy_card(new_deck, new_deck->size, deck1, i);

				copy_card(new_deck, new_deck->size, deck2, i);

				++i;
			}
			while (i < deck1->size) {
				copy_card(new_deck, new_deck->size, deck1, i);

				++i;
			}
		}

		/*
		* Deletes the original decks, then it adds the new one.
		* Since more memory is allocated when adding a node,
		* the new deck list structure must be freed in order
		* to avoid memory leaks.
		*/
		del_deck(list_decks, i_max, 0);
		del_deck(list_decks, i_min, 0);
		dll_add_nth_node(list_decks, list_decks->size, new_deck);
		free(new_deck);

		printf("The deck %d and the deck %d were successfully merged.\n", i1, i2);
	}
}

/*
* Splits a specified deck at a specified split index into two separate decks.
* The original deck is deleted.
* It adds the new ones at the original deck's position and at the one after it.
*/
void
split_deck(dll_t *list_decks, int index, int split)
{
	if (!list_decks)
		return;

	/*
	* Check's the deck's index.
	*/
	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Checks the index from which to split the deck.
		* If the index is 0, simply returns, as the deck would remain unchanged.
		*/
		if (!split) {
			printf("The deck %d was successfully split by index %d.\n", index, split);
			return;
		}
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		if (split >= (int)deck->size || split < 0) {
			printf("The provided index is out of bounds for deck %d.\n", index);
			return;
		} else {
			/*
			* Creates two new decks in which to store the split deck.
			*/
			dll_t *new_deck1 = dll_create(sizeof(card_info_t));
			dll_t *new_deck2 = dll_create(sizeof(card_info_t));

			/*
			* Copies all cards from the beginning of the original deck
			* until the split index is reached into the newly created deck.
			*/
			for (int i = 0; i < split; ++i) {
				copy_card(new_deck1, new_deck1->size, deck, i);
			}

			/*
			* Copies all cards from the split index until the end of the 
			* original deck into the second newly created deck.
			*/
			for (uint i = split; i < deck->size; ++i) {
				copy_card(new_deck2, new_deck2->size, deck, i);
			}

			/*
			* Deletes the original deck, then it adds the new ones.
			* Since more memory is allocated when adding a node,
			* the new deck list structures must be freed in order
			* to avoid memory leaks.
			*/
			del_deck(list_decks, index, 0);
			dll_add_nth_node(list_decks, index, new_deck1);
			dll_add_nth_node(list_decks, index + 1, new_deck2);
			free(new_deck1);
			free(new_deck2);

			printf("The deck %d was successfully split by index %d.\n", index, split);
		}
	}
}

/*
* Reverses a specified deck.
*/
void
reverse_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	/*
	* Check's the deck's index.
	*/
	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Creates a new deck in order to put the cards of
		* the specified deck in reverse.
		*/
		dll_t *new_deck = dll_create(sizeof(card_info_t));
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		/*
		* Copies the cards in reverse order.
		*/
		for (int i = (int)deck->size - 1; i >= 0; --i) {
			copy_card(new_deck, new_deck->size, deck, i);
		}

		/*
		* Deletes the original deck, then it adds the new one.
		* Since more memory is allocated when adding a node,
		* the new deck list structure must be freed in order
		* to avoid memory leaks.
		*/
		del_deck(list_decks, index, 0);
		dll_add_nth_node(list_decks, index, new_deck);
		free(new_deck);

		printf("The deck %d was successfully reversed.\n", index);
	}
}

/*
* Compares two cards' symbols.
* Returns 1 if the first card's symbol is 'greater than'
* the second one's.
*/
int
compare_symbol(char *symbol1, char *symbol2)
{
	if (!strcmp(symbol1, symbol2))
		return 0;
	if (!strcmp(symbol1, "HEART"))
		return 0;
	if (!strcmp(symbol1, "SPADE")) {
		if (!strcmp(symbol2, "HEART"))
			return 1;
		return 0;
	}
	if (!strcmp(symbol1, "DIAMOND")) {
		if (!strcmp(symbol2, "CLUB"))
			return 0;
		return 1;
	}
	return 1;
}

/*
* Creates an array of cards from a list.
*/
void
create_card_array(card_info_t **cards, dll_t *deck)
{
	dll_node_t *card = deck->head;
	for (uint i = 0; i < deck->size; ++i) {
			cards[i] = malloc(sizeof(card_info_t));
			DIE(!cards[i], "cards[i] malloc failed");

			int value = *(int *)((card_info_t *)card->data)->value;
			char symbol[SYMB_SIZE];
			strcpy(symbol, (char *)((card_info_t *)(card->data))->symbol);

			cards[i]->value = malloc(sizeof(int *));
			DIE(!cards[i]->value, "cards[i]->value malloc failed");
			memcpy(cards[i]->value, &value, sizeof(int));

			cards[i]->symbol = malloc(SYMB_SIZE * sizeof(char));
			DIE(!cards[i]->symbol, "cards[i]->symbol malloc failed");
			memcpy(cards[i]->symbol, symbol, SYMB_SIZE * sizeof(char));

			card = card->next;
		}
}

/*
* Frees an array of cards.
*/
void
free_card_array(card_info_t **cards, dll_t *deck)
{
	for (uint i = 0; i < deck->size; ++i) {
			free(cards[i]->value);
			free(cards[i]->symbol);
			free(cards[i]);
		}
	free(cards);
}

/*
* Swaps two cards' value and symbol.
*/
void
swap_card_info(card_info_t **card1, card_info_t **card2)
{
	card_info_t *aux = *card1;
	*card1 = *card2;
	*card2 = aux;
}

/*
* Sorts a specified deck.
*/
void
sort_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	/*
	* Checks the deck's index.
	*/
	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Creates a new deck in order to put the sorted cards of
		* the specified deck.
		*/
		dll_t *new_deck = dll_create(sizeof(card_info_t));
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		/*
		* Creates an array of cards from the deck, so that sorting them
		* is more efficient.
		*/
		card_info_t **cards = malloc(deck->size * sizeof(card_info_t *));
		DIE(!cards, "cards malloc failed");
		create_card_array(cards, deck);

		/*
		* Selection sort in the card array
		*/
		for (uint i = 0; i < deck->size - 1; ++i) {
			for (uint j = i; j < deck->size; ++j) {
				if (*(int *)cards[i]->value > *(int *)cards[j]->value) {
					swap_card_info(&cards[i], &cards[j]);
				} else if (*(int *)cards[i]->value == *(int *)cards[j]->value &&
				compare_symbol((char *)cards[i]->symbol, (char *)cards[j]->symbol)) {
					swap_card_info(&cards[i], &cards[j]);
				}
			}
		}

		/*
		* Adds the sorted cards to the new deck.
		*/
		for (uint i = 0; i < deck->size; ++i) {
			int value = *(int *)cards[i]->value;
			char symbol[SYMB_SIZE];
			strcpy(symbol, (char *)cards[i]->symbol);
			add_card(new_deck, value, symbol, new_deck->size);
		}

		/*
		* Frees the card array.
		*/
		free_card_array(cards, deck);

		/*
		* Deletes the original deck, then it adds the new one.
		* Since more memory is allocated when adding a node,
		* the new deck list structure must be freed in order
		* to avoid memory leaks.
		*/
		del_deck(list_decks, index, 0);
		dll_add_nth_node(list_decks, index, new_deck);
		free(new_deck);

		printf("The deck %d was successfully sorted.\n", index);
	}
}

/*
* Prints a card's value and symbol.
*/
void
print_card(card_info_t *card_info)
{
	if (!card_info)
		return;
	printf("%d %s\n", *((int *)card_info->value), (char *)card_info->symbol);
}

/*
* Prints a specified deck.
*/
void
show_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	/*
	* Checks the deck's index.
	*/
	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		/*
		* Gets the deck and prints all of its cards.
		*/
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;
		printf("Deck %d:\n", index);
		dll_node_t *card = deck->head;
		for (uint i = 0; i < deck->size; ++i) {
			printf("\t");
			print_card((card_info_t *)card->data);
			card = card->next;
		}
	}
}

/*
* Prints all decks in the list of decks.
*/
void
show_all(dll_t *list_decks)
{
	if (!list_decks || !list_decks->size)
		return;

	for (uint i = 0; i< list_decks->size; ++i) {
		show_deck(list_decks, i);
	}
}

/*
* Frees all the allocated memory used for
* the decks and the list of decks.
*/
void
free_all(dll_t *list_decks)
{
	if (!list_decks)
		return;

	dll_node_t *deck_node = list_decks->head;
	for (uint i = 0; i < list_decks->size; ++i) {
		dll_t *deck = (dll_t *)deck_node->data;
		free_deck(deck);
		deck_node = deck_node->next;
	}
	dll_free(&list_decks);
}
