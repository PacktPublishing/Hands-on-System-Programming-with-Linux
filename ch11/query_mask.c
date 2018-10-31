/*
 * ch11/query_mask.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 11 : Signalling Part I
 ****************************************************************
 * Brief Description:
 * A demo to show all signals that are currently blocked by the calling
 * process. The actual function code of show_blocked_signals() is in our
 * ../common.c source file.
 *
 * For details, please refer the book, Ch 11.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "../common.h"

/* Our simple signal handler */
static void my_handler(int signum)
{
	const char *str1 = "*** my_handler: handled SIGINT ***\n";
	const char *str2 = "*** my_handler: handled SIGQUIT ***\n";

	if (show_blocked_signals() < 0)
		FATAL("sigprocmask -query- failed\n");

	switch (signum) {
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

	/* Init sigaction:
	 * setup 'my_handler' as the signal handler function,
	 * trap just the SIGINT and SIGQUIT signals.
	 */
	memset(&act, 0, sizeof(act));
	act.sa_handler = my_handler;
	/* This is interesting: we fill the signal mask, implying that
	 * _all_ signals are masked (blocked) while the signal handler
	 * runs! */
	sigfillset(&act.sa_mask);

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
