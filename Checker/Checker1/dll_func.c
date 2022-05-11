// Copyright 2022 Mitran Andrei-Gabriel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/*
* Returns a pointer to the newly created list.
* Also initializes the new list's fields.
* The first node is considered to be node 0.
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
* Returns a pointer to the nth node of the list.
* If it is called with an n greater than the final node's index,
* it returns the a pointer to the final node.
*/
dll_node_t*
dll_get_nth_node(dll_t* list, uint n)
{
	if (!list || !list->head)
		return NULL;

	/*
	* If n is greater than the final's node index,
	* n becomes the final node's index.
	*/
	if (n >= list->size)
		n = list->size - 1;

	/*
	* Starts at the list's head.
	*/
	dll_node_t *it = list->head;

	/*
	* Goes along the list until you get to the nth node.
	*/
	while (n) {
		it = it->next;
		--n;
	}

	return it;
}

/*
* Creates a new node based on the information provided by data.
* The node is added to the nth position in the list.
* If it is called with an n greater than the final node's index,
* the newly created node is added at the end.
*/
void
dll_add_nth_node(dll_t* list, uint n, const void* data)
{
	if (!list)
		return;

	/*
	* Creates the new node.
	*/
	dll_node_t *new = (dll_node_t *)malloc(sizeof(dll_node_t));
	DIE(!new, "new malloc failed");

	/*
	* Adds its data.
	*/
	new->data = malloc(list->data_size);
	DIE(!new->data, "new->data malloc failed");
	memcpy(new->data, data, list->data_size);

	/*
	* If the list is empty, simply makes the list's head this new node.
	* It also makes the node's 'next' and 'prev' pointers NULL.
	*/
	if (!(list->head)) {
		list->head = new;

		new->next = NULL;

		new->prev = NULL;

		++(list->size);

		return;
	}

	/*
	* If the list is not empty, but the node is to be added at the
	* beginning of the list, makes it the new head of the list.
	* As such, its 'prev' pointer is NULL.
	*/
	if (!n) {
		new->prev = NULL;

		new->next = list->head;

		list->head->prev = new;

		list->head = new;

		++(list->size);

		return;
	}

	/*
	* Gets a pointer to the nth node.
	*/
	dll_node_t *it = dll_get_nth_node(list, n);

	/*
	* If the node is to be added at the end, its 'next' pointer is NULL.
	* Otherwise, it is added at the nth position.
	*/
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

	/*
	* Adds 1 to the list's size.
	*/
	++(list->size);
}

/*
* Removes the nth node from the list and returns a pointer to it.
* The node still exists, it is simply removed from the list.
* The memory allocated to this node must be freed outside this function.
* If it is called with an n greater than the final node's index, it
* removes the final node.
*/
dll_node_t*
dll_remove_nth_node(dll_t* list, uint n)
{
	if (!list || !list->head)
		return NULL;

	/*
	* If the list is made up of only 1 node,
	* it makes the 'head' pointer NULL.
	*/
	if (list->size == 1) {
		dll_node_t *start = list->head;

		list->head = NULL;

		--(list->size);

		return start;
	}

	/*
	* If the node to be removed is from the beginning of the list,
	* and the list has more than 1 node, it makes the node after it 
	* the new head of the list.
	* As such, the next node's 'prev' pointer becomes NULL.
	*/
	if (!n) {
		dll_node_t *start = list->head;

		start->next->prev = NULL;

		list->head = start->next;

		--(list->size);

		return start;
	}

	/*
	* Gets a pointer to the nth node.
	*/
	dll_node_t *it = dll_get_nth_node(list, n);

	/*
	* If the node is to be removed from end, the previous node's
	* 'next' pointer becomes NULL.
	* Otherwise, it is removed from the nth position.
	*/
	if (n >= list->size - 1) {
		it->prev->next = NULL;
	} else {
		it->prev->next = it->next;

		it->next->prev = it->prev;
	}

	/*
	* Subtracts 1 from the list's size.
	*/
	--(list->size);

	/*
	* Returns a pointer to the removed node.
	*/
	return it;
}

/*
* Frees the memory allocated for the list and all of its nodes.
*/
void
dll_free(dll_t** pp_list)
{
	if (!(*pp_list))
		return;

	/*
	* If the list is empty, it directly frees the memory
	* allocated for the list structure itself.
	*/
	if (!(*pp_list)->head) {
		free(*pp_list);
		*pp_list = NULL;
		return;
	}

	/*
	* Gets a pointer to the list's head, as well as to the
	* following node.
	*/
	dll_node_t *it = (*pp_list)->head;
	dll_node_t *tmp = it->next;

	/*
	* Frees the first node.
	*/
	free(it->data);
	free(it);

	/*
	* Frees the rest of the nodes (if they exist).
	*/
	while(tmp) {
		it = tmp;
		tmp = it->next;

		free(it->data);
		free(it);
	}

	/*
	* Frees the memory allocated for the list structure itself.
	*/
	free(*pp_list);
	*pp_list = NULL;
}

