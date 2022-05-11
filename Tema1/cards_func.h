// Copyright 2022 Mitran Andrei-Gabriel

#ifndef CARDS_FUNC_H_
#define CARDS_FUNC_H_

#include "utils.h"

void
break_down_line(char *line, int *argc, char argv[NR_ARGS][ARG_SIZE]);

int
check_symbol(char *symbol);

void
add_card(dll_t *deck, int value, char *symbol, uint pos);

void
add_n_cards_at_end(dll_t *deck, uint n);

void
add_deck(dll_t *list_decks, int n, uint pos);

void
free_card(dll_node_t *card);

void
free_deck(dll_t *deck);

void
del_deck(dll_t *list_decks, int index, int print);

void
del_card(dll_t *list_decks, int i_deck, int i_card);

void
add_cards(dll_t *list_decks, int i_deck, int n);

void
deck_number(dll_t *list_decks);

void
deck_len(dll_t *list_decks, int index);

void
copy_card(dll_t *deck_dest, uint pos_dest, dll_t *deck_src, uint pos_src);

void
shuffle_deck(dll_t *list_decks, int index);

void
merge_decks(dll_t *list_decks, int i1, int i2);

void
split_deck(dll_t *list_decks, int index, int split);

void
reverse_deck(dll_t *list_decks, int index);

void
create_card_array(card_info_t **cards, dll_t *deck);

void
free_card_array(card_info_t **cards, dll_t *deck);

void
swap_card_info(card_info_t **card1, card_info_t **card2);

void
sort_deck(dll_t *list_decks, int index);

void
print_card(card_info_t *card_info);

void
show_deck(dll_t *list_decks, int index);

void
show_all(dll_t *list_decks);

void
free_all(dll_t *list_decks);

#endif  // CARDS_FUNC_H_
