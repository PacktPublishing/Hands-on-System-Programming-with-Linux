/*
 * ch10/fork5.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 * Brief Description:
 * fork4.c + testing of our 'rule #4': 
 *   "Data is copied across the fork."
 *
 * For details, please refer the book, Ch 10.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

static int g = 7;

int main(int argc, char **argv)
{
	pid_t ret;
	int loc = 8;

	switch ((ret = fork())) {
	case -1:
		FATAL("fork failed, aborting!\n");
	case 0:		/* Child */
		printf("Child process, PID %d:\n", getpid());
		loc++;
		g--;
		printf(" loc=%d g=%d\n", loc, g);
		printf("Child (%d) done, exiting ...\n", getpid());
		exit(EXIT_SUCCESS);
	default:		/* Parent */
#if 1
		sleep(2);	/* let the child run first */
#endif
		printf("Parent process, PID %d:\n", getpid());
		loc--;
		g++;
		printf(" loc=%d g=%d\n", loc, g);
	}
	printf("Parent (%d) will exit now...\n", getpid());
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
