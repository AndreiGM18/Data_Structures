// Copyright 2022 Mitran Andrei-Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/*
* Functie care trebuie apelata pentru alocarea si initializarea unei liste.
* (Setare valori initiale pentru campurile specifice structurii list).
*/
dll_t*
dll_create(uint data_size)
{
	dll_t *new_list = malloc(sizeof(dll_t));

	DIE(!new_list, "dll_create malloc failed");

	new_list->head = NULL;
	new_list->data_size = data_size;
	new_list->size = 0;

	return new_list;
}

/*
* Functia intoarce un pointer la nodul de pe pozitia n din lista.
* Pozitiile din lista sunt indexate incepand cu 0 (i.e. primul nod din lista se
* afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce ultimul nod.
*/
dll_node_t*
dll_get_nth_node(dll_t* list, uint n)
{
	if (!list || !list->head)
		return NULL;

	if (n >= list->size)
		n = list->size - 1;

	dll_node_t *it = list->head;

	while (n) {
		it = it->next;
		--n;
	}

	return it;
}

/*
* Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
* adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
* lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
* pozitia n=0). Daca n >= nr_noduri, atunci adaugam nodul nou la finalul listei.
*/
void
dll_add_nth_node(dll_t* list, uint n, const void* data)
{
	if (!list)
		return;

	dll_node_t *new = (dll_node_t *)malloc(sizeof(dll_node_t));
	DIE(!new, "new malloc failed");

	new->data = malloc(list->data_size);
	DIE(!new->data, "new->data malloc failed");
	memcpy(new->data, data, list->data_size);

	if (!(list->head)) {
		list->head = new;

		new->next = NULL;

		new->prev = NULL;

		++(list->size);

		return;
	}

	if (n == 0) {
		new->prev = NULL;

		new->next = list->head;

		list->head->prev = new;

		list->head = new;

		++(list->size);

		return;
	}

	dll_node_t *it = dll_get_nth_node(list, n);

	if (n >= list->size) {
		new->next = NULL;

		new->prev = it;

		it->next = new;
	} else {
		new->prev = it->prev;

		it->prev->next = new;

		it->prev = new;

		new->next = it;
	}

	++(list->size);
}

/*
* Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
* parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
* lista se afla pe pozitia n=0). Functia intoarce un pointer spre acest nod
* proaspat eliminat din lista. Daca n >= nr_noduri - 1, se elimina nodul de la
* finalul listei. Este responsabilitatea apelantului sa elibereze memoria
* acestui nod.
*/
dll_node_t*
dll_remove_nth_node(dll_t* list, uint n)
{
	if (!list || !list->head)
		return NULL;

	if (list->size == 1) {
		dll_node_t *start = list->head;

		list->head = NULL;

		--(list->size);

		return start;
	}

	if (n == 0) {
		dll_node_t *start = list->head;

		start->next->prev = NULL;

		list->head = start->next;

		--(list->size);

		return start;
	}

	dll_node_t *it = dll_get_nth_node(list, n);

	if (n >= list->size - 1) {
		it->prev->next = NULL;
	} else {
		it->prev->next = it->next;

		it->next->prev = it->prev;
	}

	--(list->size);

	return it;
}

/*
* Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
* parametru.
*/
uint
dll_get_size(dll_t* list)
{
	if (!list)
		return 0;

	return list->size;
}

/*
* Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
* sfarsit, elibereaza memoria folosita de structura lista.
*/
void
dll_free(dll_t** pp_list)
{
	if (!(*pp_list))
		return;

	if (!(*pp_list)->head) {
		free(*pp_list);
		*pp_list = NULL;
		return;
	}

	dll_node_t *it = (*pp_list)->head;
	dll_node_t *tmp = it->next;

	free(it->data);
	free(it);

	while(tmp) {
		it = tmp;
		tmp = it->next;

		free(it->data);
		free(it);
	}

	free(*pp_list);
	*pp_list = NULL;
}

