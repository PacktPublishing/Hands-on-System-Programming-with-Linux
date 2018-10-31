/*
 * solutions_to_assgn/ch3/segflt_nocore.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From: Ch 3 : Assignments
 ****************************************************************
 * ASSIGNMENT ::
 * Q1. Write a C program segflt_nocore that will crash and cause a 'segfault'.
 * Perform an operation on memory far beyond what you have malloc'ed and thus
 * crash. (We shall cover, in subsequent chapters, in a lot of detail, regarding
 * the malloc(3), segmentation fault, etc. For now, just go ahead with this).
 * Ensure that before running the resource limit for core dump is non-zero;
 * in fact, set it like this:
 * ulimit -c unlimited
 *
 * Now, on running your program, you should get a 'segfault' and thus a 'core'
 * file (it may have a different name/location, depending; see the chapters on
 * Signaling for details). 
 * We want you to, in this program, set the core file limit to zero, such that
 * even on a segfault, the core dump does Not occur.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../../common.h"

/*---------------- Globals, Macros ----------------------------*/

/*---------------- Typedef's, constants, etc ------------------*/

/*---------------- Functions ----------------------------------*/

int main(int argc, char **argv)
{
	struct rlimit rlim;
	char *p;

	/* Set core file resource limit to zero */
	printf("%s: setting core file resource limit to 0 ...\n", argv[0]);
	memset(&rlim, 0, sizeof(struct rlimit));
	if (prlimit(0, RLIMIT_CORE, &rlim, 0) < 0)
		FATAL("prlimit(2) on core failed\n");

	p = malloc(500);
	if (!p)
		FATAL("malloc 500 failed!\n");
	printf("\n0. p=malloc(500); p = %p\n", p+100);

	printf("\n1. Doing: *(p+100)   = 'a'; i.e. [%p]='a'; should work\n", p+100);
	*(p + 100) = 'a';

	printf("\n2. Doing: *(p+4000)  = 'b'; i.e. [%p]='b'; should Not work\n", p+4000);
	*(p + 4000) = 'b';
	printf("... but it does!\n"
		"Why? because the actual current program break is %p\n", sbrk(0));
	/* Details on the 'program break' will be covered in Ch 4 */

	printf("\n3. Doing: *(p+40000) = 'c'; i.e. [%p]='b'; should Not work\n", p+40000);
	*(p + 40000) = 'c';
	printf("... but it does!\n"
		"Why? because the actual current program break is %p\n", sbrk(0));

	printf("\n4. Doing: *(p+800000) = 'c'; i.e. [%p]='b'; should Not work\n", p+800000);
	*(p + 800000) = 'c';

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
