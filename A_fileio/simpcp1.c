/*
 * A_iobuf/simpcp1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch A : File IO Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A demo program that implements a very simplistic 'cp' - copy - program. It
 * copies the given 'source file' to a given 'destination' pathname. It *only*
 * works on 'regular' files. We do not provide any 'fancy' option switches.
 * The destination file is overwritten if it exists.
 *
 * For details, please refer the book, Ch A, File I/O Essentials.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common.h"

static int gVerbose = 0;

/* docopy
 * Figure out the size of the source file (via stat(2)); also get other
 * metadata from the inode.
 * Loop around, read from source file, write into dest file, until we
 * hit EOF.
 */
static int docopy(char *src, int fd_src, int fd_dest)
{
	char *buf = NULL;
	struct stat sb;
	size_t n = 0;
	ssize_t nr = 0, nw = 0;
	int loop = 1;

	memset(&sb, 0, sizeof(struct stat));
	if (stat(src, &sb) < 0) {
		WARN("stat on %s failed\n", src);
		return -1;
	}
	VPRINT("%s: size=%ld Block size for filesystem I/O = %ld bytes\n",
	       src, sb.st_size, sb.st_blksize);
	n = sb.st_blksize;	/* 'Optimal' block size for filesystem I/O */

	buf = malloc(sb.st_blksize);
	if (!buf) {
		WARN("malloc (%ld) failed!\n", sb.st_blksize);
		return -2;
	}

	VPRINT("Expected # loop iterations: %ld [+1 for rem %ld bytes]\n\n",
	       sb.st_size / n, sb.st_size % n);

	while (1) {
		nr = read(fd_src, buf, n);
		VPRINT("%06d: read %9ld ", loop, nr);
		if (nr < 0) {
			WARN("read on src failed\n");
			free(buf);
			return -3;
		} else if (nr == 0)
			break;

		nw = write(fd_dest, buf, nr);
		VPRINT("wrote %9ld\n", nw);
		if (nw < 0) {
			WARN("write on dest failed\n");
			free(buf);
			return -4;
		}
		loop++;
	}

	free(buf);
	return 0;
}

int main(int argc, char **argv)
{
	int fdr, fdw, ret = 0;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s source-file destination-file"
			" [1 for verbose-mode]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 4 && (!strcmp(argv[3], "1")))
		gVerbose = 1;

	/* Open source as read-only */
	fdr = open(argv[1], O_RDONLY);
	if (fdr < 0)
		FATAL("open on source file \"%s\" failed, aborting.\n",
		      argv[1]);

	/* Create / overwrite destination as write-only for append; mode=0644 */
	fdw = open(argv[2], O_CREAT | O_WRONLY,
		   S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (fdw < 0)
		FATAL("open on destination file \"%s\" failed, aborting.\n",
		      argv[2]);

	if ((ret = docopy(argv[1], fdr, fdw)) < 0) {
		close(fdr);
		close(fdw);
		FATAL("docopy() failed: (stat=%d)\n", ret);
	}
	if (gVerbose == 1)
		printf("\n");

	close(fdr);
	close(fdw);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
