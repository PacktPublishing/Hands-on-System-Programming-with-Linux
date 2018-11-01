/*
 * solutions_to_assgn/ch13/3intv.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 13 : Timers
 ****************************************************************
 * Q1. Write a program '3intv.c' that sets up and experiences timeouts on three
 * interval timers - an ITIMER_REAL, ITIMER_VIRTUAL and ITIMER_PROF timers.
 *
 * Here, we reuse the 'simple clock' code for the ITIMER_REAL timer, and
 * specify some sample/random 'value' and 'interval' values for the
 * ITIMER_VIRTUAL and ITIMER_PROF timers.
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
#include "../../common.h"

static void profalrm(int signum)
{
	printf("In %s:%d sig=%d\n",
		__func__, __LINE__, signum);
}
static void virtalrm(int signum)
{
	printf("In %s:%d sig=%d\n",
		__func__, __LINE__, signum);
}
static void ticktock(int signum)
{
	char tmstamp[128];
	struct timespec tm;
	int myerrno = errno, opt=0;

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

int main(int argc, char **argv)
{
	struct sigaction act;
	struct itimerval t1, t2, t3;

	//--- Handler for first timer - SIGALRM
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

	//--- Handler for second timer - SIGVTALRM
	memset(&act, 0, sizeof(act));
	act.sa_handler = virtalrm;
	sigfillset(&act.sa_mask);	/* disallow all signals while handling */
	if (sigaction(SIGVTALRM, &act, 0) < 0)
		FATAL("sigaction on SIGALRM failed");

	//--- Handler for third timer - SIGPROF
	memset(&act, 0, sizeof(act));
	act.sa_handler = profalrm;
	sigfillset(&act.sa_mask);	/* disallow all signals while handling */
	if (sigaction(SIGPROF, &act, 0) < 0)
		FATAL("sigaction on SIGALRM failed");

	/* Timer 1: Setup a single second (repeating) interval timer */
	memset(&t1, 0, sizeof(struct itimerval));
	t1.it_value.tv_sec = 1;
	t1.it_interval.tv_sec = 1;
	if (setitimer(ITIMER_REAL, &t1, 0) < 0)
		FATAL("setitimer failed\n");

	/* Timer 2: Setup a virtual interval timer */
	memset(&t2, 0, sizeof(struct itimerval));
	t2.it_value.tv_sec = 0;
	t2.it_value.tv_usec = 70000;
	t2.it_interval.tv_sec = 0;
	t2.it_interval.tv_usec = 30000;
	if (setitimer(ITIMER_VIRTUAL, &t2, 0) < 0)
		FATAL("setitimer failed\n");

	/* Timer 3: Setup a profiling interval timer */
	memset(&t3, 0, sizeof(struct itimerval));
	t3.it_value.tv_sec = 0;
	t3.it_value.tv_usec = 10000;
	t3.it_interval.tv_sec = 0;
	t3.it_interval.tv_usec = 10000;
	if (setitimer(ITIMER_PROF, &t3, 0) < 0)
		FATAL("setitimer failed\n");

	while (1) {
		DELAY_LOOP_SILENT(20); /* Do some work on cpu, so that the
				 virtual and profiler timers can fire.. */
		(void)pause();
	}
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
