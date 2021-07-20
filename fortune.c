/* simple fortune(1) implementation with added option to choose a
 * field separator.
 * Author: Ben O'Neill <ben@benoneill.xyz>
 * License: MIT
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void fortune(FILE *ffile, char sep);

char sep = '\n'; // default separator

int main(int argc, char *argv[])
{
	FILE *fp;

	if (argc == 1) {
		fp = fopen("/lib/fortunes"); return 1;
	}
	if (argc > 3) {
		fprintf(stderr, "too many arguments.\n");
		return EXIT_FAILURE;
	}

	if (argc > 1) {
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			fprintf(stderr, "can't open file\n");
			return EXIT_FAILURE
		}
	}
	if (argc > 2)
		sep = *argv[2];

	fortune(fp, sep);
	fclose(fp);
	return EXIT_SUCCESS;
}

/* actual fortune function */
void fortune(FILE *ffile, char sep)
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
				return;
			}
			while (fgets(linebuf, buflen, ffile)) {
				if (linebuf[0] == sep && strlen(linebuf) == 2)
					return;
				printf("%s", linebuf);
			}
		}
	}
}
