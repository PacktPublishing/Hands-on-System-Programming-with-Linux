/*
 * ch10/fork_r6_of.c
 *   read it as 'rule6_openfiles'
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 * Brief Description:
 * This small program is used to demonstrate our 'fork rule #6':
 *    "Open files are (loosely) shared across the fork."
 * 
 * Thus, unless care is taken (via file locking), an open file
 * across the fork can get corrupted.
 * Here, we have that happen: both child and parent processes,
 * without any synchronization, happily write 100 lines of 'p'
 * and 'c' to a file. They "try" to get it right by performing 
 * the correct seek prior to writing, but it does not work in
 * practice - as the file's seek position is _shared_.
 *
 * For details, please refer the book, Ch 10.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common.h"

#define CHILD        1
#define PARENT       2

const int seekto = 10, lnsize = 80, numlines = 100;

static void work_on_file(int whom, int fd)
{
	char *buf = calloc(lnsize, sizeof(char));
	int i;

	if (!buf)
		FATAL("malloc failed\n");

	printf(" in %s:%s now...\n", __FILE__, __FUNCTION__);

	if (whom == CHILD) {
		/* Seek to offset 10, write 'numlines' lines of 'c's ... */
		printf("   context: child process\n");
		if (lseek(fd, seekto, SEEK_SET) < 0) {
			free(buf);
			close(fd);
			FATAL("child: lseek failed\n");
		}
		memset(buf, 'c', lnsize - 1);
		buf[lnsize - 1] = '\n';
		for (i = 0; i < numlines; i++) {
			if (write(fd, buf, lnsize) == -1) {
				free(buf);
				close(fd);
				FATAL("child: write failed\n");
			}
		}
	} else if (whom == PARENT) {
		/* Seek to offset 10+(80*100), write 'numlines' lines of 'p's ... */
		printf("   context: parent process\n");
		if (lseek(fd, seekto + (lnsize * numlines), SEEK_SET) < 0) {
			free(buf);
			close(fd);
			FATAL("parent: lseek failed\n");
		}
		memset(buf, 'p', lnsize - 1);
		buf[lnsize - 1] = '\n';
		for (i = 0; i < numlines; i++) {
			if (write(fd, buf, lnsize) == -1) {
				free(buf);
				close(fd);
				FATAL("parent: write failed\n");
			}
		}
	}
	free(buf);
}

static void do_work(int fd)
{
	pid_t ret;

	switch ((ret = fork())) {
	case -1:
		FATAL("fork failed, aborting!\n");
	case 0:		/* Child */
		printf("Child process, PID %d:\n", getpid());
		work_on_file(CHILD, fd);
		close(fd);

		printf("Child (%d) done, exiting ...\n", getpid());
		exit(EXIT_SUCCESS);
	default:		/* Parent */
#if 0
		sleep(1);	/* let the child run first */
#endif
		printf("Parent process, PID %d:\n", getpid());
		work_on_file(PARENT, fd);
		close(fd);
	}			// switch
}

int main(int argc, char **argv)
{
	int fd;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s {test-file-name}\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	unlink(argv[1]);

#define FILEMODE S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
	fd = open(argv[1], O_CREAT | O_TRUNC | O_RDWR, FILEMODE);
	if (fd == -1)
		FATAL("open(%s) failed\n", argv[1]);

	do_work(fd);

	printf("Parent (%d) will exit now...\n", getpid());
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
