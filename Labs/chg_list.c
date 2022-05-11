#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 64

/* 
* Am schimbat comentariile pentru intelegerea mai buna a codului.
*/

typedef struct dll_node_t
{
	void* data; /* Pentru ca datele stocate sa poata avea orice tip, folosim un
				pointer la void. */
	struct dll_node_t *prev, *next;
} dll_node_t;

typedef struct doubly_linked_list_t
{
	dll_node_t* head;
	unsigned int data_size;
	unsigned int size;
} doubly_linked_list_t;

/*
* Functie care trebuie apelata pentru alocarea si initializarea unei liste.
* (Setare valori initiale pentru campurile specifice structurii list).
*/
doubly_linked_list_t*
dll_create(unsigned int data_size)
{
	doubly_linked_list_t *new_list = malloc(sizeof(doubly_linked_list_t));

	if (!new_list)
		return NULL;

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
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
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
dll_add_nth_node(doubly_linked_list_t* list, unsigned int n, const void* data)
{
	if (!list)
		return;

	dll_node_t *new = (dll_node_t *)malloc(sizeof(dll_node_t));
		if (!new)
			exit(-1);

		new->data = malloc(list->data_size);
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
dll_remove_nth_node(doubly_linked_list_t* list, unsigned int n)
{
	if (!list || !list->head)
		return NULL;

	if (n == 0) {
		dll_node_t *start = list->head;

		start->next->prev = NULL;

		list->head = start->next;

		--(list->size);

		return start;
	}

	dll_node_t *it = dll_get_nth_node(list, n);

	if (n >= list->size - 1)
		it->prev->next = NULL;
	else {
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
unsigned int
dll_get_size(doubly_linked_list_t* list)
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
dll_free(doubly_linked_list_t** pp_list)
{
	if (!(*pp_list))
		return;

	if(!(*pp_list)->head) {
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

/*
* Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
* ca stocheaza int-uri. Functia afiseaza o singura data toate valorile int
* stocate in nodurile din lista, separate printr-un spatiu, incepand de la
* nodul dat ca parametru si continuand la dreapta in lista dublu inlantuita
* circulara, pana cand sunt afisate valorile tuturor nodurilor.
*/
// void
// dll_print_ints_right_circular(dll_node_t* start)
// {
// 	// Lista nu este circulara.

// 	printf("\n");
// }

/*
* Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
* ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
* din lista separate printr-un spatiu, incepand de la primul nod din lista si
* continuand in dreapta.
*/
void
dll_print_int_list(doubly_linked_list_t* list)
{
	dll_node_t *it = list->head;

	while (it) {
		printf("%d ", *((int *)it->data));

		it = it->next;
	}

	printf("\n");
}

/*
* Aceasta functie pune un nod existent la finalul listei.
*/
void
place_at_end(doubly_linked_list_t* list, dll_node_t* node)
{
	if (!list || !list->head || !node)
		return;

	dll_node_t *end = dll_get_nth_node(list, list->size);

	if (node == end) {
		return;
	}

	if (node == list->head) {
		list->head = node->next;

		node->next->prev = NULL;

		end->next = node;

		node->prev = end;

		node->next = NULL;

		return;
	}

	node->prev->next = node->next;

	node->next->prev = node->prev;

	end->next = node;

	node->prev = end;

	node->next = NULL;
}

/*
* Aceasta functie reordoneaza o lista dublu inlantuita primita ca parametru
* astfel încât toate nodurile cu valori mai mici sau egale decât X să apară
* înaintea nodurilor cu valori mai mari decât X, păstrând ordinea relativă 
* inițială a elementelor. Nu alocati noduri noi!
*/
void
reorder(doubly_linked_list_t* list, int x)
{
	if (!list || !list->head)
		return;

	unsigned int i = 0;

	dll_node_t *it = list->head; 

	while (i < list->size) {
		dll_node_t *it_next = it->next;

		if (*(int *)it->data > x)
			place_at_end(list, it);
		
		it = it_next;

		++i;
	}
}

int main()
{
	doubly_linked_list_t* list;

	while (1) {
		char command[16];
		long size, num;

		scanf("%s", command);

		if (strcmp(command, "list") == 0) {
			list = dll_create(sizeof(int));
			scanf("%ld", &size);

			long int curr_nr;
			for (int i = 0; i < size; ++i) {
				scanf("%ld", &curr_nr);
				dll_add_nth_node(list, size, &curr_nr);
			}
		}

		if (strcmp(command, "X") == 0) {
			scanf("%ld", &num);

			reorder(list, num);
			dll_print_int_list(list);
			break;
		}
	}

	dll_free(&list);

	return 0;
}
