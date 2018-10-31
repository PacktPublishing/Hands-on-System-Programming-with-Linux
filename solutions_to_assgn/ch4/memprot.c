/*
 * ch4 assessments solution : memprot.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From: Ch 4: Dynamic Memory Allocation
 ****************************************************************
 * ASSESSMENT Q2:
 * Q2. memprot: Allocate two pages of page-aligned memory.
 * Set memory protection as:
 * First page  : r-x
 * Second page : r--
 *
 * Write test cases to confirm its working as expected.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>       /* for mprotect(2) */

int gPgsz;

int main(int argc, char **argv)
{
	void *ptr = NULL;

	gPgsz = getpagesize();

	/* POSIX wants page-aligned memory for mprotect(2) */
	posix_memalign(&ptr, gPgsz, 2*gPgsz);
	if (!ptr) {
		fprintf(stderr,"posix_memalign(for %u bytes) failed\n", 4*gPgsz);
		exit(1);
	}

	//--- Test case 1: set page 0 to [r-x]
	printf("Attempting to set memory protections of %p to [r-x]...\n", ptr);
	if (mprotect((void *)ptr, gPgsz, PROT_READ|PROT_EXEC) == -1)
		fprintf(stderr," *** mprotect(r-x) failed\n");
	else
		printf(" [r-x] successful\n");
	/* Find that the above test case might fail! Why?
	 * It's really to do with kernel security modules disabling
	 * the mprotect(). On Fedora, the LSM is usually SELinux,
	 * on Ubuntu it's usually AppArmor responsible. 
	 * So, either disable the kernel LSM's or run on a 
	 * system with no kernel security modules installed.
	 * It should then work.
	 */
	
	//--- Test case 2: set page 1 to [r--]
	printf("Attempting to set memory protections of %p to [r--]...\n", ptr+gPgsz);
	if (mprotect((void *)ptr+gPgsz, gPgsz, PROT_READ) == -1)
		fprintf(stderr," *** mprotect(r--) failed\n");
	else
		printf(" [r--] successful\n");
	// Succeeds!
	
	free(ptr);
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
