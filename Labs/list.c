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
	linked_list_t *new_list = malloc(sizeof(linked_list_t));
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
	if (!(list->head) || n == 0) {
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
	ll_node_t *it = list->head;
	if (n >= list->size) {
		while (it->next)
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
	if (list->data_size == sizeof(int)) {
		ll_node_t *it = list->head;
		int x = *(int *)it->data;
		printf("%d ", x);

		while(it->next) {
			it = it->next;
			x = *(int *)it->data;
			printf("%d ", x);
		}
	}
    printf("\n");
}

/*
 * Procedura primeste doua liste simplu inlantuite, A si B, ca parametru si
 * returneaza o nouă listă simplu înlănțuită, C, pentru care fiecare nod i este
 * suma nodurilor asociate din A și B. Mai exact, nodul i din C reține
 * suma dintre valoarea nodului i din A și valoarea nodului i din B. 
 * Dacă una dintre listele primite este mai lungă decât cealaltă, se consideră
 * că nodurile asociate lipsă din cealaltă listă conțin valoarea 0, 
 * adică se păstrează valorile din lista mai lungă.
 */
linked_list_t* sum_lists(linked_list_t* list1, linked_list_t* list2)
{
    /* TODO */
	if (list1->data_size != 4 || list2->data_size != 4)
		return NULL;

	int size;
	linked_list_t *list_sum = ll_create(sizeof(int));
	if (list1->size > list2->size)
		size = list1->size;
	else
		size = list2->size;
	ll_node_t *it1 = list1->head;
	ll_node_t *it2 = list2->head;
	int sum = (*(int *)it1->data) + (*(int *)it2->data);
	ll_add_nth_node(list_sum, 0, &sum);
	for (int i = 1; i < size; ++i) {
		if (it1 && it1->next)
			it1 = it1->next;
		else
			it1 = NULL;
		if (it2 && it2->next)
			it2 = it2->next;
		else
			it2 = NULL;
		if (it1 && it2) {
			sum = (*(int *)it1->data) + (*(int *)it2->data);
			ll_add_nth_node(list_sum, i, &sum);
		} else if (it1 && !it2) {
			ll_add_nth_node(list_sum, i, it1->data);
		} else if (!it1 && it2) {
			ll_add_nth_node(list_sum, i, it2->data);
		}
	}
	return list_sum;
}

int main()
{
    linked_list_t* linked_list1;
    linked_list_t* linked_list2;
    linked_list_t* linked_list_sum;

    while (1) {
        char command[16];
        long size;

        scanf("%s", command);

        if (strcmp(command, "list1") == 0) {
            linked_list1 = ll_create(sizeof(int));
            scanf("%ld", &size);

            int curr_nr;
            for (unsigned int i = 0; i < size; ++i) {
                scanf("%d", &curr_nr);
                ll_add_nth_node(linked_list1, i, &curr_nr);
            }
        }

        if (strcmp(command, "list2") == 0) {
            linked_list2 = ll_create(sizeof(int));
            scanf("%ld", &size);

            int curr_nr;
            for (unsigned int i = 0; i < size; ++i) {
                scanf("%d", &curr_nr);
                ll_add_nth_node(linked_list2, i, &curr_nr);
            }

            break;
        }
    }

    linked_list_sum = sum_lists(linked_list1, linked_list2);

    ll_print_int(linked_list_sum);

    ll_free(&linked_list_sum);
    ll_free(&linked_list1);
    ll_free(&linked_list2);

    return 0;
}
  