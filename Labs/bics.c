#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_SIZE 64

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
    /* TODO */
    doubly_linked_list_t *dll;
    
    dll = malloc(1 * sizeof(dll));
    if(!dll) {
        return NULL;
    }
    
    dll->head = NULL;
    dll->size = 0;
    dll->data_size = data_size;

    return dll;
    
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
    /* TODO */
    dll_node_t *dll_node;
    unsigned int count;
    
    if(!list || n < 0) {
        return NULL;
    }
    
    dll_node = list->head;
    
    n %= list->size;
    
    for(count = 0; count  < n; ++count);
    dll_node = dll_node->next;
    
    return dll_node;
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
    /* TODO */
    dll_node_t *newdll_node, *auxdll_node = list->head;
    if(list->head == NULL) {
        newdll_node = (dll_node_t*)malloc(sizeof(dll_node_t));

        newdll_node->data = malloc(list->data_size);

        memcpy(newdll_node->data, data, list->data_size);

        newdll_node->next = NULL;
        list->head = newdll_node;
        ++(list->size);
        return;
    }
    if (n == 0) {
        newdll_node = (dll_node_t*)malloc(1 * sizeof(dll_node_t));
        
        newdll_node->data = malloc(list->data_size);
        
        memcpy(newdll_node->data, data, list->data_size);
        
        newdll_node->next = list->head;
        
        list->head = newdll_node;
        
        ++(list->size);
        return;
    }

    if(!list) {
        return;
    } else if(n >= list->size) {
        newdll_node = (dll_node_t*)malloc(sizeof(dll_node_t));

        while(auxdll_node->next && auxdll_node) {
            auxdll_node = auxdll_node->next;
        }
        newdll_node->data = malloc(list->data_size);

        memcpy(newdll_node->data, data, list->data_size);

        newdll_node->next = NULL;
        auxdll_node->next = newdll_node;

        ++(list->size);
    } else {
        for(unsigned int count = 0; count < n - 1; ++count) {
            auxdll_node = auxdll_node->next;
        }

        newdll_node = (dll_node_t*)malloc(sizeof(dll_node_t));

        newdll_node->data = malloc(list->data_size);

        newdll_node->next = auxdll_node->next;

         memcpy(newdll_node->data, data, list->data_size);

         auxdll_node->next = newdll_node;

         ++(list->size);
    }
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
    /* TODO */
    if(!list) {
        return NULL;
    }

    dll_node_t *auxdll_node = list->head;
    if(n == 0) {
        list->head = auxdll_node->next;
        --(list->size);
        return auxdll_node;
    }
    if(list->size == 1) {
        list->head = NULL;
        --(list->size);
        return auxdll_node;
    }
    if(n == 1 || list->size == 2) {
        dll_node_t *auxdll2_node = auxdll_node->next;
        auxdll_node->next = auxdll2_node->next;
        --(list->size);
        return auxdll2_node;
    }
    if(n >= list->size - 1){
        for(unsigned int i = 0; i < list->size - 2; ++i) {
            auxdll_node = auxdll_node->next;
        }
        dll_node_t *auxdll2_node = auxdll_node->next;
        auxdll_node->next = NULL;
        --(list->size);
        return auxdll2_node;
    } else {
        for(unsigned int i = 0; i < n - 1; ++i) {
            auxdll_node = auxdll_node->next;
        }
        dll_node_t *auxdll2_node = auxdll_node->next;
        auxdll_node->next = auxdll2_node->next;
        --(list->size);
        return auxdll2_node;
        
}
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
dll_get_size(doubly_linked_list_t* list)
{
    /* TODO */
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista.
 */
void
dll_free(doubly_linked_list_t** pp_list)
{
    /* TODO */
    dll_node_t *auxdll_node = (*pp_list)->head;
    dll_node_t *aux2dll_node = auxdll_node->next;

    free(auxdll_node->data);
    free(auxdll_node);

    while(aux2dll_node) {
        auxdll_node = aux2dll_node;
        aux2dll_node = auxdll_node->next;
        free(auxdll_node->data);
        free(auxdll_node);
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
    /* TODO */
    dll_node_t *curr;

    curr = start;
    printf("%d ", *(int*)curr->data);

    while(curr->next != start){
        curr = curr->next;
        printf("%d ", *(int*)curr->data);
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
    /* TODO */
    dll_node_t *curr;

    if(list == NULL) {
        return;
    }

    curr = list->head;
    printf("%d ", *(int *)curr->data);
    while(curr != NULL && curr->next) {
        curr = curr->next;
        printf("%d ", *(int *)curr->data);
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
    /* TODO */
    dll_node_t *curr;

    curr = start;
    printf("%s ", (char *)curr->data);

    while(curr->prev != start) {
        curr = curr->prev;
        printf("%s ", (char *)curr->data);
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
    /* TODO */
    dll_node_t *curr;

    if(list == NULL) {
        return;
    }

    curr = list->head->prev;
    printf("%s ", (char *)curr->data);
    while(curr->prev != list->head->prev) {
        curr = curr->prev;
        printf("%s ", (char *)curr->data);
    }
    printf("%s ", (char *)curr->data);
    printf("\n");
}

int main() {
    doubly_linked_list_t *doublyLinkedList;
    int is_int = 0;
    int is_string = 0;
    while(1) {
        char command[16], added_elem[MAX_STRING_SIZE], *end_ptr;
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

            if(is_int) {
                scanf("%ld", &nr);
                dll_add_nth_node(doublyLinkedList, pos, &nr);
            } else if(is_string) {
                scanf("%s", added_elem);
                dll_add_nth_node(doublyLinkedList, pos, added_elem);
            } else {
                printf("Create a list before adding elements!\n");
                exit(0);
            }
        }
        if(strcmp(command, "remove") == 0){
            if(!is_int && !is_string) {
                printf("Create a list before removing elements!\n");
                exit(0);
            }

            scanf("%ld", &pos);
            dll_node_t* removed = dll_remove_nth_node(doublyLinkedList, pos);
            free(removed->data);
            free(removed);
        }
        if(strcmp(command, "print") == 0){
            if(!is_int && !is_string) {
                printf("Create a list before printing!\n");
                exit(0);
            }

            if(is_int == 1){
                dll_print_int_list(doublyLinkedList);
            }
            if(is_string == 1){
                dll_print_string_list(doublyLinkedList);
            }
        }
        if(strcmp(command, "free") == 0){
            if(!is_int && !is_string) {
                printf("Create a list before freeing!\n");
                exit(0);
            }
            dll_free(&doublyLinkedList);
            break;
        }
    }
    return 0;
}