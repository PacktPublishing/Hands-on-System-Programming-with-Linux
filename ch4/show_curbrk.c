/*
 * ch4:show_curbrk.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:
 *  Ch 4 : Dynamic Memory Allocation
 ****************************************************************
 * Brief Description:
 * Two cases:
 * (1) If run without any parameters: displays the current program
 * break and exits
 * (2) If passed a parameter - the number of bytes of memory to
 * dynamically allocate - it does so (with malloc of course), then
 * prints the heap address returned as well as the original and
 * current program break.
 * For details, please refer the book, Ch 4.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include "../common.h"

int main(int argc, char **argv)
{
	char *heap_ptr;
	size_t num = 2048;

	/* No params, just print the current break and exit */
	if (argc == 1) {
		printf("Current program break: %p\n", sbrk(0));
		exit(EXIT_SUCCESS);
	}

	/* If passed a param - the number of bytes of memory to
	 * dynamically allocate -, perform a dynamic alloc, then
	 * print the heap address, the current break and exit.
	 */
	num = strtoul(argv[1], 0, 10);
	if ((errno == ERANGE && num == ULONG_MAX)
	    || (errno != 0 && num == 0))
		FATAL("strtoul(%s) failed!\n", argv[1]);
	if (num >= 128 * 1024)
		FATAL("%s: pl pass a value < 128 KB\n", argv[0]);

	printf("Original program break: %p ; ", sbrk(0));
	heap_ptr = malloc(num);
	if (!heap_ptr)
		FATAL("malloc failed!");
	printf("malloc(%lu) = %16p ; curr break = %16p\n",
	       num, heap_ptr, sbrk(0));
	free(heap_ptr);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
