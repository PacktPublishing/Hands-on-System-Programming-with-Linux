/*
 * Assignment Solution
 * solutions_to_assgn/ch10/smallbomb.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 * Q4. smallbomb: Write a C program that calls fork three times; keep a single
 * printf(3) after all the forks, displaying the PID. Interpret the resulting
 * output.
 *
Solution:
   The interpreted output:

$ ./smallbomb &
[3] 18051
PID 18051
PID 18053
PID 18051
PID 18053
PID 18054
PID 18051
PID 18053
PID 18054
PID 18057
PID 18058
PID 18055
PID 18056
PID 18055
PID 18060
$ 

Hard to interpret! Lets use the superb pstree(1) utility to help us:

$ pstree --ascii --highlight-pid=18051 --show-pids 18051
smallbomb(18051)-+-smallbomb(18053)-+-smallbomb(18055)---smallbomb(18060)
                 |                  `-smallbomb(18057)
                 |-smallbomb(18054)---smallbomb(18058)
                 `-smallbomb(18056)
$ 

We have a total of 8 processes (7 got spawned by this small bomb!). pstree(1)'s
output shows us the parent-child relationships, the 'tree'; useful! Why 8?
Its exponential: 2^3 = 8   (we called fork(2) thrice).

The first fork() resulted in a new child (C1); this child and its parent runs
the next line of code (recall out Fork Rule #1 : "After a successful fork,
execution in both the parent and child process continues at the instruction
following the fork"). So two processes (P and C1) call fork() again; thus we
land up with two newly born kids again (C2 and C3)! Who again call fork()
along with their parents (yielding C4, C5, C6 and C7)! Whew. 

Look at it like this:

                              P  [18051]
                              | F*                                 * F : fork()
	       -------------------------------
	       |                              |
	       P [18051]                      C1 [18053]
	       | F*                           | F*
        ------------------               --------------
	| F*              | F*           | F*          | F*
        P [18051]         C2 [18054]     C1            C3 [18055]
  -----------          ----------    -----------    ---------
  |          |         |        |    |         |    |       |  
  P          C4        C2       C5   C1        C6   C3      C7
[18051]    [18056]           [18057]         [18058]      [18060]

So, we have 1 original parent + 7 new children = 8 processes!
(That's why its called a 'fork bomb').
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../../common.h"

#define FORK_AND_PRINT  do {                              \
	if (fork() < 0)                                   \
		FATAL("PID %d: fork failed\n", getpid()); \
	printf("PID %d\n", getpid());                     \
	fflush(stdout);                                   \
} while(0)

int main(int argc, char **argv)
{
	FORK_AND_PRINT;
	FORK_AND_PRINT;
	FORK_AND_PRINT;

	pause();  // keep 'em all alive !

	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
