/*
 * ch11/zombies_clear_linux26.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 11 : Signalling Part I
 ****************************************************************
 * Brief Description:
 *
 * A process forks; the parent does not wait for the child,
 * it continues to do some work...
 * When the child does die, the parent should nonetheless clear
 * the resulting zombie.
 *
 * We do this here using one of two really easy! approaches:
 * (a) Linux 2.6: set the SA_NOCLDWAIT flag (within the sigaction()
 * trapping SIGCHLD); the kernel then ensures that terminated
 * child(ren) do not become zombies.
 * (b) Simpler: just ignore the SIGCHLD signal. But: parent will never
 * know when child(ren) die.
 *
 * For details, please refer the book, Ch 11.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../common.h"

#define DEBUG
/* SIGCHLD handler */
static void child_dies(int signum)
{
#ifdef DEBUG
	printf("\n*** Child dies! ***\n");
#endif
}

static void usage(char *name)
{
	fprintf(stderr,
		"Usage: %s {option-to-prevent-zombies}\n"
		" 1 : (2.6 Linux) using the SA_NOCLDWAIT flag with sigaction(2)\n"
		" 2 : just ignore the signal SIGCHLD\n", name);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct sigaction act;
	int opt = 0;

	if (argc != 2)
		usage(argv[0]);

	opt = atoi(argv[1]);
	if (opt != 1 && opt != 2)
		usage(argv[0]);

	memset(&act, 0, sizeof(act));
	if (opt == 1) {
		act.sa_handler = child_dies;
	/* 2.6 Linux: prevent zombie on termination of child(ren)! */
		act.sa_flags = SA_NOCLDWAIT;
	}
	if (opt == 2)
		act.sa_handler = SIG_IGN;
	act.sa_flags |= SA_RESTART | SA_NOCLDSTOP; /* no SIGCHLD on stop of child(ren) */

	if (sigaction(SIGCHLD, &act, 0) == -1)
		FATAL("sigaction failed");

	printf("parent: %d\n", getpid());
	switch (fork()) {
	case -1:
		FATAL("fork failed");
	case 0:		// Child
		printf("child: %d\n", getpid());
		DELAY_LOOP('c', 25);
		exit(0);
	default:		// Parent
		while (1)
			pause();
	}
	exit(0);
}

/* vi: ts=8 */
