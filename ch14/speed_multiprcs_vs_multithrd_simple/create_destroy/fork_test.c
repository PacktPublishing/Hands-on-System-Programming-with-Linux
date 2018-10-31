/*
 * ch14/speed_multiprcs_vs_multithrd_simple/create_destroy/fork_test.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A quick test: speed of fork/wait/exit vs 
 * pthread_create/pthread_join/pthread_exit.
 *
 * For details, please refer the book, Ch 14.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../../common.h"

#define NFORKS 60000

void do_nothing(void)
{
	unsigned long f = 0xb00da;
}

int main(void)
{
	int pid, j, status;

	for (j = 0; j < NFORKS; j++) {
		switch (pid = fork()) {
		case -1:
			FATAL("fork failed! [%d]\n", pid);
		case 0:	// Child process
			do_nothing();
			exit(EXIT_SUCCESS);
		default:	// Parent process
			waitpid(pid, &status, 0);
		}
	}
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
