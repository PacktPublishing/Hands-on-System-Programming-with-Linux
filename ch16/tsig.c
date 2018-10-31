/*
 * ch16/tsig.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading with Pthreads, Part III
 ****************************************************************
 * Brief Description:
 * A quick demo of signal handling in multithreaded (MT) apps.
 * Strategy:
 *  - mask (block) all signals in the main thread
 *  - now any thread created by main inherit it's signal mask, which 
 *    means that all signals will be blocked out in all subsequently 
 *    created threads;
 *  - create a separate dedicated signal handling thread whose only job is to
 *    handle signals for the MT app; it does so via the blocking sigwait(3).
 *
 * For details, please refer the book, Ch 16.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "../common.h"

#define NTHREADS	2

/*
 * signal_handler() : our MT app's dedicated 'signal hanler thread'. We
 * handle all signals here.
 */
static void *signal_handler(void *arg)
{
	sigset_t sigset;
	int sig;

	printf("Dedicated signal_handler() thread alive..\n");
	while (1) {
		/* Wait for any/all signals */
		if (sigfillset(&sigset) == -1)
			FATAL("sigfillset failed");
		if (sigwait(&sigset, &sig) < 0)
			FATAL("sigwait failed");

		/* Note on sigwait():
		 *    sigwait suspends the calling thread until one of 
		 *    (any of) the  signals  in set is delivered to the 
		 *    calling thread. It then stores the number of the 
		 *    signal received in the location  pointed  to  by  
		 *    "sig" and returns.  The  signals  in set must be 
		 *    blocked and not ignored on entrance to sigwait. 
		 *    If the delivered signal has a  signal handler 
		 *    function attached, that function is *not* called.
		 */

		/* We've caught a signal! 
		 * Here, as a demo, we just emit a simple printf. 
		 * In a 'real' app, handle the signal here itself.
		 */
		switch (sig) {
		case SIGINT:
			// Perform signal handling for SIGINT here
			printf("+++ signal_handler(): caught signal #%d +++\n",
			       sig);
			break;
		case SIGQUIT:
			// Perform signal handling for SIGQUIT here
			printf("+++ signal_handler(): caught signal #%d +++\n",
			       sig);
			break;
		case SIGIO:
			// Perform signal handling for SIGIO here
			printf("+++ signal_handler(): caught signal #%d +++\n",
			       sig);
			break;
		default:
			// Signal <whichever> caught
			printf
			    ("*** signal_handler(): caught signal #%2d [unhandled] ***\n",
			     sig);
			break;
		}
	}
	return (void *)0;
}

static void *work(void *id)
{
	long this = (long)id;
	printf(" [Thread #%ld] PID %d running ...\n", this, getpid());
	if (this == 1) {
		DELAY_LOOP('1', 200);
	} else if (this == 2) {
		DELAY_LOOP('2', 200);
	}
	printf("\n---------------------------------------\n");
	pthread_exit((void *)0);
}

int main(void)
{
	sigset_t sigset;
	pthread_t pthrd[NTHREADS + 1];
	pthread_attr_t attr;
	long t = 0, stat;
	int ret = 0;

	/* 
	 * Block *all* signals here in the main thread.
	 * Now all subsequently created threads also block all signals.
	 */
	sigfillset(&sigset);
	if (pthread_sigmask(SIG_BLOCK, &sigset, NULL))
		FATAL("main: pthread_sigmask failed");

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	/*--- Create the dedicated signal handling thread ---*/
	ret = pthread_create(&pthrd[t], &attr, signal_handler, NULL);
	if (ret)
		FATAL("pthread_create %ld failed [%d]\n", t, ret);

	/* Create worker threads */
	for (t = 1; t < NTHREADS + 1; t++) {
		ret = pthread_create(&pthrd[t], &attr, work, (void *)t);
		if (ret)
			FATAL("pthread_create %ld failed [%d]\n", t, ret);
	}

	/* Block on signals forever; until we catch a fatal one! */
	while (1)
		(void)pause();

	/* In this demo, this code below is never reached; it's nevertheless
	 * there for completeness. */
	// Thread join loop
	for (t = 0; t < NTHREADS; t++) {
		printf("main: joining (waiting) upon thread #%ld ...\n", t);
		ret = pthread_join(pthrd[t], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		else {
			printf
			    ("Thread #%ld successfully joined; it terminated with "
			     "status=%ld\n", t, stat);
			if ((void *)stat == PTHREAD_CANCELED)
				printf("   *** Was CANCELLED ***\n");
		}
	}

	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
}

/* vi: ts=8 */
