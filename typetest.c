/*
 * typetest.c: A simple typing speed test. Not very precise. Prints a
 * given file line by line and has user reproduce each one. It records
 * WPM, CPM, number of mistakes, and total time.
 * usage: typetest file
 * author: Ben O'Neill <ben@oneill.sh>
 * license: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

typedef struct {
	unsigned int mistakes, charCount, wordCount;
	float totalTime, averageTime;
} result_t;

static void replication_test(FILE *, result_t *);

int main(int argc, char *argv[])
{
	FILE *fp;
	result_t r;

	if (argc == 2) {
		fp = fopen(argv[1], "r");
		if (fp == NULL) {
			fprintf(stderr, "can't open file\n");
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "usage: %s file\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	replication_test(fp, &r);
	fclose(fp);
	printf("Number of mistakes: %d\nCharacters per minute: "
			"%f\nWords per minute: %f\nTotal time (ms): %f\n",
			r.mistakes,
			(60.0 * 1000 * 1000)  / r.averageTime,
			(60.0 * 1000 * 1000) / (r.totalTime / r.wordCount),
			r.totalTime / 1000);
	return EXIT_SUCCESS;
}

/*
 * replicate the given file line by line, with no time limit.
 * content: string to replicate
 * returns: number of mistakes, average, and total times as result_t
 */
static void replication_test(FILE *fp, result_t *r) {
	struct timeval start, stop;
	size_t len = 0;
	size_t words = 0, chars = 0;
	char *line = NULL;

	int read;
	gettimeofday(&start, NULL);	
	while ((read = getline(&line, &len, fp)) != EOF) {
		printf("%s> ", line);

		for (int i = 0; i < read; i++) {
			if (line[i] == ' ' || line[i] == '\n') {
				words++;
			} else {
				chars++;
			}

			char c = getchar();
			if (c == '\b') {
				i--;
			} else if (c != line[i]) {
				r->mistakes++;
			} else if (c == '\n') {
				continue;
			}
		}
	}
	gettimeofday(&stop, NULL);	

	r->wordCount = words;
	r->totalTime = stop.tv_usec - start.tv_usec;
	r->averageTime = r->totalTime / r->wordCount;
}
