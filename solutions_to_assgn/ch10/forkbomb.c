/*
 * Assignment Solution
 * solutions_to_assgn/ch10/forkbomb.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 *
Q5. forkbomb:
Develop a proof-of-concept (POC) forkbomb C application; the key point is
to use your knowledge of resource limits to limit the maximum number of processes
that can be created to 100 and limit the maximum data segment size to 1 GB.
Test to verify these limits are not breached.

Solution:
Try out the code below. Keep in mind: of the number of threads curr alive
exceeds the resource limit you're specifying, it wil lfail immediately.

[The situation is actually more complex on modern Linux desktops. The fact is,
pretty much all distros - we tried it on Fedora 28 and Ubuntu 18.04 - incorporate
control group (cgroup!) limits on number of tasks in any case. So, once one
exceeds this number, the kernel will fail the fork(2) and clone(2) syscalls with
the -EAGAIN error; the kernel log (dmesg(1) can show it might display something
like:

dmesg
...
cgroup: fork rejected by pids controller in /user.slice/user-1000.slice/session-2.scope
].

Look up the 'Further Reading' doc for more on Cgroups.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../../common.h"

#define ONE_GB  (1024*1024*1024)

static int setup_rlimits(rlim_t nprocs, rlim_t dataseg)
{
	struct rlimit rlim_new, rlim_old;

	/* Limit the # of processes (actually, threads) that can be created
	 * to 'nprocs' */
	rlim_new.rlim_cur = rlim_new.rlim_max = (rlim_t) nprocs;
	if (prlimit(0, RLIMIT_NPROC, &rlim_new, &rlim_old) == -1) {
		WARN("prlimit(2): nprocs failed\n");
		return -1;
	}
	printf(
	"NPROC rlimit [soft,hard] new [%ld:%ld] , old [%ld:%ld] (-1 = unlimited)\n",
	     rlim_new.rlim_cur, rlim_new.rlim_max,
	     rlim_old.rlim_cur, rlim_old.rlim_max);

	/* Limit the amount of data segment memory per process to 'dataseg' bytes */
	rlim_new.rlim_cur = rlim_new.rlim_max = (rlim_t) dataseg;
	if (prlimit(0, RLIMIT_DATA, &rlim_new, &rlim_old) == -1) {
		WARN("prlimit(2): nprocs failed\n");
		return -1;
	}
	printf(
	" DATA rlimit [soft,hard] new [%ld:%ld] , old [%ld:%ld] (-1 = unlimited)\n",
	     rlim_new.rlim_cur, rlim_new.rlim_max,
	     rlim_old.rlim_cur, rlim_old.rlim_max);
	return 0;
}

int main(int argc, char **argv)
{
	rlim_t maxtasks=0;
	int num=0, i = 1, j=0;
	pid_t cpid;
	void *p;
	size_t sz = 8 * getpagesize();

	if (argc != 3) {
out_usage:
		fprintf(stderr, "Usage: %s max-limit-on-tasks num ;\n"
			" 1st param: set the NPROC resource limit (# tasks max) to this number\n"
			" 2nd param: attempt to fork off approx 'num' processes\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	// TODO - validity checks on parameters passed!
	maxtasks = atoi(argv[1]);
	if (maxtasks < 1)
		goto out_usage;
	num = atoi(argv[1]);
	if (num < 1)
		goto out_usage;
	
	printf("--- Parent PID [%d]\n", getpid());
	setup_rlimits(num, ONE_GB);

	printf("\n--- Children :: \n");
	while (i <= num) {
		if ((cpid = fork()) < 0) {
			if (i == 1)
				fprintf(stderr,
				"Failing immediately; pl read the comments...\n");
			FATAL("fork() failed: i=%d\n", i);
		}
		p = malloc(sz);
		memset(p, 0, sz);
#if 0
		printf("%9d: P %9d C %9d\n", i++, getpid(), cpid);
#else
		// Indentation (silly)
#define COLS  80
		j=0;
		if (cpid) {
			for (j=0; j<i%COLS/2; j++)
				printf(" ");
			printf("| [i=%6d, PID %6d]\n", i, getpid());
		}
#endif
		
		if (cpid) // in the child
			sleep(2628000);  // ~ 1 month
		i ++;
	}

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
