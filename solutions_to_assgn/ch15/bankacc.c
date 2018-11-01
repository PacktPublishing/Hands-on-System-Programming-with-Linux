/*
 * solutions_to_assgn/ch15/bankacc.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 15 : Multithreading Part II - Synchronization
 ****************************************************************
 * Brief Description:
 * Write a simple 'banking app' as a multithreaded process (as described in the
 * early part of this chapter), just implementing (simplistic) deposit and
 * withdraw functions. Test it with and without using the mutex lock to protect
 * critical sections.
 *
 * PLEASE note that this code is trivial and naive; it is definitely NOT
 * intended to be a real banking app :-p
 * Refer Ch 15 for details, thank you.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../common.h"

#define NTHREADS	2

static int gLocking=1;
static long gBalance = 12000;
static const long gCB_MinCredit = 500, gCB_HiBalance = 1000000;
pthread_mutex_t mylock;

static void deposit(int thrdnum, long amt)
{
	if (amt <= 0)
		return;
	printf(" [thrd#%2d +] Deposit      : %12ld\n", thrdnum, amt);
	gBalance += amt;
	if (gBalance >= gCB_HiBalance)
		printf("*** Circuit Breaker *** High Balance\n");
	printf(" [thrd#%2d] Current Balance: %12ld\n", thrdnum, gBalance);
}
static void withdraw(int thrdnum, long amt)
{
	if ((gBalance - amt) < gCB_MinCredit) {
		printf("*** Circuit Breaker *** Min Credit hit! Failing withdawal...\n");
		return;
	}
	printf(" [thrd#%2d -] Withdrawal   : %12ld\n", thrdnum, amt);
	gBalance -= amt;
	printf(" [thrd#%2d] Current Balance: %12ld\n", thrdnum, gBalance);
}

#define LOCK(mtx) do {                                                   \
	int ret=0;                                                       \
	if (gLocking) {                                                  \
		if ((ret = pthread_mutex_lock(mtx)))                     \
			FATAL("pthread_mutex_lock failed! [%d]\n", ret); \
	}                                                                \
} while(0)

#define UNLOCK(mtx) do {                                                   \
	int ret=0;                                                         \
	if (gLocking) {                                                    \
		if ((ret = pthread_mutex_unlock(mtx)))                     \
			FATAL("pthread_mutex_unlock failed! [%d]\n", ret); \
	}                                                                  \
} while(0)


/*
 * Have it work this way:
 * Thread #1 :  deposit 3000 ; withdraw 250
 * Thread #2 : withdraw 9000 ; deposit 1000 
 */
void * worker(void *data)
{
	long threadnum = (long)data + 1;

	switch (threadnum) {
		case 1 : 
		 	 LOCK(&mylock);
			 deposit(threadnum, 3000);
			 withdraw(threadnum, 250);
			 UNLOCK(&mylock);
			 break;
		case 2 :
		         LOCK(&mylock);
			 withdraw(threadnum, 9000);
			 deposit(threadnum, 1000);
			 UNLOCK(&mylock);
			 break;
		default: WARN("threadnum = %ld ! [should never occur]!\n", threadnum);
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
		gLocking = 0;
	else if (atoi(argv[1]) == 1)
		gLocking = 1;
	else {
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	printf("Locking : %s\n", (gLocking == 1?"ON":"OFF"));
	if (gLocking) {
		if ((ret = pthread_mutex_init(&mylock, NULL)))
			FATAL("pthread_mutex_init() failed! [%d]\n", ret);
	}
	printf("Opening Balance : %6ld\n", gBalance);

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
	}

	if (gLocking) {
		if ((ret = pthread_mutex_destroy(&mylock)))
			FATAL("pthread_mutex_destroy() failed! [%d]\n", ret);
	}
	printf("Closing Balance : %6ld\n", gBalance);

	pthread_exit(NULL);
}
/* vi: ts=8 */
