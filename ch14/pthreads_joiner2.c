/*
 * ch14/pthreads_joiner2.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * Built upon pthreads_joiner1.c: everything remains the same, except that
 * here we reduce the sleep time of each successive worker thread by 1s.
 * THis way, the last thread created dies first, but because the join is
 * performed in a loop that iterates over the pthread_join in order from
 * #0, #1 and #2, the threads will be "joined" in that order only.
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
	int slptm=8;

	printf(" worker #%ld: will sleep for %lds now ...\n", datum, (slptm-datum));
	sleep(slptm-datum);
	printf(" worker #%ld: work (eyeroll) done, exiting now\n", datum);

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

int main(void)
{
	long i;
	int ret, stat=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Thread creation loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: creating thread #%ld ...\n", i);
		ret = pthread_create(&tid[i], &attr, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: joining (waiting) upon thread #%ld ...\n", i);
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		else
			printf("Thread #%ld successfully joined; it terminated with "
				"status=%d\n", i, stat);
	}

	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
}

/* vi: ts=8 */
