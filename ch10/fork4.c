/*
 * ch10/fork4.c
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
 * This is a better version than the earlier ones; we take into account
 * some of our 'rules' of fork.
 *
 * For details, please refer the book, Ch 10.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

static void bar(unsigned int nsec)
{
	printf(" %s:%s :: will take a nap for %us ...\n",
	       __FILE__, __FUNCTION__, nsec);
	sleep(nsec);
}

static void foo(unsigned int nsec)
{
	printf(" %s:%s :: will take a nap for %us ...\n",
	       __FILE__, __FUNCTION__, nsec);
	sleep(nsec);
}

int main(int argc, char **argv)
{
	pid_t ret;

	if (argc != 3) {
		fprintf(stderr,
			"Usage: %s {child-alive-sec} {parent-alive-sec}\n",
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
		foo(atoi(argv[1]));
		printf("Child process (%d) done, exiting ...\n", getpid());
		exit(EXIT_SUCCESS);
	default:		/* Parent */
		printf("Parent process, PID %d:\n"
		       " return %d from fork()\n", getpid(), ret);
		bar(atoi(argv[2]));
	}
	printf("Parent (%d) will exit now...\n", getpid());
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
