#include <stdio.h>
#include <stdlib.h>
#define NCOL 3

int main(void)
{
	int nrow;
	scanf("%d", &nrow);
	int (*a)[NCOL] = malloc(nrow * sizeof(*a));
	for (int i = 0; i < nrow; ++i)
		for (int j = 0; j < NCOL; ++j)
			scanf("%d", &a[i][j]);
	for (int i = 0; i < nrow; ++i)
		for (int j = 0; j < NCOL; ++j)
			printf("%p ", &a[i][j]);
	printf("\n");
	sizeof(a);
	for (int i = 0; i < nrow; ++i)
		printf("%p ", a[i]);
	free(a);
	return 0;
}