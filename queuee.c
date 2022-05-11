#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_STRING_SIZE	256

/* --- START IMPLEMENT QUEUE HERE --- */

typedef struct queue_t queue_t;
struct queue_t
{
	/* Dimensiunea maxima a cozii */
	unsigned int max_size;
	/* Dimensiunea cozii */
	unsigned int size;
	/* Dimensiunea in octeti a tipului de date stocat in coada */
	unsigned int data_size;
	/* Indexul de la care se vor efectua operatiile de front si dequeue */
	unsigned int read_idx;
	/* Indexul de la care se vor efectua operatiile de enqueue */
	unsigned int write_idx;
	/* Bufferul ce stocheaza elementele cozii */
	void **buff;
};

queue_t *
q_create(unsigned int data_size, unsigned int max_size)
{
	queue_t *q = (queue_t *)malloc(sizeof(queue_t));
	if (!q)
		return NULL;

	q->max_size = max_size;
	q->size = 0;
	q->data_size = data_size;
	q->read_idx = 0;
	q->write_idx = 0;
	q->buff = malloc(max_size * sizeof(void *));
	for (unsigned int i = 0; i < q->max_size; ++i)
			q->buff[i] = malloc(q->data_size);

	if (!q->buff) {
		return NULL;
		free(q);
	}

	return q;
}

/*
 * Functia intoarce numarul de elemente din coada al carei pointer este trimis
 * ca parametru.
 */
unsigned int
q_get_size(queue_t *q)
{
	if (!q)
		return 0;

	return q->size;
}

/*
 * Functia intoarce 1 daca coada este goala si 0 in caz contrar.
 */
unsigned int
q_is_empty(queue_t *q)
{
	if (!q || !q->size)
		return 1;

	return 0;
}

/* 
 * Functia intoarce primul element din coada, fara sa il elimine.
 */
void *
q_front(queue_t *q)
{
	if (!q)
		return NULL;

	if (q_is_empty(q))
		return NULL;
	
	return q->buff[q->read_idx];

}

/*
 * Functia scoate un element din coada. Se va intoarce 1 daca operatia s-a
 * efectuat cu succes (exista cel putin un element pentru a fi eliminat) si
 * 0 in caz contrar.
 */
int
q_dequeue(queue_t *q)
{
	if (!q)
		return 0;

	if (q_is_empty(q))
		return 0;

	q->read_idx = (q->read_idx + 1) % q->max_size;
	--(q->size);

	return 1;
}

/* 
 * Functia introduce un nou element in coada. Se va intoarce 1 daca
 * operatia s-a efectuat cu succes (nu s-a atins dimensiunea maxima) 
 * si 0 in caz contrar.
 */
int
q_enqueue(queue_t *q, void *new_data)
{
	if (!q)
		return 0;

	if (q->size == q->max_size)
		return 0;

	memcpy(q->buff[q->write_idx], new_data, q->data_size);
	q->write_idx = (q->write_idx + 1) % q->max_size;
	++(q->size);
	return 1;
}

/*
 * Functia elimina toate elementele din coada primita ca parametru.
 */
void
q_clear(queue_t *q)
{
	if (!q || q_is_empty(q))
		return;

	q->read_idx = 0;
	q->write_idx = 0;
	q->size = 0;
}

/*
 * Functia elibereaza toata memoria ocupata de coada.
 */
void
q_free(queue_t *q)
{
	if (!q)
		return;

	for (unsigned int i = 0; i < q->max_size; ++i)
		free(q->buff[i]);
	free(q->buff);
	free(q);

}

int main ()
{
	queue_t *queue;
	int is_int = 0;
	int is_string = 0;

	while (1) {
		char command[MAX_STRING_SIZE];
		char elem[MAX_STRING_SIZE];
		long nr;

		scanf("%s", command);
		if (strncmp(command, "create_q_str", 12) == 0) {
			queue = q_create(MAX_STRING_SIZE, 5);
			is_string = 1;
		}
		
		if (strncmp(command, "create_q_int", 12) == 0) {
			queue = q_create(sizeof(int), 5);
			is_int = 1;
		}

		if (strncmp(command, "enq", 3) == 0) {
			if (is_int) {
				scanf("%ld", &nr);
				int ret = q_enqueue(queue, &nr);
				if (!ret)
					printf("Enqueue operation failed!\n");
			} else if (is_string) {
				scanf("%s", elem);
				int ret = q_enqueue(queue, elem);
				if (!ret)
					printf("Enqueue operation failed!\n");
			} else {
				printf("Create a queue first!\n");
				exit(0);
			}
		}

		if (strncmp(command, "deq", 3) == 0) {
			if (!is_int && !is_string) {
				printf("Create a queue first!\n");
				exit(0);
			}

			int ret = q_dequeue(queue);
			if (!ret)
				printf("Dequeue operation failed!\n");
		}

		if (strncmp(command, "front", 5) == 0) {
			if (!is_int && !is_string) {
				printf("Create a queue first!\n");
				exit(0);
			}

			if (is_int) {
				int *ptr = (int *) q_front(queue);
				if (ptr) {
					int val = *ptr;
            		printf("%d\n", val);
				}
			}

			if (is_string) {
				char *val = (char *) q_front(queue);
				if (val)
					printf("%s\n", val);
			}
		}

		if (strncmp(command, "free", 4) == 0) {
			if (!is_int && !is_string) {
				printf("Create a queue first!\n");
				exit(0);
			}
			q_clear(queue);
			q_free(queue);
			break;
		}
	}

	return 0;
}
