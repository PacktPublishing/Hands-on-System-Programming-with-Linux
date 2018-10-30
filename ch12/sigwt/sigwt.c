/*
 * ch13/sigwt/sigwt.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 13 : Signaling Part II
 ****************************************************************
 * Brief Description:
 * This program demonstrates using the sigwait(3) API; one can run it in two
 * ways, passing 'option' 0 or 1:
 * Usage: ./sigwt 0|1
 * 0 => block All signals and sigwait for them
 * 1 => block all signals except the SIGFPE and SIGSEGV, and sigwait
 *      (further, for case 1, we setup an async handler for the SIGFPE, not the SIGSEGV)
 *
 * For details, please refer the book, Ch 13.
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
	const char *str1 = "*** siggy: handled SIGFPE (8) ***\n";
	const char *str2 = "*** siggy: handled SIGSEGV (11) ***\n";

	switch (signum) {
	case SIGFPE:
		if (write(STDOUT_FILENO, str1, strlen(str1)) < 0)
			WARN("write str1 failed!");
		return;
	case SIGSEGV:
		if (write(STDOUT_FILENO, str2, strlen(str2)) < 0)
			WARN("write str2 failed!");
		return;
	}
}

int main(int argc, char **argv)
{
	struct sigaction act;
	sigset_t set;
	int sig=0;

	if (argc != 2 || ((atoi(argv[1]) != 0) && atoi(argv[1]) != 1)) {
		fprintf(stderr, "Usage: %s 0|1\n"
				" 0 => block All signals and sigwait for them\n"
				" 1 => block all signals except the SIGFPE and"
				" SIGSEGV, and sigwait\n"
				"      (further, for case 1, we setup an async handler for the"
				" SIGFPE, not the SIGSEGV)\n"
				, argv[0]);
		exit(EXIT_FAILURE);
	}
	sigfillset(&set);  /* the '0' case - block all signals */

	if (atoi(argv[1]) == 1) {
		/* IMP: unblocking signals here removes them from the influence of 
		 * the sigwait* APIs; this is *required* for correctly handling
		 * fatal signals from the kernel.
		 */
		printf("%s: removing SIGFPE and SIGSEGV from the signal mask...\n", argv[0]);
		sigdelset(&set, SIGFPE);
#if 1
		sigdelset(&set, SIGSEGV);
#endif
	/* 
	 * If trapped/caught via sigaction/signal, AND via sigwait, it is upto the
	 * implementation to decide how to handle the delivered signal. Linux seems
	 * to favour the sigwait.
	 */
		memset(&act, 0, sizeof(act));
		act.sa_handler = siggy;
		if (sigaction(SIGFPE, &act, 0) < 0)
			FATAL("sigaction on SIGFPE failed");
		printf("%s: all signals except SIGFPE and SIGSEGV blocked\n", argv[0]);
	}
	else
		printf("%s: All signals blocked (and only SIGFPE caught w/ sigaction)\n", argv[0]);

	/* MUST block the signals we expect (this includes RT sigs as well)
	  - else, a race is setup and it may not work correctly.
	*/
	if (sigprocmask(SIG_BLOCK, &set, 0) < 0)
		FATAL("sigprocmask -to block- failed\n");
	if (show_blocked_signals() < 0)
		WARN("sigprocmask -query- failed\n");

	printf("%s: waiting upon signals now ...\n", argv[0]);
	while (1) {
		if (sigwait(&set, &sig))
			FATAL("sigwait failed\n");
		printf("Received signal# %2d\n", sig);
	}
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
