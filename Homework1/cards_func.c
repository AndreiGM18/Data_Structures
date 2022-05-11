// Copyright 2022 Mitran Andrei-Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "dll_func.h"

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

void
add_card(dll_t *deck, int value, char *symbol, uint pos)
{
	card_info_t card_info;

	card_info.value = malloc(sizeof(int));
	DIE(!card_info.value, "card_info.value malloc failed");
	memcpy(card_info.value, &value, sizeof(int));

	card_info.symbol = malloc(SYMB_SIZE * sizeof(char));
	DIE(!card_info.symbol, "card_info.symbol malloc failed");
	memcpy(card_info.symbol, symbol, SYMB_SIZE * sizeof(char));

	dll_add_nth_node(deck, pos, &card_info);
}

void
add_n_cards_at_end(dll_t *deck, int n)
{
	int value;
	char symbol[SYMB_SIZE];
	char line[LINE_SIZE];

	while (n) {
		fgets(line, LINE_SIZE, stdin);
		int len = strlen(line);

		if (line[len - 1] == '\n')
			line[len - 1] = '\0';

		int argc = 0;
		char argv[NR_ARGS][ARG_SIZE];

		break_down_line(line, &argc, argv);

		if (argc != 2) {
			printf("The provided card is not a valid one.\n");
		} else {
			value = atoi(argv[0]);
			strcpy(symbol, argv[1]);

			if (!check_symbol(symbol) || value < 1 || value > 14) {
			printf("The provided card is not a valid one.\n");
			} else {
			add_card(deck, value, symbol, deck->size);
			--n;
			}
		}
	}
}

void
add_deck(dll_t *list_decks, int n, uint pos)
{
	dll_t *new_deck = dll_create(sizeof(card_info_t));
	add_n_cards_at_end(new_deck, n);
	if (new_deck) {
		dll_add_nth_node(list_decks, pos, new_deck);
		free(new_deck);
		printf("The deck was successfully created with %d cards.\n", n);
	}
}

void
free_card(dll_node_t *card)
{
	free(((card_info_t *)card->data)->symbol);
	free(((card_info_t *)card->data)->value);
	free(card->data);
	free(card);
}

void
free_deck(dll_t *deck)
{
	if (!deck || !deck->size)
		return;

	dll_node_t *it = dll_remove_nth_node(deck, 0);

	while (deck->size) {
		free_card(it);
		it = dll_remove_nth_node(deck, 0);
	}
	free_card(it);
}

void
del_deck(dll_t *list_decks, int index, int print)
{
	if (!list_decks)
		return;

	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_node_t *deck_node = dll_remove_nth_node(list_decks, index);
		dll_t *deck = (dll_t *)deck_node->data;
		free_deck(deck);
		free(deck);
		free(deck_node);
		if (print)
			printf("The deck %d was successfully deleted.\n", index);
	}
}

void
del_card(dll_t *list_decks, int i_deck, int i_card)
{
	if (!list_decks)
		return;

	if (i_deck >= (int)list_decks->size || i_deck < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, i_deck)->data;
		if (i_card >= (int)deck->size || i_card < 0) {
			printf("The provided index is out of bounds for deck %d.\n", i_deck);
		} else {
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

void
add_cards(dll_t *list_decks, int i_deck, uint n)
{
	if (!list_decks)
		return;

	if (i_deck >= (int)list_decks->size || i_deck < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, i_deck)->data;
		add_n_cards_at_end(deck, n);
		printf("The cards were successfully added to deck %d.\n", i_deck);
	}
}

void
deck_number(dll_t *list_decks)
{
	if (!list_decks)
		return;

	printf("The number of decks is %u.\n", list_decks->size);
}

void
deck_len(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		printf("The length of deck %d is %u.\n", index, deck->size);
	}
}

void
copy_card(dll_t *deck_dest, uint pos_dest, dll_t *deck_src, uint pos_src)
{
	dll_node_t *card = dll_get_nth_node(deck_src, pos_src);

	int value = *(int *)((card_info_t *)(card->data))->value;
	char symbol[SYMB_SIZE];
	strcpy(symbol, (char *)((card_info_t *)(card->data))->symbol);

	add_card(deck_dest, value, symbol, pos_dest);
}

void
shuffle_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *new_deck = dll_create(sizeof(card_info_t));

		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		for (uint i = deck->size / 2; i < deck->size; ++i) {
			copy_card(new_deck, new_deck->size, deck, i);
		}

		for (uint i = 0; i < deck->size / 2; ++i) {
			copy_card(new_deck, new_deck->size, deck, i);
		}

		del_deck(list_decks, index, 0);

		dll_add_nth_node(list_decks, index, new_deck);

		free(new_deck);

		printf("The deck %d was successfully shuffled.\n", index);
	}
}

void
merge_decks(dll_t *list_decks, int i1, int i2)
{
	if (!list_decks)
		return;

	int size = (int)list_decks->size;

	if (i1 >= size || i2 >= size || i1 < 0 || i2 < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *new_deck = dll_create(sizeof(card_info_t));

		int i_min, i_max;
		dll_node_t *deck1_node, *deck2_node;

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

		del_deck(list_decks, i_max, 0);
		del_deck(list_decks, i_min, 0);

		dll_add_nth_node(list_decks, list_decks->size, new_deck);

		free(new_deck);

		printf("The deck %d and the deck %d were successfully merged.\n", i1, i2);
	}
}

void
split_deck(dll_t *list_decks, int index, int split)
{
	if (!list_decks)
		return;

	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		if (!split) {
			printf("The deck %d was successfully split by index %d.\n", index, split);
			return;
		}

		if (split >= (int)deck->size || split < 0) {
			printf("The provided index is out of bounds for deck %d.\n", index);
			return;
		} else {
			dll_t *new_deck1 = dll_create(sizeof(card_info_t));
			dll_t *new_deck2 = dll_create(sizeof(card_info_t));

			for (int i = 0; i < split; ++i) {
				copy_card(new_deck1, new_deck1->size, deck, i);
			}

			for (uint i = split; i < deck->size; ++i) {
				copy_card(new_deck2, new_deck2->size, deck, i);
			}

			del_deck(list_decks, index, 0);

			dll_add_nth_node(list_decks, index, new_deck1);
			dll_add_nth_node(list_decks, index + 1, new_deck2);

			free(new_deck1);
			free(new_deck2);

			printf("The deck %d was successfully split by index %d.\n", index, split);
		}
	}
}


void
reverse_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *new_deck = dll_create(sizeof(card_info_t));

		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		for (int i = (int)deck->size - 1; i >= 0; --i) {
			copy_card(new_deck, new_deck->size, deck, i);
		}

		del_deck(list_decks, index, 0);

		dll_add_nth_node(list_decks, index, new_deck);

		free(new_deck);

		printf("The deck %d was successfully reversed.\n", index);
	}
}

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

void
swap_card_info(card_info_t **card1, card_info_t **card2)
{
	card_info_t *aux = *card1;
	*card1 = *card2;
	*card2 = aux;
}

void
sort_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
		dll_t *new_deck = dll_create(sizeof(card_info_t));

		dll_t *deck = (dll_t *)dll_get_nth_node(list_decks, index)->data;

		card_info_t **cards = malloc(deck->size * sizeof(card_info_t *));
		DIE(!cards, "cards malloc failed");

		create_card_array(cards, deck);

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

		for (uint i = 0; i < deck->size; ++i) {
			int value = *(int *)cards[i]->value;
			char symbol[SYMB_SIZE];
			strcpy(symbol, (char *)cards[i]->symbol);
			add_card(new_deck, value, symbol, new_deck->size);
		}

		free_card_array(cards, deck);

		del_deck(list_decks, index, 0);

		dll_add_nth_node(list_decks, index, new_deck);

		free(new_deck);

		printf("The deck %d was successfully sorted.\n", index);
	}
}

void
print_card(card_info_t *card_info)
{
	if (!card_info)
		return;
	printf("%d %s\n", *((int *)card_info->value), (char *)card_info->symbol);
}

void
show_deck(dll_t *list_decks, int index)
{
	if (!list_decks)
		return;

	if (index >= (int)list_decks->size || index < 0) {
		printf("The provided index is out of bounds for the deck list.\n");
		return;
	} else {
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

void
show_all(dll_t *list_decks)
{
	if (!list_decks || !list_decks->size)
		return;

	for (uint i = 0; i< list_decks->size; ++i) {
		show_deck(list_decks, i);
	}
}

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
