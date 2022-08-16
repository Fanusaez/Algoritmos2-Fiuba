#include "testing.h"
#include <stdio.h>

#include <unistd.h> // isatty
#define ANSI_COLOR_LGH_RED	   "\x1b[1m\x1b[31m"
#define ANSI_COLOR_LGH_GREEN   "\x1b[1m\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

static int _failure_count;

void real_print_test(const char* mensaje, bool ok,
		     const char* file, int line, const char* failed_expr) {
	if (ok) {
		printf("%s... ", mensaje);
		if (isatty(1)==1) printf(ANSI_COLOR_LGH_GREEN);
		printf("OK\n");
	} else {
		printf("%s: ",mensaje);
		if (isatty(1)==1) printf(ANSI_COLOR_LGH_RED);
		printf("ERROR\n" "%s:%d: %s\n", file, line, failed_expr);
	}
	if (isatty(1)==1) printf(ANSI_COLOR_RESET);
	fflush(stdout);
	_failure_count += !ok;
}

int failure_count() {
	return _failure_count;
}
