#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
	int cmd_len;
	char *cmd;
	FILE *pfp;
	int i;

	if (argc < 2) {
		fprintf(stderr, "usage: %s command\n", argv[0]);
		exit(1);
	}
	cmd_len = 0;
	for (i = 1; i < argc; i++)
		cmd_len += strlen(argv[i]) + 1;
	cmd = (char *)malloc(cmd_len + 2);
	*cmd = '\0';
	for (i = 1; i < argc; i++) {
		strcat(cmd, argv[i]);
		strcat(cmd, " ");
	}
/*
	strcat(cmd, "&");
*/
/*
	fputs(cmd, stderr);
*/
	if ((pfp = popen(cmd, "r")) == NULL) {
		fprintf("can not execute command \"%s\"\n", cmd);
		exit(1);
	}
	while (!feof(pfp)) {
	    putchar(fgetc(pfp));
	}
	getchar();
	pclose(pfp);
	exit(0);
}

