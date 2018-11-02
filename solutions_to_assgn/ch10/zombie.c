/*
 * Assignment Solution
 * solutions_to_assgn/ch10/zombie.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 * Q9. zombie:
 * Write a C program that generates a zombie process. Verify the same
 * by running it in the background and checking the output of ps -l (it should
 * show a Z in the second column and mark the process as being <defunct>).
 *
 * Solution:
 * Small fork() demo program to generate a zombie; we have the parent process
 * fork, and have the child immediately terminate. Importantly, we do Not have
 * the parent perform the wait(2) (it just sleeps for a while); thus, the child
 * becomes a zombie!
 *
 * A trial run:

$ ./zombie &
[2] 19077
Parent pid 19077 : p=19079; sleeping now for 300s withoutwait()-ing (resulting in a zombie!)..
Child pid 19079 : p=0; exiting now ...
$ ps -l
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S  1000  6389  3885  0  80   0 - 57079 -      pts/1    00:00:00 bash
0 S  1000  6516  6389  0  80   0 - 280695 restar pts/1   00:00:05 gitg
0 S  1000  6683  1915  0  80   0 - 54848 restar pts/1    00:00:00 git-credential-
0 S  1000 19077  6389  0  80   0 -  1079 hrtime pts/1    00:00:00 zombie
1 Z  1000 19079 19077  0  80   0 -     0 -      pts/1    00:00:00 zombie <defunct>
4 R  1000 19085  6389  0  80   0 - 61596 -      pts/1    00:00:00 ps
$ kill 19077
[2]+  Terminated              ./zombie
$ ps -l
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S  1000  6389  3885  0  80   0 - 57079 -      pts/1    00:00:00 bash
0 S  1000  6516  6389  0  80   0 - 280695 restar pts/1   00:00:05 gitg
0 S  1000  6683  1915  0  80   0 - 54848 restar pts/1    00:00:00 git-credential-
4 R  1000 19100  6389  0  80   0 - 61596 -      pts/1    00:00:00 ps
$ 
 
Notice how killing the parent has the Linux kernel optimize by getting rid of
any and all zombies for that parent process; as, without the parent, of what
use is the zombie(s)?
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "../../common.h"

int main(int argc, char **argv)
{
	pid_t p;

	p = fork();
	switch (p) {	/* often coded as switch( (p=fork()) ) */
	case -1:
		FATAL("fork failed\n");
	case 0:		// Child
		printf
		    ("Child pid %d : p=%d; exiting now ...\n",
		     getpid(), p);
		exit(EXIT_SUCCESS);
	default:	// Parent
		printf("Parent pid %d : p=%d; sleeping now for 300s without"
			"wait()-ing (resulting in a zombie!)..\n", getpid(), p);
		sleep(300);
		printf("Parent: sleep done, exiting. Notice how (on Linux) the"
			" zombie is now cleared!\n");
		exit(EXIT_SUCCESS);
	}
}

/* vi: ts=8 */
