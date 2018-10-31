/*
 * ch11/sig2.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 11 : Signaling Part I
 ****************************************************************
 * Brief Description:
 * A simple signal handling program: here, we 'trap' two signals -
 * SIGINT and SIGQUIT - via the sigaction(2) system call, and demonstrate
 * a simple signal handler function that 'handles' them, writing out a message.
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
static void siggy(int signum)
{
	const char *str1 = "*** siggy: handled SIGINT ***\n";
	const char *str2 = "*** siggy: handled SIGQUIT ***\n";

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

	/* Init sigaction to defaults via the memset,
	 * setup 'siggy' as the signal handler function,
	 * trap just the SIGINT and SIGQUIT signals.
	 */
	memset(&act, 0, sizeof(act));
	act.sa_handler = siggy;
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
