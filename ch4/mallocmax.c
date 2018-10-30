/*
 * ch4:mallocmax.c
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
 * How much can malloc(3) actually allocate?
 * A few test cases ...
 * For details, please refer the book, Ch 4.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include "../common.h"

/*---------------- Globals, Macros ----------------------------*/

/*---------------- Typedef's, constants, etc ------------------*/
typedef unsigned long long u64;

/*---------------- Functions ----------------------------------*/

static void max_malloc(void)
{
	int szt = sizeof(size_t);
	float max = 0;

	printf("*** %s() ***\n", __FUNCTION__);
	max = pow(2, szt * 8);
	printf("sizeof size_t = %u; "
	       "max value of the param to malloc = %.0f\n", szt, max);
}

static void negative_malloc(void)
{
	int szt = sizeof(size_t);
	float max = 0;
	/* const size_t oneGB =       1073741824; */
	/* const size_t oneTB =    1099511628000; */
	const size_t onePB = 1125899907000000;	/* 1 petabyte */
	int qa = 28 * 1000000;

	long int ld_num2alloc = qa * onePB;
	size_t szt_num2alloc = qa * onePB;
	short int sd_num2alloc;
	void *ptr;

	printf("*** %s() ***\n", __FUNCTION__);

	max = pow(2, szt * 8);
	printf("size_t max    = %.0f\n", max);

	printf("ld_num2alloc  = %ld\nszt_num2alloc = %zu\n",
	       ld_num2alloc, szt_num2alloc);

	ptr = malloc(ld_num2alloc);
	printf("1. long int used:  malloc(%ld) returns %p\n", ld_num2alloc,
	       ptr);
	free(ptr);		/* free(NULL) is handled */

	ptr = malloc(szt_num2alloc);
	printf("2. size_t used:    malloc(%zu) returns %p\n", szt_num2alloc,
	       ptr);
	free(ptr);

	sd_num2alloc = 6 * 1024;
	ptr = malloc(sd_num2alloc);
	printf("3. short int used: malloc(%d) returns %p\n", sd_num2alloc, ptr);
	free(ptr);

	sd_num2alloc = 60 * 1024;
	ptr = malloc(sd_num2alloc);
	printf("4. short int used: malloc(%d) returns %p\n", sd_num2alloc, ptr);
	free(ptr);

	szt_num2alloc = sd_num2alloc;
	ptr = malloc(szt_num2alloc);
	printf("5. size_t used:    malloc(%zu) returns %p\n", szt_num2alloc,
	       ptr);
	free(ptr);

	ptr = malloc(-1UL);
	printf("6.                 malloc(%lu) returns %p\n", -1UL, ptr);
	free(ptr);

	ptr = malloc(0);
	printf("7.                 malloc(0) returns %p\n", ptr);
	free(ptr);
}

int main(void)
{
	max_malloc();
	negative_malloc();
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
