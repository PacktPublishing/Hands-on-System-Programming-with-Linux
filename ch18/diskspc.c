/*
 * ch18/diskspc.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 18 : Advanced File IO
 ****************************************************************
 * Brief Description:
 * A simple experiment, using the posix_fallocate(3) API to, in effect,
 * 'reserve' the disk space specified for a file.
 *
 * For details, please refer the book, Ch 18.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common.h"

int main(int argc, char **argv)
{
	int fd;
	off_t sz;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s new-file-pathname size-to-reserve-in-MB\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	sz = atol(argv[2])*1024*1024;
	if (sz <= 0)
		FATAL("invalid size %s MB\n", argv[2]);

	unlink(argv[1]);
	umask(0);    // reset the permission bitmask
	fd = open(argv[1], O_CREAT|O_RDWR, 0644);
	if (fd < 0)
		FATAL("open on %s failed\n", argv[1]);

	if (posix_fallocate(fd, 0, sz) != 0)
		FATAL("posix_fallocate failed\n");

	close(fd);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
