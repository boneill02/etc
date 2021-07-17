/* simple fortune(1) implementation. License: MIT */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int fortune(FILE *ffile, char sep);

char sep = '\n'; // default separator

int main(int argc, char *argv[])
{
	FILE *ffile;

	if (argc == 1) {
		fprintf(stderr, "a fortune file must be provided.\n");
		return 1;
	}
	if (argc > 3) {
		fprintf(stderr, "too many arguments.\n");
		return 1;
	}

	if (argc > 1)
		ffile = fopen(argv[1], "r");
	if (argc > 2)
		sep = *argv[2];

	return fortune(ffile, sep);
}

int fortune(FILE *ffile, char sep)
{
	/* 255 characters per line */
	int fnum = 0, buflen = 255, cur = 0, resfortune;
	char linebuf[buflen];

	srand(time(NULL));

	/* get number of fields in character */
	while(fgets(linebuf, buflen, ffile)) {
		if (sep == '\n') {
			/* every line is a new fortune */
			fnum++;
		} else if (linebuf[0] == sep && strlen(linebuf) == 2) {
			fnum++;
		}
	}

	/* pull random fortune */
	resfortune = rand() % fnum;

	rewind(ffile);
	while (fgets(linebuf, buflen, ffile)) {
		if (sep == '\n' || (linebuf[0] == sep && strlen(linebuf) == 2))
			cur++;

		if (cur == resfortune) {
			if (sep == '\n') {
				printf("%s", linebuf);
				goto cleanup;
			}
			while (fgets(linebuf, buflen, ffile)) {
				if (linebuf[0] == sep && strlen(linebuf) == 2)
					goto cleanup;
				printf("%s", linebuf);
			}
		}
	}

cleanup:
	fclose(ffile);
	return 0;
}
