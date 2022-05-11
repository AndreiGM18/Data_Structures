#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 64

typedef struct ll_node_t
{
    void* data;
    struct ll_node_t* next;
} ll_node_t;

typedef struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
} linked_list_t;

linked_list_t* ll_create(unsigned int data_size)
{
    /* TODO */
	linked_list_t *new_list = malloc(1 * sizeof(linked_list_t));
	if (!new_list)
		return NULL;
	new_list->head = NULL;
	new_list->data_size = data_size;
	new_list->size = 0;
	return new_list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, ignorati comanda (iesiti din functie).
 */
void ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    /* TODO */
	if (!list)
		return;
	if (!(list->head)) {
		ll_node_t *new = (ll_node_t *)malloc(sizeof(ll_node_t));
		if (!new)
			exit(-1);

		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		new->next = NULL;

		list->head = new;
		++(list->size);
		return;
	}
	if (n == 0) {
		ll_node_t *new = (ll_node_t *)malloc(sizeof(ll_node_t));
		if (!new)
			exit(-1);

		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		new->next = list->head;

		list->head = new;
		++(list->size);
		return;
	}
	ll_node_t *it = list->head;
	if (n >= list->size) {
		while (it->next && it)
			it = it->next;

		ll_node_t *new = (ll_node_t *)malloc(sizeof(ll_node_t));
		if (!new)
			exit(-1);

		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		new->next = NULL;
		it->next = new;

		++(list->size);
	} else {
		for (unsigned int i = 0; i < n - 1; ++i)
			it = it->next;

		ll_node_t *new = (ll_node_t *)malloc(sizeof(ll_node_t));
		if (!new)
			exit(-1);
		new->next = it->next;
		new->data = malloc(list->data_size);
		memcpy(new->data, new_data, list->data_size);
		it->next = new;
		++(list->size);
	}
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, ignorati comanda (iesiti din functie).
 * Functia intoarce un pointer spre acest nod proaspat eliminat din lista.
 * Este responsabilitatea apelantului sa elibereze memoria acestui nod.
 */
ll_node_t* ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    /* TODO */
	if (!list)
		return NULL;
	ll_node_t *it = list->head;
	if (n == 0) {
		list->head = it->next;
		--(list->size);
		return it;
	}
	if (list->size == 1) {
		list->head = NULL;
		--(list->size);
		return it;
	}
	if (n == 1 || list->size == 2) {
		ll_node_t *tmp = it->next;
		it->next = tmp->next;
		--(list->size);
		return tmp;
	}
	if (n >= list->size - 1) {
		for (unsigned int i = 0; i < list->size - 2; ++i)
			it = it->next;

		ll_node_t *tmp = it->next;

		it->next = NULL;

		--(list->size);

		return tmp;
	} else {
		for (unsigned int i = 0; i < n - 1; ++i)
			it = it->next;

		ll_node_t *tmp = it->next;

		it->next = tmp->next;

		--(list->size);

		return tmp;
	}
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int ll_get_size(linked_list_t* list)
{
    /* TODO */
	return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t** pp_list)
{
    /* TODO */
	ll_node_t *it = (*pp_list)->head;
	ll_node_t *tmp = it->next;
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
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t* list)
{
    /* TODO */
	if (!list)
		exit(-1);
	ll_node_t *it = list->head;
	printf("%d ", *((int *)it->data));

	while(it && it->next) {
		it = it->next;
		printf("%d ", *((int *)it->data));
	}
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t* list)
{
    /* TODO */
	if (!list || !list->head)
		return;
	ll_node_t *it = list->head;
	printf("%s ", (char *)it->data);

	while(it && it->next) {
		it = it->next;
		printf("%s ", (char *)it->data);
	}
    printf("\n");
}

int main()
{
    linked_list_t* linkedList;
    int is_int = 0;
    int is_string = 0;

    while (1) {
        char command[16], added_elem[64], *end_ptr;
        long nr, pos;

        scanf("%s", command);

        if (strcmp(command, "create_str") == 0) {
            linkedList = ll_create(MAX_STRING_SIZE);
            is_string = 1;
        }

        if (strcmp(command, "create_int") == 0) {
            linkedList = ll_create(sizeof(int));
            is_int = 1;
        }

        if (strcmp(command, "add") == 0) {
            scanf("%ld", &pos);
            scanf("%s", added_elem);

            nr = strtol(added_elem, &end_ptr, 10);
            if (nr != 0) {
                ll_add_nth_node(linkedList, pos, &nr);
            } else {
                ll_add_nth_node(linkedList, pos, end_ptr);
            }
        }

        if (strcmp(command, "remove") == 0) {
            scanf("%ld", &pos);
            ll_node_t *removed = ll_remove_nth_node(linkedList, pos);
            free(removed->data);
            free(removed);
        }

        if (strcmp(command, "print") == 0) {
            if (is_int == 1) {
                ll_print_int(linkedList);
            }

            if (is_string == 1) {
                ll_print_string(linkedList);
            }
        }

        if (strcmp(command, "free") == 0) {
            ll_free(&linkedList);
            break;
        }
    }

    return 0;
}
