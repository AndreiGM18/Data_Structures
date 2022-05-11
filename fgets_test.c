#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char line[256];
	fgets(line, 256, stdin);

	int len = strlen(line);

	if (line[len - 1] == '\n')
		line[len - 1] = '\0';

	char *tok = strtok(line, " \n");
	char newline[256];

	while (tok) {
		printf("the size of %s is %ld\n", tok, strlen(tok));
		strcat(newline, tok);
		if (!atoi(tok))
			strcat(newline, " ");
		tok = strtok(NULL, "\n ");
	}

	printf("%s and size is %ld\n", newline, strlen(newline));

	return 0;
}