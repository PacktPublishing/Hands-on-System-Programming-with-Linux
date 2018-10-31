/*
 * Assignment Solution
 * solutions_to_assgn/ch11/atomic_B.c
 * 
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

#define SLEEP_SEC 5
volatile sig_atomic_t gFlag = 0;

static void handle_sigs(int signum, siginfo_t * info, void * rst)
{
	gFlag = 1;
}

int main(int argc, char **argv)
{
	struct sigaction sigact;

	memset(&sigact, 0, sizeof(sigact));
	sigact.sa_sigaction = handle_sigs;
	sigact.sa_flags = SA_SIGINFO | SA_NOCLDSTOP;
	if (sigaction(SIGUSR1, &sigact, 0) < 0)
		FATAL("sigaction on SIGUSR1 failed");
	
	printf("%s:%d trapped SIGUSR1, blocking now ...\n",
			argv[0], getpid());

	/* As a demo, poll gFlag's value */
	while(1) {
		printf(" *%d* ", gFlag);
		fflush(stdout);
		sleep(SLEEP_SEC);
	}
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
