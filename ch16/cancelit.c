/*
 * ch16/cancelit.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading with Pthreads, Part III
 ****************************************************************
 * Brief Description:
 * This program demonstrates cancelling a thread; it is built upon our
 * earlier ch14/pthreads_joiner1.c code.
 *
 * For details, please refer the book, Ch 16.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../common.h"

#define NTHREADS	2

void *worker(void *data)
{
	long datum = (long)data;
	int slptm = 8, ret = 0;

	if (datum == 0) {	/* "Thread A"; lets keep it in a 'critical' state,
				   non-cancellable, for a short while, then enable
				   cancellation upon it. */
		printf(" worker #%ld: disabling Cancellation:"
			" will 'work' now...\n", datum);
		if ((ret =
		     pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL)))
			FATAL("pthread_setcancelstate failed 0 [%d]\n", ret);
		DELAY_LOOP(datum + 48, 100);	// the 'work'
		printf("\n worker #%ld: enabling Cancellation\n", datum);
		if ((ret = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)))
			FATAL("pthread_setcancelstate failed 1 [%d]\n", ret);
	}

	printf(" worker #%ld: will sleep for %ds now ...\n", datum, slptm);
	sleep(slptm);		// sleep() is a 'cancellation point'
	printf(" worker #%ld: work (eyeroll) done, exiting now\n", datum);

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

int main(void)
{
	long i, stat = 0;
	int ret;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Thread creation loop */
	for (i = 0; i < NTHREADS; i++) {
		printf("main: creating thread #%ld ...\n", i);
		ret = pthread_create(&tid[i], &attr, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	/* Lets send a cancel request to thread A */
	ret = pthread_cancel(tid[0]);
	if (ret)
		FATAL("pthread_cancel(thread 0) failed! [%d]\n", ret);

	/* Thread join loop */
	for (i = 0; i < NTHREADS; i++) {
		printf("main: joining (waiting) upon thread #%ld ...\n", i);
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		else {
			printf
			    ("Thread #%ld successfully joined; it terminated with "
			     "status=%ld\n", i, stat);
			if ((void *)stat == PTHREAD_CANCELED)
				printf("   *** Was CANCELLED ***\n");
		}
	}

	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
}

/* vi: ts=8 */
