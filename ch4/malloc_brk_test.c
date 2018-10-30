/*
 * ch4:malloc_brk_test.c
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
 *
 * This program helps us study how the system allocates memory dynamically
 * to user-space applications via the usual malloc(3) / calloc(3) API.
 * For details, please refer the book, Ch 4.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <malloc.h>

/*---------------- Globals, Macros ----------------------------*/
#undef USE_CALLOC
#define TRIES 5

int gFlag_show_mstats, gFlag_large_allocs, gFlag_test_segfault1,
    gFlag_test_segfault2;
void *heap_ptr[TRIES];
void *init_brk;

/*---------------- Typedef's, constants, etc ------------------*/

/*---------------- Functions ----------------------------------*/
static void alloctest(int index, size_t num)
{
	void *p;

#ifndef USE_CALLOC
	p = malloc(num);
#else
	p = calloc(num, sizeof(char));
#endif
	if (!p) {
		fprintf(stderr, "out of memory!\n");
		exit(EXIT_FAILURE);
	}

	heap_ptr[index] = p;	// save ptr in order to free later..
#ifndef USE_CALLOC
	printf("%2d: malloc(%8lu) = ", index, num);
#else
	printf("%d: calloc", index);
#endif
	printf("%16p ", p);
	printf(" %16p [%lu]\n", sbrk(0), (sbrk(0) - init_brk));
	if (gFlag_show_mstats == 1) {
		malloc_stats();
		printf("\n");
	}
}

static void usage(char *name)
{
	fprintf(stderr,
		"Usage: %s [option | --help]\n"
		" option = 0 : show only mem pointers [default]\n"
		" option = 1 : opt 0 + show malloc stats as well\n"
		" option = 2 : opt 1 + perform larger alloc's (over MMAP_THRESHOLD)\n"
		" option = 3 : test segfault 1\n"
		" option = 4 : test segfault 2\n"
		"-h | --help : show this help screen\n", name);
}

static void process_args(int argc, char **argv)
{
	if ((argc == 2) && ((strncmp(argv[1], "-h", 2) == 0) ||
			    (strncmp(argv[1], "--help", 6) == 0))) {
		usage(argv[0]);
		exit(EXIT_SUCCESS);
	}

	if (argc == 2) {
		switch (atoi(argv[1])) {
		case 1:
			gFlag_show_mstats = 1;
			break;
		case 2:
			gFlag_show_mstats = gFlag_large_allocs = 1;
			break;
		case 3:
			gFlag_test_segfault1 = 1;
			break;
		case 4:
			gFlag_test_segfault2 = 1;
			break;
		default:
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv)
{
	int i = 0;
	volatile char *q;

	process_args(argc, argv);

	init_brk = sbrk(0);
	printf("                              init_brk = %16p\n", init_brk);
	printf
	    (" #: malloc(       n) =        heap_ptr           cur_brk   delta [cur_brk-init_brk]\n");

	alloctest(i++, 8);

	if (gFlag_test_segfault1 == 1) {
		q = heap_ptr[0];
		*(q + 3000) = 'a';	/* "should" segfault but does (probably) not bcoz a *page*
					 * or more is alloc'ed by the previous alloc, not just 8
					 * bytes! See value of prg break compared to this pointer.
					 */
		printf("### test segfault 1:\n"
		       " q=0x%lx ; (q+3000) is the mem loc 0x%lx ; mem here is '0x%x'.\n"
		       "###\n",
		       (unsigned long)q, (unsigned long)(q + 3000),
		       (unsigned int)*(q + 3000));
	}

	/* *Make* it segfault here by poking into a region 
	   just beyond what the kernel allocated */
	if (gFlag_test_segfault2 == 1) {
		q = heap_ptr[0];
		printf("### test segfault 2:\n"
		       " q=0x%lx ; (q+3000+(sbrk(0)-init_brk)) is the mem loc 0x%lx\n"
		       "###\n",
		       (unsigned long)q,
		       (unsigned long)(q + 3000 + (sbrk(0) - init_brk)));
		*(q + 3000 + (sbrk(0) - init_brk)) = 'b';
	}

	alloctest(i++, (getpagesize() - 8 - 5));
	alloctest(i++, 3);

	if (gFlag_large_allocs == 1) {
		alloctest(i++, (sbrk(0) - init_brk + 1000));
		/* This (above) allocation request is a large one: ~132Kb. The
		 * 'mmap threshold' is (default) 128Kb; thus, this causes an
		 * mmap() to the process virtual address space, mapping in the
		 * virtually allocated region (which will later be mapped to
		 * physical page frames via the MMU page-faulting on
		 * application access to these memory regions!
		 */
		alloctest(i++, 1024 * 1024);
	}

	while (i--)
		free(heap_ptr[i]);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
