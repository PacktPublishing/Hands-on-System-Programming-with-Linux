/*
 * ch13/intv_clksimple.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 13 : Timers
 ****************************************************************
 * Brief Description:
 * A (very) simple digital clock - a demo of using the (older) UNIX
 * interval timer API - the setitimer(2) syscall. The program also
 * uses appropriate (signal-safe) APIs to query the current timestamp
 * within the SIGALRM signal handler.
 *
 * For details, please refer the book, Ch 13.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "../common.h"

static volatile sig_atomic_t opt;

static void ticktock(int signum)
{
	char tmstamp[128];
	struct timespec tm;
	int myerrno = errno;

	/* Query the timestamp ; both clock_gettime(2) and
	 * ctime_r(3) are reentrant-and-signal-safe */
	if (clock_gettime(CLOCK_REALTIME, &tm) < 0)
		FATAL("clock_gettime failed\n");
	if (ctime_r(&tm.tv_sec, &tmstamp[0]) == NULL)
		FATAL("ctime_r failed\n");

	if (opt == 0) {
		if (write(STDOUT_FILENO, tmstamp, strlen(tmstamp)) < 0)
			FATAL("write failed\n");
	} else if (opt == 1) {
		/* WARNING! Using the printf / fflush here in a signal handler is
		 * unsafe! We do so for the purposes of this demo app only; do not
		 * use in production.
		 */
		tmstamp[strlen(tmstamp) - 1] = '\0';
		printf("\r%s", tmstamp);
		fflush(stdout);
	}
	errno = myerrno;
}

static void usage_die(char *name)
{
	fprintf(stderr, "Usage: %s {0|1}\n"
		" 0 : the Correct way (using write(2) in the signal handler)\n"
		" 1 : the *Wrong* way (using printf(3) in the signal handler) *@your risk*\n",
		name);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct sigaction act;
	struct itimerval mytimer;

	if ((argc != 2) || (strlen(argv[1]) != 1))
		usage_die(argv[0]);
	opt = atoi(argv[1]);
	if ((opt != 0) && (opt != 1))
		usage_die(argv[0]);

	printf("Initializing ...\n");
	if (opt == 1)
		printf(" *WARNING* [Using printf in signal handler]\n");

	memset(&act, 0, sizeof(act));
	act.sa_handler = ticktock;
	sigfillset(&act.sa_mask);	/* disallow all signals while handling */
	/*
	 * We deliberately do *not* use the SA_RESTART flag;
	 * if we do so, its possible that any blocking syscall gets
	 * auto-restarted. In a timeout context, we don't want that
	 * to happen - we *expect* a signal to interrupt our blocking
	 * syscall (in this case, the pause(2)).
	 *  act.sa_flags = SA_RESTART;
	 */
	if (sigaction(SIGALRM, &act, 0) < 0)
		FATAL("sigaction on SIGALRM failed");

	/* Setup a single second (repeating) interval timer */
	memset(&mytimer, 0, sizeof(struct itimerval));
	mytimer.it_value.tv_sec = 1;
	mytimer.it_interval.tv_sec = 1;
	if (setitimer(ITIMER_REAL, &mytimer, 0) < 0)
		FATAL("setitimer failed\n");

	while (1)
		(void)pause();

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
