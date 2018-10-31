/*
 * ch9/successor_setnm.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 9 : Process Execution
 ****************************************************************
 * Brief Description:
 * [Paired with the 'predcs_name.c' code].
 * A demo of using the execl(3) API; this is the second of two
 * programs. 
 *
 * In this program, we accept a name (provided by the user)
 * passed o us by the predecessor program.
 * However, we find that this does not actually change the name of
 * this, the successor process; the right way to do so is to use
 * the prctl(2) with the PR_SET_NAME parameter, (or via the 
 * pthread_setname_np(3) Pthreads API) within the successor itself.
 *
 * For details, please refer the book, Ch 9.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include "../common.h"

int main(int argc, char **argv)
{
	int i;

	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: %s {successor_name}"
				" [do-it-right-param]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("%s:parameters received:\n", argv[0]);
	for (i=0; i<argc; i++)
		printf("argv[%d]=%s\n", i, argv[i]);

	if (argc == 3) { /* the "do-it-right" case! */
		printf("%s: setting name to \"%s\" via prctl(2)"
				" [Right]\n", argv[0], argv[2]);
		if (prctl(PR_SET_NAME, argv[2], 0, 0, 0) < 0)
			FATAL("prctl failed\n");
	} else {         /* wrong way... */
		printf("%s: attempt to implicitly set name to \"%s\""
			" via the argv[0] passed to execl [Wrong]\n",
			argv[0], argv[1]);
	}

	printf("%s: pausing now...\n", argv[0]);
	pause();

	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
