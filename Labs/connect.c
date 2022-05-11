#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ", __FILE__,		\
				__LINE__);				\
			perror(call_description);			\
			exit(errno);					\
		}							\
	} while (0)

/* Helper data structures definitions */
typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
	void* data;
	ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
	ll_node_t* head;
	unsigned int data_size;
	unsigned int size;
};

typedef struct stack_t stack_t;
struct stack_t
{
	struct linked_list_t* list;
};

typedef struct queue_t queue_t;
struct queue_t
{
	unsigned int max_size;
	unsigned int size;
	unsigned int data_size;
	unsigned int read_idx;
	unsigned int write_idx;
	void **buff;
};

typedef struct
{
	linked_list_t** neighbors;
	int nodes;
} list_graph_t;

/* Helper data structures functions */
linked_list_t*
ll_create(unsigned int data_size)
{
	linked_list_t* ll = calloc(1, sizeof(*ll));
	DIE(!ll, "calloc list");

	ll->data_size = data_size;

	return ll;
}

static ll_node_t*
get_nth_node(linked_list_t* list, unsigned int n)
{
	unsigned int len = list->size - 1;
	unsigned int i;
	ll_node_t* node = list->head;

	n = MIN(n, len);

	for (i = 0; i < n; ++i)
		node = node->next;

	return node;
}

static ll_node_t*
create_node(const void* new_data, unsigned int data_size)
{
	ll_node_t* node = calloc(1, sizeof(*node));
	DIE(!node, "calloc node");

	node->data = malloc(data_size);
	DIE(!node->data, "malloc data");

	memcpy(node->data, new_data, data_size);

	return node;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
	ll_node_t *new_node, *prev_node;

	if (!list)
		return;

	new_node = create_node(new_data, list->data_size);

	if (!n || !list->size) {
		new_node->next = list->head;
		list->head = new_node;
	} else {
		prev_node = get_nth_node(list, n - 1);
		new_node->next = prev_node->next;
		prev_node->next = new_node;
	}

	++list->size;
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
	ll_node_t *prev_node, *removed_node;

	if (!list || !list->size)
		return NULL;

	if (!n) {
		removed_node = list->head;
		list->head = removed_node->next;
		removed_node->next = NULL;
	} else {
		prev_node = get_nth_node(list, n - 1);
		removed_node = prev_node->next;
		prev_node->next = removed_node->next;
		removed_node->next = NULL;
	}

	--list->size;

	return removed_node;
}

unsigned int
ll_get_size(linked_list_t* list)
{
	return !list ? 0 : list->size;
}

void
ll_free(linked_list_t** pp_list)
{
	ll_node_t* node;

	if (!pp_list || !*pp_list)
		return;

	while ((*pp_list)->size) {
		node = ll_remove_nth_node(*pp_list, 0);
		free(node->data);
		free(node);
	}

	free(*pp_list);
	*pp_list = NULL;
}

void
ll_print_int(linked_list_t* list)
{
	ll_node_t* node = list->head;

	for (; node; node = node->next)
		printf("%d ", *(int*)node->data);
	printf("\n");
}

void
ll_print_string(linked_list_t* list)
{
	ll_node_t* node = list->head;

	for (; node; node = node->next)
		printf("%s ", (char*)node->data);
	printf("\n");
}

stack_t*
st_create(unsigned int data_size)
{
	stack_t* st = malloc(sizeof(*st));
	DIE(!st, "malloc st failed");
	st->list = ll_create(data_size);
	DIE(!st->list, "malloc list failed");

	return st;
}

unsigned int
st_get_size(stack_t* st)
{
	if (!st || !st->list)
		return 0;
	return st->list->size;
}

unsigned int
st_is_empty(stack_t* st)
{
	return !st || !st->list || !st->list->size;
}

void*
st_peek(stack_t* st)
{
	if (!st || !st->list || !st->list->size)
		return NULL;

	return st->list->head->data;
}

void
st_pop(stack_t* st)
{
	ll_node_t *node;

	if (!st || !st->list)
		return;

	node = ll_remove_nth_node(st->list, 0);
	free(node->data);
	free(node);
}

void
st_push(stack_t* st, void* new_data)
{
    if (!st || !st->list)
        return;

    ll_add_nth_node(st->list, 0, new_data);
}

void
st_clear(stack_t* st)
{
	if (!st || !st->list)
		return;

	ll_free(&st->list);
}

void
st_free(stack_t* st)
{
	if (!st || !st->list)
		return;

	ll_free(&st->list);
	free(st);
}

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = calloc(1, sizeof(*q));
	DIE(!q, "calloc queue failed");

	q->data_size = data_size;
	q->max_size = max_size;

	q->buff = malloc(max_size * sizeof(*q->buff));
	DIE(!q->buff, "malloc buffer failed");

	return q;
}

unsigned int
q_get_size(queue_t *q)
{
	return !q ? 0 : q->size;
}

unsigned int
q_is_empty(queue_t *q)
{
	return !q ? 1 : !q->size;
}

void *
q_front(queue_t *q)
{
	if (!q || !q->size)
		return NULL;

	return q->buff[q->read_idx];
}

int
q_dequeue(queue_t *q)
{
	if (!q || !q->size)
		return 0;

	free(q->buff[q->read_idx]);

	q->read_idx = (q->read_idx + 1) % q->max_size;
	--q->size;
	return 1;
}

int
q_enqueue(queue_t *q, void *new_data)
{
	void *data;
	if (!q || q->size == q->max_size)
		return 0;

	data = malloc(q->data_size);
	DIE(!data, "malloc data failed");
	memcpy(data, new_data, q->data_size);

	q->buff[q->write_idx] = data;
	q->write_idx = (q->write_idx + 1) % q->max_size;
	++q->size;

	return 1;
}

void
q_clear(queue_t *q)
{
	unsigned int i;
	if (!q || !q->size)
		return;

	for (i = q->read_idx; i != q->write_idx; i = (i + 1) % q->max_size)
		free(q->buff[i]);

	q->read_idx = 0;
	q->write_idx = 0;
	q->size = 0;
}

void
q_free(queue_t *q)
{
	if (!q)
		return;

	q_clear(q);
	free(q->buff);
	free(q);
}


static int is_node_in_graph(int n, int nodes)
{
	return n >= 0 && n < nodes;
}

list_graph_t*
lg_create(int nodes)
{
	int i;

	list_graph_t *g = malloc(sizeof(*g));
	DIE(!g, "malloc graph failed");

	g->neighbors = malloc(nodes * sizeof(*g->neighbors));
	DIE(!g->neighbors, "malloc neighbours failed");

	for (i = 0; i != nodes; ++i)
		g->neighbors[i] = ll_create(sizeof(int));

	g->nodes = nodes;

	return g;
}

void
lg_add_edge(list_graph_t* graph, int src, int dest)
{
	if (
		!graph || !graph->neighbors
		|| !is_node_in_graph(src, graph->nodes)
		|| !is_node_in_graph(dest, graph->nodes)
	)
		return;

	ll_add_nth_node(graph->neighbors[src], graph->neighbors[src]->size, &dest);
}

static ll_node_t *find_node(linked_list_t *ll, int node, unsigned int *pos)
{
	ll_node_t *crt = ll->head;
	unsigned int i;

	for (i = 0; i != ll->size; ++i) {
		if (node == *(int *)crt->data) {
			*pos = i;
			return crt;
		}

		crt = crt->next;
	}

	return NULL;
}

int
lg_has_edge(list_graph_t* graph, int src, int dest)
{
	unsigned int pos;

	if (
		!graph || !graph->neighbors
		|| !is_node_in_graph(src, graph->nodes)
		|| !is_node_in_graph(dest, graph->nodes)
	)
		return 0;

	return find_node(graph->neighbors[src], dest, &pos) != NULL;
}

linked_list_t*
lg_get_neighbours(list_graph_t* graph, int node)
{
	if (
		!graph || !graph->neighbors
		|| !is_node_in_graph(node, graph->nodes)
	)
		return NULL;

	return graph->neighbors[node];
}

void
lg_remove_edge(list_graph_t* graph, int src, int dest)
{
	unsigned int pos;

	if (
		!graph || !graph->neighbors
		|| !is_node_in_graph(src, graph->nodes)
		|| !is_node_in_graph(dest, graph->nodes)
	)
		return;

	if (!find_node(graph->neighbors[src], dest, &pos))
		return;

	ll_remove_nth_node(graph->neighbors[src], pos);
}

void
lg_free(list_graph_t* graph)
{
	int i;

	for (i = 0; i != graph->nodes; ++i)
		ll_free(graph->neighbors + i);
	
	free(graph->neighbors);
	free(graph);
}

/* --- START IMPLEMENT DFS HERE --- */

/* Implementati parcurgerea DFS a unui Matrix Graph
 * Functia trebuie sa printeze un nod, imediat ce acesta este vizitat pentru prima data
 * In alegerea ordinii parcurgerii vecinilor, acestia vor fi parcursi in ordinea fireasca citirii unei linii dintr-o matrice
 * (i=(1..n))
*/

void
init_dfs(list_graph_t* lg,
                 int** state,
                 int** t_desc,
                 int** t_fin)
{
	for (int i = 0; i < lg->nodes; ++i) {
		(*state)[i] = -1;
		(*t_desc)[i] = 0;
		(*t_fin)[i] = 0;
	}
}

void
visit(int node, int** state, int** t_desc, int *time)
{
	++(*time);

	(*t_desc)[node] = *time;

	(*state)[node] = 1;
}

void
dfs_list_graph(list_graph_t* lg,
                 int node,
                 int** state,
                 int** t_desc,
                 int** t_fin,
				 linked_list_t *component)
{
	int time = 0;

	stack_t *st = st_create(sizeof(int));

	visit(node, state, t_desc, &time);
	st_push(st, &node);

	while (!st_is_empty(st)) {
		int top = *(int *)st_peek(st);
		int v = -1;
		ll_node_t *it = lg->neighbors[top]->head;
		while (it) {
			int i = *(int *)it->data;
			if ((*state)[i] == -1 && lg_has_edge(lg, top, i)) {
				v = i;
				break;
			}
			it = it->next;
		}
		if (v != -1) {
			visit(v, state, t_desc, &time);
			st_push(st, &v);
		} else {
			++time;
			(*t_fin)[top] = time;
			st_pop(st);
			ll_add_nth_node(component, component->size, &top);
		}
	}

	st_free(st);
}

/*
    Output format:
    
    N
    cc1_n1 cc1_n2 cc1_n3 ...
    cc2_n1 cc2_n2 cc2_n3 cc2_n4 ...
    .
    .
    .
    ccN_n1 ccN_n2 ccN_n3 ...

    where 

    cc1_n1 < cc2_n2 < ... < ccN_n1

    Meaning: first line is number of components, then one connected comp per line,
    nodes separated by space. Order of nodes DOES matter (use DFS and when traversing
	neighbours use the order in which they appear in the input). Print the node only
	after all its neighbours have been visited, not before.
    Order of connected comps DOES matter (start traverals in ascending order of 
	nodes, from 0 to N)
*/
void print_connected_components(list_graph_t* graph) {
	int* state = (int *)malloc(graph->nodes * sizeof(int));
	DIE(!state, "malloc state failed");
	int* t_desc = (int *)malloc(graph->nodes * sizeof(int));
	DIE(!t_desc, "malloc t_desc failed");
	int* t_fin = (int *)malloc(graph->nodes * sizeof(int));
	DIE(!t_fin, "malloc t_fin failed");

	int nr_connect = 0;

	linked_list_t** components = (linked_list_t **)malloc(graph->nodes * sizeof(linked_list_t *));
	DIE(!components, "malloc components failed");

	for (int i = 0; i != graph->nodes; ++i)
		components[i] = ll_create(sizeof(int));

	init_dfs(graph, &state, &t_desc, &t_fin);

	for (int i = 0; i < graph->nodes; ++i) {
		if (state[i] != 1) {
			dfs_list_graph(graph, i, &state, &t_desc, &t_fin, components[nr_connect]);
			++nr_connect;
		}
	}

	printf("%d\n", nr_connect);

	for (int i = 0; i < nr_connect; ++i)
		ll_print_int(components[i]);

	for (int i = 0; i != graph->nodes; ++i)
		ll_free(components + i);
	
	free(components);

	free(state);
	free(t_desc);
	free(t_fin);
}

int main() {
    int n, m, src, dest;
    list_graph_t* graph;
    char buf[256];
    
	// stupid fgets workaround for lambda
    fgets(buf, 256, stdin);
    sscanf(buf, "%d %d\n", &n, &m);

    graph = lg_create(n);

    for(int i = 0; i < m; ++i) {
        fgets(buf, 256, stdin);
        sscanf(buf,"%d %d\n", &src, &dest);
        lg_add_edge(graph, src, dest);
        lg_add_edge(graph, dest, src);
    }

    print_connected_components(graph);
    lg_free(graph);

    return 0;
}
