/*
 * A_iobuf/io_setbuf.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch A : File IO Essentials
 ****************************************************************
 * Brief Description:
 * Usage of the setvbuf(3) to demonstrate (stdio layer) IO buffering,
 * resulting in clear IO performance improvements.
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

#define MIN(x,y)  ((x)<(y)?(x):(y))
#define NREAD		   512

static int gVerbose = 1;
static char *gbuf;
// Our own IO buffers
static char *iow_buf, *ior_buf;
static size_t bufsz = 0;

static void testit(FILE * wrstrm, FILE * rdstrm, int numio)
{
	int i;
	size_t fnr = 0;

	VPRINT("numio=%d   total rdwr=%u   expected # rw syscalls=%ld\n",
	       numio, NREAD * numio, MIN(numio, NREAD * numio / bufsz));

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

/* Dump /proc/<pid>/io */
static void print_iostats(void)
{
	char cmd[128];
	snprintf(cmd, 127, "cat /proc/%d/io", getpid());
	(void)system(cmd);
}

int main(int argc, char **argv)
{
	char *fname = "/tmp/iobuf_tmp";
	FILE *wfp = NULL, *rfp = NULL;
	int numio;

	if (argc != 3) {
		fprintf(stderr,
			"Usage: %s new-iobuf-size number-of-times-to-rdwr\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	errno = 0;
	bufsz = strtoul(argv[1], 0, 0);
	if (errno == ERANGE || errno == EINVAL)
		FATAL("Invalid size input [strtoul]\n");

	numio = atoi(argv[2]);
	if (numio <= 0 || bufsz <= 0) {
		fprintf(stderr,
			"Usage: %s new-iobuf-size number-of-times-to-rdwr\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	printf("%s: setting our IO RW buffers to size %zu bytes; # IOs=%d\n",
	       argv[0], bufsz, numio);

	// Allocate our own 'write' stdio IO buffer
	iow_buf = malloc(bufsz);
	if (!iow_buf)
		FATAL("malloc %zu failed!\n", bufsz);

	// Allocate our own 'read' stdio IO buffer
	ior_buf = malloc(bufsz);
	if (!ior_buf)
		FATAL("malloc %zu failed!\n", bufsz);

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

	/* Change stdio buf: to fully buffered IO, and, importantly, with
	 * our specified buffer size 'bufsz'.
	 */
	if (setvbuf(rfp, ior_buf, _IOFBF, bufsz))
		FATAL("setvbuf on read buffer failed\n");
	if (setvbuf(wfp, iow_buf, _IOFBF, bufsz))
		FATAL("setvbuf on write buffer failed\n");

	testit(wfp, rfp, numio);
	fclose(rfp);
	fclose(wfp);
	print_iostats();
	free(gbuf);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
