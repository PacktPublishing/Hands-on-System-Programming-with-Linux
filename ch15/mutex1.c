/*
 * ch15/mutex1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 15 : Multithreading Part II - Synchronization
 ****************************************************************
 * Brief Description:
 * A small program to demonstrate a simple critical section and the usage of
 * a mutex lock to synchronize access to it.
 *
 * For details, please refer the book, Ch 15.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../common.h"

#define NTHREADS	3
#define MYCONST		3

static int locking=1;
static long g1=10, g2=12, g3=14;    /* our globals */
static pthread_mutex_t mylock;   /* lock to protect our globals */

static void * worker(void *data)
{
	long datum = (long)data + 1;
	int ret=0;

	if (locking) {
		if ((ret = pthread_mutex_lock(&mylock)))
			FATAL("pthread_mutex_lock failed! [%d]\n", ret);
	}

	/*--- Critical Section begins */
	g1 ++; g2 ++; g3 ++;
	printf("[Thread #%ld] %2ld   %2ld   %2ld\n", datum, g1, g2, g3);
	/*--- Critical Section ends */

	if (locking) {
		if ((ret = pthread_mutex_unlock(&mylock)))
			FATAL("pthread_mutex_unlock failed! [%d]\n", ret);
	}

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

static inline void usage(char *name)
{
	fprintf(stderr, "Usage: %s lock-or-not\n"
			" 0 : do Not lock (buggy!)\n"
			" 1 : do lock (correct)\n", name);
}

int main(int argc, char **argv)
{
	long i;
	int ret, stat=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;

	if (argc != 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if (atoi(argv[1]) == 0)
		locking = 0;
	else if (atoi(argv[1]) == 1)
		locking = 1;
	else {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	if (locking) {
		if ((ret = pthread_mutex_init(&mylock, NULL)))
			FATAL("pthread_mutex_init() failed! [%d]\n", ret);
	}

	printf("At start:   g1   g2   g3\n"
		"            %2ld   %2ld   %2ld\n", g1, g2, g3);

	// Thread creation loop
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_create(&tid[i], &attr, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		/*else
			printf(" Thread #%ld successfully joined; it "
				"terminated with status=%d\n", i, stat);*/
	}
	if (locking) {
		if ((ret = pthread_mutex_destroy(&mylock)))
			FATAL("pthread_mutex_destroy() failed! [%d]\n", ret);
	}

	pthread_exit(NULL);
}

/* vi: ts=8 */
