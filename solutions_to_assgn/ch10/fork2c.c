/*
 * Assignment Solution
 * solutions_to_assgn/ch10/fork2c.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 *
 * Q7. fork2c:
 * A process P forks (twice), thus creating two children, C1 and C2.
 * Have the children stay alive performing some mundane task, and then have them
 * terminate. Let C1 terminate in 10s (seconds) and C2 in 8s. When they die, the
 * parent, P, must "fetch" their termination status information and print it out.
 *
 * Solution:
 * Here, we have the two kids just sleep; by default, the first child for 3s
 * and the second child for 7s. If the user passes along two integer values as
 * parameters, we have the children sleep for those times resp.
 * A key point: correctly creating the children implies that the parent, after
 * creating the first child, does not simply call fork(2); then both the new child
 * and parent run the code creating 4 processes. So, no, we have the child sleep
 * and the in it's own code the parent calls fork a second time (pl follow the code
 * below). 
 * Next, we have the parent wait upon the termination of any number of children
 * by invoking the wait() within a while loop, which breaks when the wait()
 * returns an error. The parent prints all the info about how and why the child
 * terminated as well.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../../common.h"

static void displayChildStatus(int stat, pid_t cpid)
{
	printf("\nChild %d just died!\n"
	       "WIFEXITED : %d\n"
	       "Exit status: %d\n"
	       "WIFSIGNALED : %d\n"
	       "Termination signal: %d\n"
	       "WIFSTOPPED : %d\n"
	       "Stop signal: %d\n"
	       "WCOREDUMP: %d\n"
	       "WIFCONTINUED: %d\n",
	       cpid,
	       WIFEXITED(stat),
	       WEXITSTATUS(stat),
	       WIFSIGNALED(stat),
	       WTERMSIG(stat),
	       WIFSTOPPED(stat),
	       WSTOPSIG(stat), WCOREDUMP(stat), WIFCONTINUED(stat)
	    );
}

static void createChild(int sleep_time)
{
	pid_t n;

	switch (n = fork()) {
	case -1:
		perror("fork");
		exit(1);
	case 0:		// Child
		printf("Child 2 PID %d sleeping for %ds...\n", getpid(),
		       sleep_time);
		sleep(sleep_time);
		exit(0);
	default:;	// Parent returns..
	}
}

static int chkrange(int num, int min, int max)
{
	if (num < min)
		return -2;
	else if (num > max)
		return -3;
	return 0;
}

int main(int argc, char **argv)
{
	int stat = 0;
	unsigned short int c1_slptm = 3, c2_slptm = 7;
	pid_t n, cpid;

	if (argc == 3) {
		c1_slptm = atoi(argv[1]);
		if (chkrange(c1_slptm, 1, 60) < 0) {
			fprintf(stderr,
				"%s: ensure first param [%u] is within acceptable"
				" range [1-60]\n", argv[0], c1_slptm);
			exit(EXIT_FAILURE);
		}
		c2_slptm = atoi(argv[2]);
		if (chkrange(c2_slptm, 1, 60) < 0) {
			fprintf(stderr,
				"%s: ensure second param [%u] is within acceptable"
				" range [1-60]\n", argv[0], c2_slptm);
			exit(EXIT_FAILURE);
		}
	}

	switch (n = fork()) {
	case -1:
		perror("fork");
		exit(1);
	case 0:		// Child
		printf("Child 1 PID %d sleeping for %ds...\n", getpid(),
		       c1_slptm);
		sleep(c1_slptm);
		exit(EXIT_SUCCESS);
	default:	// Parent
		createChild(c2_slptm);
		/* (Synchronously) Wait for all children to die */
		while ((cpid = wait(&stat)) != -1) {
			displayChildStatus(stat, cpid);
		}
	}
	printf("\nParent: all children dead, exiting...\n");
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
