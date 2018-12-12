/*
 * ch15/cv_simple.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 15 : Multithreading Part II - Synchronization
 ****************************************************************
 * Brief Description:
 * A small program to demonstrate the usage of a condition variable and it's
 * associated mutex lock. Here, we create two threads - A and B. We then have
 * thread B perform some 'work' and thread A synchronize upon completion of
 * that work - by using a CV !
 *
 * For details, please refer the book, Ch 15.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../common.h"

#define NTHREADS	2

static int gWorkDone=0;
/* The {cv,mutex} pair */
static pthread_cond_t mycv;
static pthread_mutex_t mycv_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Thread B: we perform the 'work', and then 'signal' thread A via the
 * {CV, mutex} pair that we are done.
 */
static void * workerB(void *msg)
{
	int ret=0;

	printf(" [thread B] : perform the 'work' now (first sleep(1) :-)) ...\n");
	sleep(1);
	DELAY_LOOP('b', 72);

	printf("\n [thread B] : work done, signal thread A to continue ...\n");
	/* It's not strictly required to lock/unlock the associated mutex
	 * while signalling; we do it here to be pedantically correct (and
	 * to shut helgrind up).
	 */
	LOCK_MTX(&mycv_mutex);
	gWorkDone = 1;
	ret = pthread_cond_signal(&mycv);
	if (ret)
		FATAL("pthread_cond_signal() in thread B failed! [%d]\n", ret);
	UNLOCK_MTX(&mycv_mutex);
	pthread_exit((void *)0);
}

/* Thread A must wait until thread B completes the 'work';
 * we use a {CV, mutex} pair to easily and efficiently achieve
 * this.
 */
static void * workerA(void *msg)
{
	int ret=0;

	LOCK_MTX(&mycv_mutex);
	while (1) {
		printf(" [thread A] : now waiting on the CV for thread B to finish...\n");
		ret = pthread_cond_wait(&mycv, &mycv_mutex);
		// Blocking: associated mutex auto-released ...
		if (ret)
			FATAL("pthread_cond_wait() in thread A failed! [%d]\n", ret);
		// Unblocked: associated mutex auto-acquired upon release from the condition wait...
		
		printf(" [thread A] : recheck the predicate (is the work really "
			"done or is it a spurious wakeup?)\n");
		if (gWorkDone)
			break;
		printf(" [thread A] : SPURIOUS WAKEUP detected !!! "
			"(going back to CV waiting)\n");
	}
	UNLOCK_MTX(&mycv_mutex);
	printf(" [thread A] : (cv wait done) thread B has completed it's work...\n");
	pthread_exit((void *)0);
}

int main(void)
{
	long i;
	int ret, stat=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	pthread_condattr_t cvattr;

	// Init the thread attribute structure to defaults
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Init a condition variable attribute object
	if ((ret = pthread_condattr_init(&cvattr)))
		FATAL("pthread_condattr_init failed [%d].\n", ret);
	// Init a {cv,mutex} pair: condition variable & it's associated mutex
	if ((ret = pthread_cond_init(&mycv, &cvattr)))
		FATAL("pthread_cond_init failed [%d].\n", ret);
	//  the mutex lock has been statically initialized above.

	// Create the worker - A and B - threads
	ret = pthread_create(&tid[0], &attr, workerA, (void *)0);
	if (ret)
		FATAL("pthread_create() thread A failed! [%d]\n", ret);
	ret = pthread_create(&tid[1], &attr, workerB, (void *)0);
	if (ret)
		FATAL("pthread_create() thread B failed! [%d]\n", ret);
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
	}

	// Destroy the cv attr object and the {mutex,cv} pair..
	pthread_condattr_destroy(&cvattr);
	pthread_mutex_destroy(&mycv_mutex);
	pthread_cond_destroy(&mycv);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
