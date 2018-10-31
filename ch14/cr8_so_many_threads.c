/*
 * ch14/cr8_so_many_threads.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A small program to create as many threads as the user says. The parameter is
 * the number of threads to create. There must be a limit, yes?
 *
 * For details, please refer the book, Ch 14.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../common.h"

void * worker(void *data)
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
	long numthrds=0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s number-of-threads-to-create\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	numthrds = atol(argv[1]);
	if (numthrds <= 0) {
		fprintf(stderr, "Usage: %s number-of-threads-to-create\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < numthrds; i++) {
		ret = pthread_create(&tid, NULL, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() #%d failed! [%d]\n", i, ret);
	}
	pthread_exit(NULL);
}

/* vi: ts=8 */
