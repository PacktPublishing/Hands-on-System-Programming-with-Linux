/*
 * solutions_to_assgn/ch12/sigtmwtinfo.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 ****************************************************************
 * Assignment:

Q5. 'sigtmwtinfo' : Enhance our ch12/sigwt/sigwt program to use the
sigtimedwait(2) API. Set the timeout at 1.5 seconds. If no signals is received
within this time, print a simple timed out message to the console and continue
waiting. Further, trap fatal kernel signals (such as SIGSEGV and SIGFPE) via
the sigaction(2) API (and not via the sigtimedwait(2)). Arrange to have the
kernel generate SIGSEGV (by deliberately inserting a bug in your code), and
handle the resultant segfault, displaying diagnostics (as in the first Q1
assignment).

 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "../../common.h"

#define APPNAME "sigtmwtinfo"

static void fatal_sig_handler(int signum, siginfo_t *si, void *ucontext)
{
	switch (signum) {
	case SIGSEGV: 
		fprintf(stderr,
			"%s:\n------------------- FATAL signal ---------------------------\n",
			APPNAME);
		fprintf(stderr, " *** FATAL signal %d :: \n", signum);
		fprintf(stderr, " Faulting insn/memory address: %p\n", si->si_addr);
		fprintf(stderr, " Origin: ");
		switch (si->si_code) {
		case SEGV_MAPERR:
			fprintf(stderr, "SEGV_MAPERR : Address not mapped to object.\n");
			break;
		case SEGV_ACCERR:
			fprintf(stderr, "SEGV_ACCERR : Invalid permissions for mapped object.\n");
			break;
#if 0
		case SEGV_BNDERR: // 3.19 onwards
			fprintf(stderr, " SEGV_BNDERR : Failed address bound checks..\n");
			break;
		case SEGV_PKUERR: // 4.6 onwards
			fprintf(stderr, " SEGV_PKUERR : Access was denied by memory protection keys.\n");
			break;
#endif
		}
		fprintf(stderr, "%s: Aborting ...\n", APPNAME);
		fprintf(stderr,
			"------------------------------------------------------------\n");

		/* Have the kernel generate the core dump:
		 *  Reset the SIGSEGV to glibc default, and,
		 *  Re-raise it!
		 */
		if (signal(SIGSEGV, SIG_DFL) == SIG_ERR)
			FATAL("signal -reverting SIGSEGV to default- failed");
		if (raise(SIGSEGV))
			FATAL("raise SIGSEGV failed");
		break;
	}
}

/* The classic NULL pointer dereference! */
void buggy(void)
{
     int *i = NULL;
     *i = 0;
}

int main(int argc, char **argv)
{
	struct sigaction act;
	sigset_t set;
	int sig=0;
	struct timespec tm;
	siginfo_t si;

	if (argc != 2 ) {
		fprintf(stderr, "Usage: %s 0|1\n"
				" 0 => no segfault bug\n"
				" 1 => segfault bug present\n"
				, argv[0]);
		exit(EXIT_FAILURE);
	}
	sigfillset(&set);  /* the '0' case - block all signals */

	/* IMP: unblocking signals here removes them from the influence of 
	 * the sigwait* APIs; this is *required* for correctly handling
	 * fatal signals from the kernel.
	 */
	printf("%s: removing SIGFPE and SIGSEGV from the signal mask...\n", argv[0]);
	sigdelset(&set, SIGFPE);
	sigdelset(&set, SIGSEGV);

	/* 
	 * If trapped/caught via sigaction/signal, AND via sigwait, it is upto the
	 * implementation to decide how to handle the delivered signal. Linux seems
	 * to favour the sigwait.
	 */
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = fatal_sig_handler;
	act.sa_flags = SA_RESTART | SA_SIGINFO;
	if (sigaction(SIGSEGV, &act, 0) < 0)
		FATAL("sigaction on SIGSEGV failed");
	if (sigaction(SIGFPE, &act, 0) < 0)
		FATAL("sigaction on SIGFPE failed");
	printf("%s: All signals blocked (except that SIGSEGV and SIGFPE caught w/ sigaction)\n"
		, argv[0]);

	/* MUST block the signals we expect (this includes RT sigs as well)
	  - else, a race is setup and it may not work correctly.
	*/
	if (sigprocmask(SIG_BLOCK, &set, 0) < 0)
		FATAL("sigprocmask -to block- failed\n");
	if (show_blocked_signals() < 0)
		WARN("sigprocmask -query- failed\n");

	tm.tv_sec = 1;
	tm.tv_nsec = 100000000; // 0.5s
	printf("%s: waiting upon signals now (bugginess setting is %s)...\n", 
			argv[0], atoi(argv[1]) == 1?"Y":"N");
	while (1) {
		if ((sig = sigtimedwait(&set, &si, &tm)) < 0) {
			if (errno == EAGAIN) {
				printf("<timeout>! retrying ...\n");
				if (atoi(argv[1]) == 1)
					buggy();
				continue;
			} else
				FATAL("sigtimedwait failed\n");
		}
		printf("Received signal# %2d\n", sig);
	}
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
