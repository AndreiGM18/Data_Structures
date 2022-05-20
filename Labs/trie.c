/*
	SD 2022 - Trie
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define ALPHABET "abcdefghijklmnopqrstuvwxyz"

typedef struct trie_node_t trie_node_t;
struct trie_node_t {
	/* Value associated with key (set if end_of_word = 1) */
	void* value;

	/* 1 if current node marks the end of a word, 0 otherwise */
	int end_of_word;

	trie_node_t** children;
	int n_children;
};

typedef struct trie_t trie_t;
struct trie_t {
	trie_node_t* root;
	
	/* Number of keys */
	int size;

	/* Generic Data Structure */
	int data_size;

	/* Trie-Specific, alphabet properties */
	int alphabet_size;
	char* alphabet;

	/* Callback to free value associated with key, should be called when freeing */
	void (*free_value_cb)(void*);

	/* Optional - number of nodes, useful to test correctness */
	int nNodes;
};

trie_node_t* trie_create_node(trie_t * trie) {
	// TODO
	if (!trie)
		return NULL;

	trie_node_t *node = (trie_node_t *)malloc(sizeof(*node));
	if (!node) {
		fprintf(stderr, "node malloc failed\n");
		exit(-1);
	}

	node->children = (trie_node_t **)malloc(ALPHABET_SIZE * sizeof(trie_node_t *));
	if (!node->children) {
		fprintf(stderr, "node->children malloc failed\n");
		exit(-1);
	}
	for (int i = 0; i < ALPHABET_SIZE; ++i)
		node->children[i] = NULL;

	node->end_of_word = 0;
	node->n_children = 0;

	node->value = malloc(trie->data_size);
	if (!node->value) {
		fprintf(stderr, "node->value malloc failed\n");
		exit(-1);
	}

	return node;
}

trie_t* trie_create(int data_size, int alphabet_size, char* alphabet, void (*free_value_cb)(void*)) {
	// TODO
	if (!alphabet)
		return NULL;

	trie_t *trie = (trie_t *)malloc(sizeof(*trie));
	if (!trie) {
		fprintf(stderr, "trie malloc failed\n");
		exit(-1);
	}

	trie->alphabet = (char *)malloc((alphabet_size + 1) * sizeof(char));
	if (!trie->alphabet) {
		fprintf(stderr, "trie->alphabet malloc failed\n");
		exit(-1);
	}

	memcpy(trie->alphabet, ALPHABET, alphabet_size + 1);
	trie->alphabet_size = alphabet_size;
	trie->data_size = data_size;
	trie->free_value_cb = free_value_cb;
	trie->nNodes = 1;
	trie->root = trie_create_node(trie);
	*(int *)trie->root->value = -1;
	trie->size = 0;

	return trie;
}

void trie_insert(trie_t* trie, char* key, void* value) {
	// TODO
	if (!trie || !key || !value)
		return;

	trie_node_t *curr = trie->root;

	for (int cnt = 0; cnt < (int)strlen(key); ++cnt)
	{
		int index = key[cnt] - 'a';
		if (!curr->children[index]) {
			curr->children[index] = trie_create_node(trie);
			++(curr->n_children);
			++(trie->nNodes);		
		}

		curr = curr->children[index];
	}

	curr->end_of_word = 1;
	memcpy(curr->value, value, trie->data_size);
	++(trie->size);
}

void* trie_search(trie_t* trie, char* key) {
	// TODO
	if (!trie || !key)
		return NULL;

	trie_node_t *curr = trie->root;

	if (key[0] == '\0')
		return trie->root->value;

	for (int cnt = 0; cnt < (int)strlen(key); ++cnt)
	{
		int index = key[cnt] - 'a';

		if (!curr->children[index])
			return NULL;

		curr = curr->children[index];
	}

	if (curr->end_of_word)
		return curr->value;
	else
		return NULL;
}

void trie_free_node(trie_t *trie, trie_node_t *node,
	trie_node_t *prev_node, int index) {
	trie->free_value_cb(node->value);
	free(node->children);
	free(node);
	--(trie->nNodes);
	if (prev_node) {
		prev_node->children[index] = NULL;
		--(prev_node->n_children);
	}
}

void trie_remove(trie_t* trie, char* key) {
    // TODO
	if (!trie || !key)
		return;
	
	trie_node_t *prev_node;

	trie_node_t *node = trie->root;
	for (int cnt = 0; cnt < (int)strlen(key); ++cnt) {
		int index = key[cnt] - 'a';
		trie_node_t *next_node = node->children[index];

		if (!next_node)
			return;

		prev_node = node;
		node = next_node;
	}

	if (!node->end_of_word)
		return;

	if (node->n_children) {
		node->end_of_word = 0;
		return;
	}

	trie_free_node(trie, node, prev_node, key[(int)strlen(key) - 1] - 'a');
	--(trie->size);

	while (1) {
		int i;
		trie_node_t *node = trie->root;
		prev_node = node;

		for (int cnt = 0; cnt < (int)strlen(key); ++cnt) {
			i = cnt;
			int index = key[cnt] - 'a';
			trie_node_t *next_node = node->children[index];

			if (!next_node)
				break;

			prev_node = node;
			node = next_node;
		}

		if (node->end_of_word || node->n_children || node == trie->root)
			return;
		else
			trie_free_node(trie, node, prev_node, key[i - 1] - 'a');
	}
}

void __trie_free(trie_t *pTrie, trie_node_t **pNode)
{
	if (!pTrie || !pNode || !*pNode)
		return;

	for (int i = 'a'; i <= 'z'; ++i)
		__trie_free(pTrie, &(*pNode)->children[i - 'a']);

	trie_free_node(pTrie, *pNode, NULL, -1);
}

void trie_free(trie_t** pTrie) {
	if (!pTrie)
		return;

	__trie_free(*pTrie, &(*pTrie)->root);
	free((*pTrie)->alphabet);
	free(*pTrie);
}  

/* Needed for Lambda tests, ignore :) */
void cleanup_example_string(char* str) {
	int len = (int)strlen(str);
	
	if(str[len-2] == '\\') {
		str[len-2] = '\0';
	}
}

int main() {
	int n, value;
	char alphabet[] = ALPHABET;
	char buf[256], key[256], op;
	
	trie_t* trie = trie_create(sizeof(int), ALPHABET_SIZE, alphabet, free);
	
	fgets(buf, 256, stdin);
	sscanf(buf, "%d\n", &n);

	for(int i = 0; i < n; ++i) {
		fgets(buf, 256, stdin);
		sscanf(buf, "%c", &op);

		if(op == 'i') {
			sscanf(buf, "%c %s %d\n", &op, key, &value);
			trie_insert(trie, key, &value);
		} else if(op == 'r') {
			sscanf(buf, "%c %s\n", &op, key);
			cleanup_example_string(key);
			printf("nNodes before removing %s: %d\n", key, trie->nNodes);
			trie_remove(trie, key);
			printf("nNodes after removing %s: %d\n", key, trie->nNodes);
		} else if(op == 's') {
			sscanf(buf, "%c %s\n", &op, key);
			cleanup_example_string(key);

			if(key[0] == '_') {
				key[0] = '\0';
			}

			int* found = trie_search(trie, key);
			printf("%s: ", key[0] == '\0' ? "_" : key);

			if(found) {
				printf("%d\n", *found);
			} else {
				printf("not found\n");
			}
		}
	}

	trie_free(&trie);

	return 0;
}
