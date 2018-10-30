/*
 * ch4:mlock_try.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From: Ch 4
 ****************************************************************
 * Brief Description:
 * We try out the mlock(2) API below, passing it page-aligned
 * memory (obtained via posix_memalign(3)). We lock the memory
 * region, sleep for a second (everyone needs a break, guys ;),
 * and then unlock it.
 *
 * If you attempt to lock more than the allowed resource limit
 * (64 KB by default), it fails, unless you run as root (or set
 * the CAP_IPC_LOCK capability bit - covered later).
 *
 * Also, we have the process query the amount of currently
 * locked memory it has via procfs.
 * For details, please refer the book, Ch 4.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>		// for mlock(),...
#include <string.h>
#include <limits.h>
#include "../common.h"

#define CMD_MAX  256

static void disp_locked_mem(void)
{
	char *cmd = malloc(CMD_MAX);
	if (!cmd)
		FATAL("malloc(%zu) failed\n", CMD_MAX);
	snprintf(cmd, CMD_MAX - 1, "grep Lck /proc/%d/status", getpid());
	system(cmd);
	free(cmd);
}

static void try_mlock(const char *cpgs)
{
	size_t num_pg = atol(cpgs);
	const long pgsz = sysconf(_SC_PAGESIZE);
	void *ptr = NULL;
	size_t len;

	len = num_pg * pgsz;
	if (len >= LONG_MAX)
		FATAL("too many bytes to alloc (%zu), aborting now\n", len);

	/* ptr = malloc(len); */
	/* Don't use the malloc; POSIX wants page-aligned memory for mlock */
	posix_memalign(&ptr, pgsz, len);
	if (!ptr)
		FATAL("posix_memalign(for %zu bytes) failed\n", len);

	/* Lock the memory region! */
	if (mlock(ptr, len)) {
		free(ptr);
		FATAL("mlock failed\n");
	}
	printf("Locked %zu bytes from address %p\n", len, ptr);
	memset(ptr, 'L', len);
	disp_locked_mem();
	sleep(1);

	/* Now unlock it.. */
	if (munlock(ptr, len)) {
		free(ptr);
		FATAL("munlock failed\n");
	}
	printf("unlocked..\n");
	free(ptr);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s pages-to-alloc\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	disp_locked_mem();
	try_mlock(argv[1]);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
