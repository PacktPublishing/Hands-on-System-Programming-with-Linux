/*
 * ch12/sleeping_beauty.c
 * ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 12 : Signaling Part II
 ****************************************************************
 * Brief Description:
 * This program tests two ways of haveing a process (or thread) sleep:
 * option 0 : via the 'usual' sleep(3) API
 * option 1 : via the powerful nanosleep(2) API.
 *
 * One finds that sleep(3) returns the time remaining to sleep; most
 * developers do not take this into account.
 * With the nanosleep(2), the OS returns the time remaining to sleep
 * (after signal interruption); we call the API in a loop, setting 'req'
 * to 'rem', thus ensuring that the sleep completes.
 *
 * For details, please refer the book, Ch 12.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include "../common.h"

static void sig_handler(int signum)
{
	fprintf(stderr, "**Signal %d interruption!**\n", signum);
}

int main(int argc, char **argv)
{
	struct sigaction act;
	int nsec = 10, ret;
	struct timespec req, rem;

	if (argc == 1) {
		fprintf(stderr, "Usage: %s option=[0|1]\n"
			"0 : uses the sleep(3) function\n"
			"1 : uses the nanosleep(2) syscall\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* setup signals: trap SIGINT and SIGQUIT */
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &act, 0) || sigaction(SIGQUIT, &act, 0))
		FATAL("sigaction failure\n");

	if (atoi(argv[1]) == 0) {	/* sleep */
		printf("sleep for %d s now...\n", nsec);
		ret = sleep(nsec);
		printf("sleep returned %u\n", ret);
	} else if (atoi(argv[1]) == 1) {	/* nanosleep */
		req.tv_sec = nsec;
		req.tv_nsec = 0;
		while ((nanosleep(&req, &rem) == -1) && (errno == EINTR)) {
			printf("nanosleep interrupted: rem time: %07lu.%07lu\n",
			       rem.tv_sec, rem.tv_nsec);
			req = rem;
		}
	}
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
