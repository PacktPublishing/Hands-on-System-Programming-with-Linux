/*
 * ch10/fork4_prnum.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 * Brief Description:
 * A quick and simple demo of the fork(2) system call.
 * Same as fork4.c + here, we use a simple DELAY_LOOP macro to simulate
 * 'real work", printing p's and c's (for parent and child) as they run.
 *
 * For details, please refer the book, Ch 10.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

int main(int argc, char **argv)
{
	pid_t ret;

	if (argc != 3) {
		fprintf(stderr,
			"Usage: %s {child-numbytes-to-write} {parent-numbytes-to-write}\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	/* We leave the validation of the two parameters as a small
	 * exercise to you, dear reader :-)
	 */

	switch ((ret = fork())) {
	case -1:
		FATAL("fork failed, aborting!\n");
	case 0:		/* Child */
		printf("Child process, PID %d:\n"
		       " return %d from fork()\n", getpid(), ret);
		DELAY_LOOP('c', atoi(argv[1]));
		printf("Child process (%d) done, exiting ...\n", getpid());
		exit(EXIT_SUCCESS);
	default:		/* Parent */
		printf("Parent process, PID %d:\n"
		       " return %d from fork()\n", getpid(), ret);
		DELAY_LOOP('p', atoi(argv[2]));
	}
	printf("Parent (%d) will exit now...\n", getpid());
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
