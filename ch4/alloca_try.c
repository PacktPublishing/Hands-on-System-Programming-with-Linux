/*
 * ch4:alloca_try.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From: Ch 4: Dynamic Memory Allocation
 ****************************************************************
 * Brief Description:
 * We try out the alloca(3) API, noting how it does _not_ return
 * failure even when attempting to allocate far too large an
 * amount of memory. Only if memset() is used subsequently on
 * that memory, do we die due to a segmentation fault.
 * For details, please refer the book, Ch 4.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <alloca.h>
#include <string.h>
#include "../common.h"

static void try_alloca(const char *csz, int do_the_memset)
{
	size_t sz = atol(csz);
	void *aptr;

	aptr = alloca(sz);
	if (!aptr)
		FATAL("alloca(%zu) failed\n", sz);

	if (1 == do_the_memset)
		memset(aptr, 'a', sz);

	/* Must _not_ call free(), just return;
	 * the memory is auto-deallocated!
	 */
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s size-to-alloca do_the_memset[1|0]\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	if (atoi(argv[2]) == 1)
		try_alloca(argv[1], 1);
	else if (atoi(argv[2]) == 0)
		try_alloca(argv[1], 0);
	else {
		fprintf(stderr, "Usage: %s size-to-alloca do_the_memset[1|0]\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
