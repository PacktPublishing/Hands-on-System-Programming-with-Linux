/* 
 * Assignment Solution
 * solutions_to_assgn/ch12/killer.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:
 *  Ch 12 : Signaling Part II
 ****************************************************************
 * Question/Exercise:

Q4. 'killer' : A quick question: a process traps a signal (say SIGINT) and
sets up a signal handler for it; later, it forks; the child process comes
alive and runs.
Have the new child just wait for a signal; then have the parent process send
SIGINT to its child. What will happen? Will the child inherit the signal
dispositions of its parent and thus enter the signal handler, or will the child
die abruptly (default action)? Write the 'killer' program and find out!

Soln: We find that the child process will indeed inherit the parent's signal
dispositions (and will therefore enter the signal handler code).
 */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "../../common.h"

/* Strictly speaking we should not use [f|s]printf() in a signal handler;
 * we do use it here in this demo for convenience...
 */
static void catchit(int signo)
{
	if (signo == SIGINT) {
		printf
		    ("catcher: process %d received signal SIGINT ; will now die.\n",
		     getpid());
		exit(0);
	} else
		printf("catchit: error, handler for SIGINT only\n");
}

int main(int argc, char **argv)
{
	pid_t pid, n;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = catchit;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;

	if (sigaction(SIGINT, &act, 0) == -1) {
		perror("killer: sigaction failed.");
		exit(1);
	}

	switch ((pid = fork())) {
	case -1:
		perror("fork failed");
		exit(1);
	case 0:		// Child
		(void)pause();	/* wait for signal */
		exit(0);	/* never get here */
	default:		// Parent      
		printf("parent is %d, child is %d\n", getpid(), pid);
		(void)sleep(1);

		if (kill(pid, 0) == 0) {	// iff child is alive ...
			/* send signal to child */
			if (kill(pid, SIGINT) == -1) {
				perror("kill failed");
				exit(1);
			}
		}

		if ((n = wait(0)) == -1) {
			perror("wait failed");
			exit(1);
		}
		printf("parent %d: child process fetched, wait "
		       "returned %d\n", getpid(), n);
	}
	return 0;
}

/* vi: ts=8 */
