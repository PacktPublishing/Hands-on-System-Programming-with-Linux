/*
 * A_iobuf/mode_def.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch A : File IO Essentials
 ****************************************************************
 * Brief Description:
 * Demonstrate correctly setting file permission bits at creation time.
 *
 * For details, please refer the book, Ch A, File I/O Essentials.
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

	if (argc != 2) {
		fprintf(stderr, "Usage: %s new-file-pathname\n", argv[0]);
		exit(EXIT_FAILURE);
	}
#if 0				// the buggy way
	fd = open(argv[1], O_CREAT | O_RDONLY);
#else				// the right way
	umask(0);		// reset the permission bitmask
	fd = open(argv[1], O_CREAT,
		  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
#endif
	if (fd < 0)
		FATAL("open on %s failed\n", argv[1]);

	printf("Current seek position: %ld\n", lseek(fd, 0, SEEK_CUR));
	close(fd);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
