/*
 * typetest.c: a simple typing speed test. not super accurate.
 * prints a file line by line and has user reproduce the line.
 * records WPM, CPM, number of mistakes, total time.
 * usage: typetest file
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *sample_test = "Porro molestias eius maiores reprehenderit rerum.";

struct result_s {
	unsigned int mistakes, total_time, num_words;
	float average_time;
};

typedef struct result_s result_t;

result_t replication_test(FILE *);

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

	r = replication_test(fp);
	fclose(fp);
	printf("Number of mistakes: %d\nCharacters per minute: "
			"%f\nWords per minute: %f\nTotal time: %d seconds\n",
			r.mistakes, ((float) 60) / r.average_time,
			60 / ((float) r.total_time / r.num_words), r.total_time);
	return 0;
}

/*
 * replicate the given file line by line, no time limit.
 * content: string to replicate
 * returns: number of mistakes, average, and total times as result_t
 */
result_t replication_test(FILE *fp)
{
	int mistakes = 0, c, i;
	size_t len = 0;
	size_t total_len = 0, words = 0;
	ssize_t read;
	char *line = NULL;
	time_t start_time, end_time;
	result_t r;

	start_time = time(NULL);
	while ((read = getline(&line, &len, fp)) != EOF) {
		printf("%s> ", line);

		total_len += read - 1;
		for (i = 0; i < read; i++) {
			if (line[i] == ' ' || line[i] == '\n')
				words++;

			c = getchar();
			if (c == '\b') {
				i--;
			} else if (c != line[i]) {
				mistakes++;
			} else if (c == '\n') {
				continue;
			}
		}
	}
	end_time = time(NULL);

	r.mistakes = mistakes;
	r.num_words = words;
	r.total_time = end_time - start_time;
	r.average_time = ((float) r.total_time) / total_len;
	return r;
}
