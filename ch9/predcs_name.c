/*
 * ch9/predcs_name.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 9 : Process Execution
 ****************************************************************
 * Brief Description:
 * [Paired with the 'successor_setnm.c' code].
 * A demo of using the execl(3) API; this is the first of two
 * programs. 
 * In this program, we pass a name (provided by the user)
 * to the second program - the successor - by passing it as the
 * 'argv[0]' value to the successor. However, we find that this
 * does not actually change the name of the successor process; the
 * right way to do so is to use the prctl(2) with the PR_SET_NAME
 * parameter, (or via the pthread_setname_np(3) Pthreads API) within
 * the successor itself.
 *
 * For details, please refer the book, Ch 9.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "../common.h"

int main(int argc, char **argv)
{
	char *err;

	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: %s {successor_name} [do-it-right]\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	/* Check for the successor file being there, with r.x permissions */
	if (access(argv[1], R_OK | X_OK) < 0) {
		if (errno == ENOENT)
			err = "is missing; please build it and retry.";
		else if (errno == EACCES)
			err =
			    "does not have sufficient permissions to execute.";
		else
			err = ": [other error]";
		FATAL("Successor file \"%s\" %s\n", argv[1], err);
	}

	/* Have us, the predecessor, exec the successor! 
	 * Notice how we "cleverly" pass argv[1] as the successor's
	 * argv[0] and argv[1]; hence, the thinking is, it will receive
	 * this name as "its" name (here, we're proved wrong though),
	 * and it's first parameter (that works). 
	 *
	 * So, to right the wrong, lets use this logic:
	 * if the user passes us 2 params, we'll pass them along to
	 * the successor; if the successor detects 3 params, it
	 * uses the 'correct' approach to naming. See the successor
	 * code for details on how this is done ...
	 */
	if (argc == 2) {	/* the wrong way */
		if (execl("./successor_setnm", argv[1], argv[1],
			  (char *)0) == -1)
			FATAL("execl \"successor_setnm\" 1 failed\n");
	} else if (argc == 3) {	/* the right way */
		if (execl("./successor_setnm", argv[2], argv[1],
			  argv[2], (char *)0) == -1)
			FATAL("execl \"successor_setnm\" 2 failed\n");
	}
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
