/*
 * ch10/fork_malloc_test.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 * Brief Description:
 * Building on the sample code of fork5.c, this program is used to
 * demonstrate the reality that if a dynamic memory alloc (say, an
 * malloc) is done and after that the process forks, the 'heap'
 * memory is _copied across the fork_ - fork rule #4! after all.
 * Thus, we have a _copy_ of the memory pointer and the memory
 * chunk in the child process. 
 *
 * But, the address is the *same* in parent and child! True, but
 * don't leap to erroneous conclusions: it's the same because
 * a) it's a _virtual_ address (not physical)
 * b) it _is_ actually the same virtual address, because modern
 * OS's like Linux do not immediately make a copy of data/stack
 * segments on fork; they use an optimized semantic called COW -
 * Copy On Write!
 *
 * Also, note we must free the memory chunk in _both_ the parent
 * and child processes.
 *
 * For details, please refer the book, Ch 10.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

#define CHILD        1
#define PARENT       2

#define WORK_ON_MEM(PROCESS, loc, g) do {    \
	if (PROCESS == CHILD) {              \
		loc ++ ; g --;               \
	} else if (PROCESS == PARENT) {      \
		loc -- ; g ++;               \
	}                                    \
	printf(" loc=%d, g=%d\n"             \
	       " &loc=%p, &g=%p\n"           \
		, loc, g, &loc, &g);         \
} while(0)

const int memsz = 2048;
static char *gptr;
static int g = 5;

static inline void disp_few(char *p)
{
	int i;
	printf(" malloc gptr=%p\n ", p);
	for (i = 0; i < 16; i++)
		printf("%c", *(p + i));
	printf("\n");
}

int main(int argc, char **argv)
{
	pid_t ret;
	int show_vars = 0, loc = 8;

	if (argc > 1)
		show_vars = 1;

	gptr = malloc(memsz);
	if (!gptr)
		FATAL("malloc failed!\n");

	printf("Init: malloc gptr=%p\n", gptr);
	if (show_vars)
		printf("Init: loc=%d, g=%d\n", loc, g);

	switch ((ret = fork())) {
	case -1:
		free(gptr);
		FATAL("fork failed, aborting!\n");
	case 0:		/* Child */
		printf("\nChild process, PID %d:\n", getpid());

		memset(gptr, 'c', memsz);
		disp_few(gptr);
		if (show_vars)
			WORK_ON_MEM(CHILD, loc, g);

		free(gptr);
		printf("Child (%d) done, exiting ...\n", getpid());
		exit(EXIT_SUCCESS);
	default:		/* Parent */
#if 1
		sleep(2);	/* let the child run first (silly way) */
#endif
		printf("\nParent process, PID %d:\n", getpid());

		memset(gptr, 'p', memsz);
		disp_few(gptr);
		if (show_vars)
			WORK_ON_MEM(PARENT, loc, g);
	}

	free(gptr);
	printf("Parent (%d) will exit now...\n", getpid());
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
