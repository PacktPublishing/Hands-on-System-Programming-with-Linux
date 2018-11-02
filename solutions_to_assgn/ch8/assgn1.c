/*
 * Assignment Solution
 * solutions_to_assgn/ch8/assgn1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 8 : Process Capabilities
 ****************************************************************
 * Q1. Take our Ch 7, Process Credentials, ch7/query_creds.c program,
 * make a copy of it and add the CAP_CHOWN capability; then, within it, use
 * the chown(2) system call to change the ownership of a regular file to root.
 *
 * Solution:
 * In the Makefile, we add the CAP_CHOWN capability bit via the setcap(8);
 * here's the relevant lines for the target 'assgn1':
  	$(info Adding CAP_CHOWN to the binary executable file 'assgn1')
	sudo setcap cap_chown+ep ./assgn1
 * Notice that (a) we do so as root, (b) 'assgn1' is now a 'capability-dumb'
 * binary.
 * When run, though its not running as root, it now has the CAP_CHOWN capability,
 * thus the chown(2) - to root! - that we perform on the binary itself should
 * succeed. Check this out:

$ make assgn1
gcc -O2 -Wall -UDEBUG -c ../../common.c -o common.o
gcc -O2 -Wall -UDEBUG -c assgn1.c -o assgn1.o
Adding CAP_CHOWN to the binary executable file 'assgn1'
gcc -o assgn1 assgn1.o common.o 
sudo setcap cap_chown+ep ./assgn1      <--- adding CAP_CHOWN
[sudo] password for kai: xxx
$ getcap ./assgn1
./assgn1 = cap_chown+ep                <--- setcap(8) worked
$ ls -l assgn1
-rwxrwxr-x. 1 kai kai 13648 Nov  1 14:19 assgn1
$ ./assgn1 
RUID=1000 EUID=1000
RGID=1000 EGID=1000
$ ls -l assgn1
-rwxrwxr-x. 1 root kai 13648 Nov  1 14:19 assgn1  <--- after run, owner is 'root'!
               ^-- voila!
$ 

 * For details, please refer the book, Ch 8.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../../common.h"

#define SHOW_CREDS() do {		\
  printf("RUID=%d EUID=%d\n"	\
         "RGID=%d EGID=%d\n",	\
		getuid(), geteuid(),    \
		getgid(), getegid());   \
} while (0)

int main(int argc, char **argv)
{
	SHOW_CREDS();
	if (geteuid() == 0) {
		printf("%s now effectively running as root! ...\n", argv[0]);
		sleep(1);
	}
	if (chown(argv[0], 0, -1) < 0)
		FATAL("chown(2) failed\n");
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
