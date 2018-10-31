/*
 * A_iobuf/streamio1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch A : File IO Essentials
 ****************************************************************
 * Brief Description:
 * Open a stream (via the fopen(3) with either mode 'r' or 'a' depending on
 * the second parameter; then use the ftell(3) to query and report the current
 * 'seek' position.
 *
 * For details, please refer the book, Ch A, File I/O Essentials.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "../common.h"

int main(int argc, char **argv)
{
	FILE *fp = NULL;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s pathname {mode=b|e}\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (!strncmp(argv[2], "b", 1))
		fp = fopen(argv[1], "r");
	else if (!strncmp(argv[2], "e", 1))
		fp = fopen(argv[1], "a");
	else {
		fprintf(stderr, "Usage: %s pathname {mode=b|e}\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (!fp)
		FATAL("fopen on %s failed\n", argv[1]);

	printf("Current seek position: %ld\n", ftell(fp));
	fclose(fp);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
