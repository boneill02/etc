/*
 * brainfuck interpreter
 */
#define MAX_LOOPS 2048
#define TAPE_SIZE 30000
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint16_t start, end;
} Loop;

char *prog;
uint8_t tape[TAPE_SIZE];
uint16_t tp, ip, sp;
Loop loops[MAX_LOOPS];
size_t program_len;
size_t num_loops;

void build_loops() {
	num_loops = 0;

	for (int i = 0; i < program_len; i++) {
		if (prog[i] == '[') {
			loops[num_loops].start = i;
			num_loops++;
		} else if (prog[i] == ']') {
			for (int j = num_loops - 1; j >= 0; j--) {
				if (!loops[j].end) {
					loops[j].end = i;
					break;
				}
			}
		}
	}
}

void interpret() {
	switch (prog[ip]) {
		case '+':
			tape[tp]++;
			break;
		case '-':
			tape[tp]--;
			break;
		case '>':
			tp++;
			break;
		case '<':
			tp--;
			if (tp < 0) {
				fprintf(stderr, "tape pointer out of bounds.\n");
				exit(1);
			}
			break;
		case ',':
			char c = fgetc(stdin);
			/* for reading files (EOF = -1, will overflow on uint8_t) */
			if (c == EOF) {
				c = 0;
			}
			tape[tp] = c;
			break;
		case '.':
			putchar(tape[tp]);
			break;
		case '[':
			if (!tape[tp]) {
				for (int i = 0; i < num_loops; i++) {
					if (loops[i].start == ip) {
						ip = loops[i].end;
					}
				}
			}
			break;
		case ']':
			if (tape[tp]) {
				for (int i = 0; i < num_loops; i++) {
					if (loops[i].end == ip) {
						ip = loops[i].start;
					}
				}
			}
			break;
	}
}

void run() {
	tp = 0;
	sp = 0;

	build_loops();
	for (ip = 0; ip < program_len; ip++) {
		interpret(prog[ip]);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s progfile\n", argv[0]);
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	if (f) {
		fseek(f, 0, SEEK_END);
		program_len = ftell(f);
		fseek(f, 0, SEEK_SET);

		prog = malloc(program_len);
		if (prog) {
			fread(prog, 1, program_len, f);
		} else {
			fprintf(stderr, "can't allocate memory for program storage.\n");
		}
		fclose(f);
	} else {
		fprintf(stderr, "can't open file.\n");
		return 1;
	}

	run();
	free(prog);
	return 0;
}
