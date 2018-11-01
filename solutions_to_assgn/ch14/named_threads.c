/*
 * solutions_to_assgn/ch14/named_threads.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Qs. 
 * Extend the cr8_so_many_threads.c program to give each individual thread a
 * unique name prefix as specified by the user (of the form
 * "<name-prefix>_<thread#>" ;
$ ./named_threads
Usage: ./named_threads thread-name-prefix number-of-threads-to-create
$
 * Refer Ch 14 for details, thank you.
 */
#include "../../common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *worker(void *data)
{
	long datum = (long)data;
	printf("Worker thread #%5ld: pausing now...\n", datum);
	(void)pause();
	printf(" #%5ld: work done, exiting now\n", datum);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	long i;
	int ret;
	pthread_t tid;
	long numthrds = 0;
#define MAXLEN  11
	char nm[MAXLEN];

	if (argc != 3) {
		fprintf(stderr,
			"Usage: %s thread-name-prefix number-of-threads-to-create\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	
	numthrds = atol(argv[2]);
	if (numthrds <= 0) {
		fprintf(stderr,
			"Usage: %s thread-name-prefix number-of-threads-to-create\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	if (strlen(argv[1]) >= MAXLEN-3) {
		fprintf(stderr, "%s: thread-name-prefix  \"%s\" is too long (max %d bytes)\n",
				argv[0], argv[1], MAXLEN-4);
		exit(EXIT_FAILURE);
	}


	for (i = 0; i < numthrds; i++) {
		ret = pthread_create(&tid, NULL, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() #%d failed! [%d]\n", i,
			      ret);
		/* Name the thread via the pthread_setname_np(3) API; note that
		 * its Linux-specific and non-portable.
		 */
		memset(nm, 0, MAXLEN);
		snprintf(nm, MAXLEN, "%s_%ld", argv[1], i);
		if ((ret = pthread_setname_np(tid, nm)))
			FATAL("pthread_setname_np (%d) failed! [%d]\n",
			      i, ret);
	}
	pthread_exit(NULL);
}
/* vi: ts=8 */
