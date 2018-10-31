/*
 * ch15/facto.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 15 : Multithreading Part II - Synchronization
 ****************************************************************
 * Brief Description:
 * A small program to demonstrate how a data "race" can be simulated. Here,
 * we have two threads run in parallel calculating the factorial of two given
 * numbers. With locking on, it obviously works correctly yielding the expected
 * results. With locking set to 'Off' though, the results are sometimes
 * spectacularly wrong!
 * Tip: turn on the Verbose mode and run it, increasing the chances of a 'race'
 * occuring.
 *
 * For details, please refer the book, Ch 15.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../common.h"

#define NTHREADS	2

static int gLocking=1, gVerbose=0;
static unsigned long long gFactorial=1;
static pthread_mutex_t mylock;

/*
 * This is the function that calculates the factorial of the given parameter.
 * Stress it, making it susceptible to the data race, by turning verbose mode
 * On; then, it will take more time to execute, and likely end up "racing" on
 * the value of the global gFactorial.
 */
static void factorize(int num)
{
	int i;

	gFactorial = 1;
	if (num <= 0)
		return;

	for (i=1; i<=num; i++) {
		gFactorial *= i;
		VPRINT(" i=%2d  fact=%20lld\n", i, gFactorial);
	}
}

/*
 * Worker threads: have them work this way:
 * Thread #1 : calculate 10!
 * Thread #2 : calculate 12! 
 */
static void * worker(void *data)
{
	long datum = (long)data + 1;
	int N=0, ret=0;

	switch (datum) {
		case 1 : N = 10;
			 break;
		case 2 : N = 12;
			 break;
		default: WARN("datum = %ld ! [should never occur]!\n", datum);
	}

	if (gLocking) {
		if ((ret = pthread_mutex_lock(&mylock)))
			FATAL("pthread_mutex_lock failed! [%d]\n", ret);
	}

	/*--- Critical Section begins! */
	factorize(N);
	printf("[Thread #%ld] (factorial) %d ! = %20lld\n",
			datum, N, gFactorial);
	/*--- Critical Section ends */

	if (gLocking) {
		if ((ret = pthread_mutex_unlock(&mylock)))
			FATAL("pthread_mutex_unlock failed! [%d]\n", ret);
	}

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

static inline void usage(char *name)
{
	fprintf(stderr, "Usage: %s lock-or-not [verbose=[0]|1]\n"
			"Locking mode:\n"
			" 0 : do Not lock (buggy!)\n"
			" 1 : do lock (correct)\n"
			"(TIP: turn locking OFF and verbose mode ON to see the issue!)\n",
			name);
}

int main(int argc, char **argv)
{
	long i;
	int ret, stat=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;

	if (argc < 2) {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if (atoi(argv[1]) == 0)
		gLocking = 0;
	else if (atoi(argv[1]) == 1)
		gLocking = 1;
	else {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 3)
		gVerbose = atoi(argv[2]);

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	printf( "Locking mode : %s\n" 
		"Verbose mode : %s\n",
			(gLocking == 1?"ON":"OFF"),
			(gVerbose == 1?"ON":"OFF"));
	if (gLocking) {
		if ((ret = pthread_mutex_init(&mylock, NULL)))
			FATAL("pthread_mutex_init() failed! [%d]\n", ret);
	}

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
	if (gLocking) {
		if ((ret = pthread_mutex_destroy(&mylock)))
			FATAL("pthread_mutex_destroy() failed! [%d]\n", ret);
	}

	pthread_exit(NULL);
}

/* vi: ts=8 */
