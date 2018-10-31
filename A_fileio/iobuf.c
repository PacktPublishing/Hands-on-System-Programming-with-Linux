/*
 * A_iobuf/iobuf.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch A : File IO Part I - Essentials
 ****************************************************************
 * Brief Description:
 * Library (stdio) layer file IO APIs demo.
 *
 * For details, please refer the book, Ch A, File I/O Essentials.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common.h"

#define NREAD	512

static int gVerbose = 1;
static char *gbuf = NULL;

static void testit(FILE * wrstrm, FILE * rdstrm, int numio)
{
	int i, syscalls = NREAD * numio / getpagesize();
	size_t fnr = 0;

	if (syscalls <= 0)
		syscalls = 1;
	VPRINT("numio=%d   total rdwr=%u   expected # rw syscalls=%d\n",
	       numio, NREAD * numio, syscalls);

	for (i = 0; i < numio; i++) {
		fnr = fread(gbuf, 1, NREAD, rdstrm);
		if (!fnr)
			FATAL("fread on /dev/urandom failed\n");

		if (!fwrite(gbuf, 1, fnr, wrstrm)) {
			free(gbuf);
			if (feof(wrstrm))
				return;
			if (ferror(wrstrm))
				FATAL("fwrite on our file failed\n");
		}
	}
}

int main(int argc, char **argv)
{
	char *fname = "/tmp/iobuf_tmp";
	FILE *wfp = NULL, *rfp = NULL;
	int numio;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s number-of-times-to-rdwr\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	numio = atoi(argv[1]);
	if (numio <= 0) {
		fprintf(stderr, "Usage: %s number-of-times-to-rdwr\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf
	    ("%s: using default stdio IO RW buffers of size %u bytes; # IOs=%d\n",
	     argv[0], getpagesize(), numio);

	gbuf = malloc(NREAD);
	if (!gbuf)
		FATAL("malloc %zu failed!\n", NREAD);

	unlink(fname);
	wfp = fopen(fname, "a");
	if (!wfp)
		FATAL("fopen on %s failed\n", fname);

	rfp = fopen("/dev/urandom", "r");
	if (!rfp)
		FATAL("fopen on /dev/urandom failed\n");

	testit(wfp, rfp, numio);
	fclose(rfp);
	fclose(wfp);
	free(gbuf);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
