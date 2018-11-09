/*
 * ch16/pthreads_app_template.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading Part 3
 ****************************************************************
 * 
 * A quick and very much *incomplete* generic template for writing Pthreads
 * apps!
 * _Please Note_ that this is Not meant to be a full working 'model' Pthreads
 *  app; nevertheless, it might serve the reader as a useful starting point to
 * build upon; please refer to and use it in that spirit.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <pthread.h>
#include "../common.h"

#define NTHREADS	3

struct stToThread {
	int thrdnum;
	int start_num, end_num;
	char *iobuf;
};
static struct stToThread *ToThread[NTHREADS];


static void cleanup_handler(void *arg)
{
	struct stToThread *pstToThread = (struct stToThread *)arg;
	assert(pstToThread);

	printf("+++ In %s():thrd #%d +++\n", __func__, pstToThread->thrdnum);
	//free(arg);
}

static void *worker(void *msg)
{
	struct stToThread *pstToThread = (struct stToThread *)msg;
	assert(pstToThread);

	/* Install a 'cleanup handler' routine */
	pthread_cleanup_push(cleanup_handler, pstToThread);


	/* ... do the work ... */


	/* *Must* invoke the 'push's counterpart: the cleanup 'pop' routine;
	 * passing 0 as parameter just registers it, it does not actually pop
	 * off and execute the handler.
	 */
	pthread_cleanup_pop(0);
	pthread_exit((void *)0);
}


int main(int argc, char **argv)
{
	int i, ret;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	void *stat=0;

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Thread creation loop */
	for (i = 0; i < NTHREADS; i++) {
		ToThread[i] = calloc(1, sizeof(struct stToThread));
		if (!ToThread[i])
			FATAL("calloc stToThread[%d] failed!\n", i);

		ToThread[i]->thrdnum = i;
		ToThread[i]->start_num = 1;
		ToThread[i]->end_num = 50;
		ret = pthread_create(&tid[i], &attr,
				     worker, (void *)ToThread[i]);
		if (ret)
			FATAL("[%d] pthread_create() failed! [%d]\n", i, ret);
		printf("Thread #%d successfully created\n", i);
	}
	pthread_attr_destroy(&attr);

#if 0
	/* Cancel a thread? */
	if (argc >= 2 && atoi(argv[1]) == 1) {
		// Lets send a cancel request to thread A (the first worker thread)
		printf("%s: sending CANCEL REQUEST to worker thread 0 ...\n",
		       __func__);
		ret = pthread_cancel(tid[0]);
		if (ret)
			FATAL("pthread_cancel(thread 0) failed! [%d]\n", ret);
	}
#endif

	/* Thread join loop */
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("[%d] pthread_join() failed! [%d]\n", i, ret);
		else {
			printf(" Thread #%d successfully joined; it "
			       "terminated with status=%ld\n", i, (long)stat);
			if (stat == PTHREAD_CANCELED)
				printf("  : was CANCELED\n");
		}
		free(ToThread[i]);
	}
	/* all worker threads have terminated, have main() call it a day! */
	pthread_exit(NULL);
}

/* vi: ts=8 */
