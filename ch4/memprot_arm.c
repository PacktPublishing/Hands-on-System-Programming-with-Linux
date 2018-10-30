/*
 * ch4:memprot_arm.c
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
 * Same as the memprot program, except adjusted the variables and
 * printf formats for an ARM-32.
 * For details, please refer the book, Ch 4.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>		/* for mprotect(2) */
#include "../common.h"

/*---------------- Globals, Macros ----------------------------*/
int gPgsz;
int okornot[4];

#define TEST_READ(pgnum, addr) do { \
	printf("page %d: reading: byte @ 0x%lx is ", \
			pgnum, (u32)addr); \
	fflush(stdout); \
	printf(" %x", *addr); \
	printf(" [OK]\n"); \
} while (0)

#define TEST_WRITE(pgnum, addr, byte) do { \
	printf("page %d: writing: byte '%c' to address 0x%lx now ...", \
			pgnum, byte, (u32)addr); \
	fflush(stdout); \
	*addr = byte; \
	printf(" [OK]\n"); \
} while (0)

/*---------------- Typedef's, constants, etc ------------------*/
typedef unsigned long u32;

/*---------------- Functions ----------------------------------*/
static void test_mem(void *ptr, int write_on_ro_mem)
{
	int byte = random() % gPgsz;
	char *start_off = (char *)ptr + byte;

	printf("\n----- %s() -----\n", __FUNCTION__);

	/* Page 0 : rw [default] mem protection */
	if (okornot[0] == 1) {
		TEST_WRITE(0, start_off, 'a');
		TEST_READ(0, start_off);
	} else
		printf("*** Page 0 : skipping tests as memprot failed...\n");

	/* Page 1 : ro mem protection */
	if (okornot[1] == 1) {
		start_off = (char *)ptr + 1 * gPgsz + byte;
		TEST_READ(1, start_off);
		if (write_on_ro_mem == 1) {
			TEST_WRITE(1, start_off, 'b');
		}
	} else
		printf("*** Page 1 : skipping tests as memprot failed...\n");

	/* Page 2 : RWX mem protection */
	if (okornot[2] == 1) {
		start_off = (char *)ptr + 2 * gPgsz + byte;
		TEST_READ(2, start_off);
		TEST_WRITE(2, start_off, 'c');
	} else
		printf("*** Page 2 : skipping tests as memprot failed...\n");

	/* Page 3 : 'NONE' mem protection */
	if (okornot[3] == 1) {
		start_off = (char *)ptr + 3 * gPgsz + byte;
		TEST_READ(3, start_off);
		TEST_WRITE(3, start_off, 'd');
	} else
		printf("*** Page 3 : skipping tests as memprot failed...\n");
}

static void protect_mem(void *ptr)
{
	int i;
	u32 start_off = 0;
	char str_prots[][128] = { "PROT_READ|PROT_WRITE", "PROT_READ",
		"PROT_READ|PROT_WRITE|PROT_EXEC", "PROT_NONE"
	};
	int prots[4] = { PROT_READ | PROT_WRITE, PROT_READ,
		PROT_READ | PROT_WRITE | PROT_EXEC, PROT_NONE
	};

	printf("----- %s() -----\n", __FUNCTION__);
	memset(okornot, 0, sizeof(okornot));

	/* Loop over each page, setting protections as required */
	for (i = 0; i < 4; i++) {
		start_off = (u32) ptr + (i * gPgsz);
		printf("page %d: protections: %30s: "
		       "range [0x%lx, 0x%lx]\n",
		       i, str_prots[i], start_off, start_off + gPgsz - 1);

		if (mprotect((void *)start_off, gPgsz, prots[i]) == -1)
			WARN("mprotect(%s) failed\n", str_prots[i]);
		else
			okornot[i] = 1;
	}
}

int main(int argc, char **argv)
{
	void *ptr = NULL;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s test-write-to-ro-mem [0|1]\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	gPgsz = getpagesize();
#if 0
	/* FAILS */
	printf
	    ("Attempting to change text (of &main 0th page here %p) to [rwx] now...\n",
	     &main);
	if (mprotect((void *)&main, gPgsz, PROT_READ | PROT_WRITE | PROT_EXEC)
	    == -1)
		WARN("mprotect(%s) on main failed\n",
		     "PROT_READ|PROT_WRITE|PROT_EXEC");
#endif

	/* POSIX wants page-aligned memory for mprotect(2) */
	posix_memalign(&ptr, gPgsz, 4 * gPgsz);
	if (!ptr)
		FATAL("posix_memalign(for %zu bytes) failed\n", 4 * gPgsz);

	protect_mem(ptr);
	test_mem(ptr, atoi(argv[1]));

	free(ptr);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
