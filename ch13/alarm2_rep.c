/*
 * ch13/alarm2_rep.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 13 : Timers
 ****************************************************************
 * Brief Description:
 * Here, we just enhance the previous code (ch14/alarm1.c) to have the
 * timeout continually repeat.
 *
 * For details, please refer the book, Ch 13.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "../common.h"

/* Our simple signal handler */
static void sig_handler(int signum)
{
	const char *str = "   *** Timeout! [SIGALRM received] ***\n";

	if (signum != SIGALRM)
		return;
	if (write(STDOUT_FILENO, str, strlen(str)) < 0)
		WARN("write str failed!");
}

int main(int argc, char **argv)
{
	int n = 0;
	struct sigaction act;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s seconds-to-repetitive-timeout(>0)\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	n = atoi(argv[1]);
	if (n <= 0) {
		fprintf(stderr, "Usage: %s seconds-to-repetitive-timeout(>0)\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Init sigaction to defaults via the memset,
	 * setup 'sig_handler' as the signal handler function,
	 * trap just the SIGALRM signal.
	 */
	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_handler;
	if (sigaction(SIGALRM, &act, 0) < 0)
		FATAL("sigaction on SIGALRM failed");

	alarm(n);
	printf("A timeout for %ds has been armed...\n", n);
	/* (Manually) re-invoke the alarm every 'n' seconds */
	while (1) {
		pause();	/* wait for the signal ... */
		alarm(n);
		printf(" Timeout for %ds has been (re)armed...\n", n);
	}

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
