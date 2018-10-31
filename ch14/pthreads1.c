/*
 * ch14/pthreads1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A very simplistic (and in fact, incorrect - racy!) 'Hello, world' for
 * multithreading with Pthreads.
 *
 * For details, please refer the book, Ch 14.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../common.h"

#define NTHREADS	3

void * worker(void *data)
{
	long datum = (long)data;
	printf("Worker thread #%ld says: hello, world.\n", datum);
	printf(" #%ld: work done, exiting now\n", datum);
}

int main(void)
{
	long i;
	int ret;
	pthread_t tid;

	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_create(&tid, NULL, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
