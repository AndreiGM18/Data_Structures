#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 15

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
* (Setare valori initiale pentru campurile specifice structurii LinkedList).
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
* afla pe pozitia n=0). Daca n >= nr_noduri, atunci se intoarce nodul de pe
* pozitia rezultata daca am "cicla" (posibil de mai multe ori) pe lista si am
* trece de la ultimul nod, inapoi la primul si am continua de acolo. Cum putem
* afla pozitia dorita fara sa simulam intreaga parcurgere?
*/
dll_node_t*
dll_get_nth_node(doubly_linked_list_t* list, unsigned int n)
{
	if (!list || !list->head)
		return NULL;

	n = n % list->size;
	dll_node_t *it = list->head;

	if (n <= list->size/2)
		for (unsigned int i = 0; i < n; ++i)
			it = it->next;
	else
		for (unsigned int i = 0; i < (list->size - n); ++i)
			it = it->prev;

	return it;
}

/*
* Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
* adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
* lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
* pozitia n=0). Cand indexam pozitiile nu "ciclam" pe lista circulara ca la
* get, ci consideram nodurile in ordinea de la head la ultimul (adica acel nod
* care pointeaza la head ca nod urmator in lista). Daca n >= nr_noduri, atunci
* adaugam nodul nou la finalul listei.
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

		new->next = new;

		new->prev = new;

		++(list->size);

		return;
	}

	if (n == 0) {
		new->prev = list->head->prev;

		list->head->prev->next = new;

		list->head->prev = new;

		new->next = list->head;

		list->head = new;

		++(list->size);

		return;
	}

	dll_node_t *it;

	if (n < list->size)
		it = dll_get_nth_node(list, n);
	else {
		it = list->head;
	}

	new->prev = it->prev;

	it->prev->next = new;

	it->prev = new;

	new->next = it;

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

	dll_node_t *it;

	if (n < list->size - 1)
		it = dll_get_nth_node(list, n);
	else
		it = list->head->prev;

	it->prev->next = it->next;

	it->next->prev = it->prev;

	if (n == 0)
		list->head = it->next;

	--(list->size);

	return it;
}

/*
* Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
* parametru.
*/
unsigned int
dll_get_size(doubly_linked_list_t* list)
{	if (!list)
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
	it->prev->next = NULL;
	dll_node_t *tmp = it->next;

	free(it->data);
	free(it);

	while (tmp) {
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
void
dll_print_ints_right_circular(dll_node_t* start)
{
	if (!start) {
		return;
	}

	printf("%d ", *((int *)start->data));

	dll_node_t *it = start->next;

	while (it != start) {
		printf("%d ", *((int *)it->data));

		it = it->next;
	}

	printf("\n");
}

/*
* Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
* ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
* din lista separate printr-un spatiu, incepand de la primul nod din lista.
*/
void
dll_print_int_list(doubly_linked_list_t* list)
{
	if (!list || !list->head)
		return;

	printf("%d ", *((int *)list->head->data));

	dll_node_t *it = list->head->next;

	while (it != list->head) {
		printf("%d ", *((int *)it->data));

		it = it->next;
	}

	printf("\n");
}

/*
* Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
* ca stocheaza string-uri. Functia afiseaza o singura data toate valorile string
* stocate in nodurile din lista, separate printr-un spatiu, incepand de la
* nodul dat ca parametru si continuand la stanga in lista dublu inlantuita
* circulara, pana cand sunt afisate valorile tuturor nodurilor.
*/
void
dll_print_strings_left_circular(dll_node_t* start)
{
	if (!start)
		return;

	printf("%s ", (char *)start->data);

	dll_node_t *it = start->prev;

	while (it != start) {
		printf("%s ", (char *)it->data);

		it = it->prev;
	}

	printf("\n");
}

/*
* Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
* ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
* nodurile din lista separate printr-un spatiu, incepand de la primul nod din
* lista.
*/
void
dll_print_string_list(doubly_linked_list_t* list)
{
	if (!list || !list->head)
		return;

	printf("%s ", (char *)list->head->prev->data);

	dll_node_t *it = list->head->prev->prev;

	while (it != list->head->prev) {
		printf("%s ", (char *)it->data);

		it = it->prev;
	}

	printf("\n");
}

void add_deck(doubly_linked_list_t *list_packets, int n)
{
	doubly_linked_list_t *packet = dll_create(MAX_STRING_SIZE);
	
	while (n) {
		
		--n;
	}
	
	
	dll_add_nth_node(list_packets, list_packets->size, packet);

}

int main() {
	doubly_linked_list_t *doublyLinkedList = dll_create(sizeof(doubly_linked_list_t *));
	int is_int = 0;
	int is_string = 0;
	while(1) {
		char command[16], added_elem[64], *end_ptr;
		long nr, pos;
		scanf("%s", command);
		if(strcmp(command, "create_str") == 0){
			doublyLinkedList = dll_create(MAX_STRING_SIZE);
			is_string = 1;
		}
		if(strcmp(command, "create_int") == 0){
			doublyLinkedList = dll_create(sizeof(int));
			is_int = 1;
		}
		if(strcmp(command, "add") == 0){
			scanf("%ld", &pos);
			scanf("%s", added_elem);
			nr = strtol(added_elem, &end_ptr, 10);
			if(nr != 0) {
				dll_add_nth_node(doublyLinkedList, pos, &nr);
			}
			else{
				dll_add_nth_node(doublyLinkedList, pos, end_ptr);
			}
		}
		if(strcmp(command, "remove") == 0){
			scanf("%ld", &pos);
			dll_node_t* removed = dll_remove_nth_node(doublyLinkedList, pos);
			free(removed->data);
			free(removed);
		}
		if(strcmp(command, "print") == 0){
			if(is_int == 1){
				dll_print_int_list(doublyLinkedList);
			}
			if(is_string == 1){
				dll_print_string_list(doublyLinkedList);
			}
		}
		if(strcmp(command, "free") == 0){
			dll_free(&doublyLinkedList);
			break;
		}
	}
	return 0;
}
