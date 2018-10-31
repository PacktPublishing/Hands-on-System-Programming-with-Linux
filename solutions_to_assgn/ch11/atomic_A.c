/*
 * Assignment Solution
 * solutions_to_assgn/ch11/atomic_A.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 ****************************************************************

Q4. atomic_A, atomic_B : As explained in Ch 11, section 'Signal-Safe Atomic
Integers', implement a small application with two processes: A and B. Have
process A complete some work, and signal process B that it has done so.
Do this in an atomic fashion (no data races).

 *
 * We poll (and display) the value of the gFlag atomic variable; thus,
 * one can see that after signal delivery it's set to 1.
 *
 * Run as (f.e.):
 * $ ./ atomic_B &
 * ./atomic_B:26622 trapped SIGUSR1, blocking now ...
 * *0*  [...]
 * $ ./atomic_A @ 26622
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ *0* \
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ *1* $  *1*
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include "../../common.h"

static void foo(char ch)
{
	DELAY_LOOP(ch, 220);
}

int main(int argc, char **argv)
{
	pid_t processB=0;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s char-to-emit PID-of-process-atomicB\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (strlen(argv[1]) > 1) {
		fprintf(stderr, "Usage: %s char-to-emit\n"
				" Specify ONLY ONE character please...\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	processB = atoi(argv[2]);
	/* Is this PID valid? */
	if (kill(processB, 0) < 0)
		FATAL("kill: PID %d is invalid or perm issue, aborting...\n", processB);

	foo(argv[1][0]);

	/* foo() completed, signal process 'B' */
	if (kill(processB, SIGUSR1) < 0)
		FATAL("kill: sending SIGUSR1 to process B failed\n");
	
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
