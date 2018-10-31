/*
 * solutions_to_assgn/ch9/exec_pid_poc.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 ****************************************************************
 * Q1. We learned that the 'exec' axiom demands that both the predecessor
 * and successor processes have the same PID; write a small
 * proof-of-concept (POC) program to prove this statement
 * (Tip: just displaying the PIDs in some fashion will suffice).
 * -------------
 * To compile this program:
 *   gcc exec_pid_poc.c -o exec_pid_poc -Wall
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/* 
 * TOC :: Theory Of Operation
 *
 * Often, one would solve this POC by writing two small 'C'
 * programs, both of which print their PID value. We would
 * then arrange for one (the predecessor) to print it's PID
 * of course, and then invoke the other (the successor).
 * Yes, it would work. But there's a simpler way:
 * Have us, the predecessor, print our PID and then exec
 * 'ps' as the successor!
 * We do this as we realize that ps prints it's own PID,
 * thus proving the point (you will see that the PIDs match)!
 */

int main(int argc, char **argv)
{
	printf("Predecessor [%s]: PID is %d\n", argv[0], getpid());
	if (execlp("ps", "ps", (char *)0) == -1) {
		perror("execl(ps) failed");
		exit (EXIT_FAILURE);
	}
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
