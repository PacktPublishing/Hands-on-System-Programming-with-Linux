/*
 * solutions_to_assgn/A_fileio/redirct2.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:
 *  Ch A : File IO Essentials
 ****************************************************************
 * Q1. 
 * In our A_fileio/redirct.c program, we find that the printf data, though
 * written first, follows the fprintf to stderr write in the log file.
 * Modify the application to ensure that the printf data is written before
 * the fprintf to stderr.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "../../common.h"

int main(int argc, char **argv)
{
	FILE *fplog=NULL, *pfp=NULL, *fp=NULL;
#define DTMAX  256
	char dt[DTMAX];
	char *junk="abcxyz";

	if (argc != 2) {
		fprintf(stderr, "Usage: %s logfile\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Effectively redirect the stdout stream to our log file stream */
	fplog = freopen(argv[1], "a", stdout);
	if (!fplog)
		FATAL("freopen stdout on %s failed\n", argv[1]);
	/* Effectively redirect the stderr stream to our log file stream */
	fplog = freopen(argv[1], "a", stderr);
	if (!fplog)
		FATAL("freopen stderr on %s failed\n", argv[1]);

	/* Get human-readable timestamp via the popen(3) */
	if (!(pfp = popen("date", "r")))
		FATAL("popen (date) failed\n");
	if (!fgets(dt, DTMAX, pfp)) {
		WARN("fgets (date) failed\n");
		strncpy(dt, "---", 4);
	}
	dt[strlen(dt)-1]='\0';  // fgets: rm the terminating newline
	pclose(pfp);

	/* 1. Write to stdout */
	printf("%s:%s:current seek position: %ld\n", argv[0], dt, ftell(fplog));

	/**** Here's the key part: ****
	 * First flush the files, */
	fflush(fplog);
	fflush(stdout);
	/* then, explicitly sync them */
	if (fsync(fileno(fplog)) < 0)
		WARN("fsync 1 failed\n");
	if (fsync(fileno(stdout)) < 0)
		WARN("fsync 2 failed\n");

	/* 2. Deliberately cause an error, thus writing to stderr */
	if (!(fp = fopen(junk, "r"))) {
		fprintf(stderr, "\n%s:%s:fopen on %s failed\n", argv[0], dt, junk);
		perror(" fopen");
		fclose(fplog);
		exit(EXIT_SUCCESS);
	}

	fclose(fp);
	fclose(fplog);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
