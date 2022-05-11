// Copyright 2022 Mitran Andrei-Gabriel

#ifndef DLL_FUNC_H_
#define DLL_FUNC_H_

#include "utils.h"

dll_t *
dll_create(unsigned int data_size);

dll_node_t*
dll_get_nth_node(dll_t* list, unsigned int n);

void
dll_add_nth_node(dll_t* list, unsigned int n, const void* new_data);

dll_node_t *
dll_remove_nth_node(dll_t* list, unsigned int n);

unsigned int
dll_get_size(dll_t* list);

void
dll_free(dll_t** pp_list);

#endif  // DLL_FUNC_H_
