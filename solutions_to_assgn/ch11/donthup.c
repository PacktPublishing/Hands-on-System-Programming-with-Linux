/*
 * Assignment Solution
 * solutions_to_assgn/ch11/donthup.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 ****************************************************************

Q1. What if we run an application process from a terminal window, but, while
it's alive, close the terminal window?
Well, it's a known fact—the processes belonging to that terminal (technically,
in the process group associated with that tty device) will be killed via the
SIGHUP signal; they will "hang up".
Write a program 'donthup' that will not die even when its controlling terminal
is shut down.

 * Recommendation:
 * Redirect stdout to a log file.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "../../common.h"

/* Our simple signal handler */
static void siggy(int signum)
{
	const char *str0 = "*** siggy: handled SIGHUP ***\n";
	const char *str1 = "*** siggy: handled SIGINT ***\n";
	const char *str2 = "*** siggy: handled SIGQUIT ***\n";

	switch (signum) {
	case SIGHUP:
		if (write(STDOUT_FILENO, str0, strlen(str0)) < 0)
			WARN("write str0 failed!");
		return;
	case SIGINT:
		if (write(STDOUT_FILENO, str1, strlen(str1)) < 0)
			WARN("write str1 failed!");
		return;
	case SIGQUIT:
		if (write(STDOUT_FILENO, str2, strlen(str2)) < 0)
			WARN("write str2 failed!");
		return;
	}
}

int main(void)
{
	unsigned long int i = 1;
	struct sigaction act;

	/* Init sigaction to defaults via the memset,
	 * setup 'siggy' as the signal handler function,
	 * trap just the SIGINT and SIGQUIT signals.
	 */
	memset(&act, 0, sizeof(act));
	act.sa_handler = siggy;
	if (sigaction(SIGHUP, &act, 0) < 0)
		FATAL("sigaction on SIGINT failed");
	if (sigaction(SIGINT, &act, 0) < 0)
		FATAL("sigaction on SIGINT failed");
	if (sigaction(SIGQUIT, &act, 0) < 0)
		FATAL("sigaction on SIGQUIT failed");

	while (1) {
		printf("Looping, iteration #%02ld ...\n", i++);
		(void)sleep(1);
	}

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
