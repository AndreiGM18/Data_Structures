#include <stdio.h>
#include <stdlib.h>
 
int check(int x1, int y1, int x2, int y2, int r1, int r2)
{
    int dist_sq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    int rad_sum_sq = (r1 + r2) * (r1 + r2);
    if (dist_sq <= rad_sum_sq)
        return 1;
    else
		return 0;
}

int main(void)
{
	int n;
	scanf("%d", &n);
	int **a = (int **)malloc(n * sizeof(int *));
	if (!a) {
		printf("malloc failed\n");
		return -1;
	}
	for (int i = 0; i < n; ++i) {
		a[i] = (int *)malloc(3 * sizeof(int));
		if (!a[i]) {
			printf("malloc failed\n");
			return -1;
			}
		for (int j = 0; j < 3; ++j)
			scanf("%d", &a[i][j]);
	}
	int nr = 0;;
	for (int i = 0; i < n - 1; ++i)
		for (int j = i + 1; j < n; ++j)
    		if (check(a[i][0], a[i][1], a[j][0], a[j][1], a[i][2], a[j][2]))
				++nr;
	for (int i = 0; i < n; ++i)
		free(a[i]);
	free(a);
	printf("%d", nr);
    return 0;
}
