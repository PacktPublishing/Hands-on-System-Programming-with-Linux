/*
 * solutions_to_assgn/ch14/pthreads_joiner3.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Assessment:
 * Passing Multiple Parameters: Modify the pthreads_joiner2.c application
 * (call it pthreads_joiner3.c) to have the worker threads sleep for a
 * specific number of seconds; pass both this number and the 'thread number'
 * (the loop index) as a parameter to the app.
 * (Tips: 
 * (i) use a structure to pass multiple parameters to the worker thread
 * (ii) careful with thread-safety on passing the thread parameter).
 *
 * [Refer Ch 14 for details, thank you].
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../common.h"

#define NTHREADS	3

struct passit {
	int thrdnum;
	int sleeptm;
};

void * worker(void *data)
{
	struct passit *arg = (struct passit *)data; // retrieve the parameter
	int slptm = arg->sleeptm - arg->thrdnum;

	printf("*********** arg->thrdnum=%d arg->sleeptm=%d\n", arg->thrdnum, arg->sleeptm);
	printf(" worker #%d: will sleep for %ds now ...\n", arg->thrdnum, slptm);
	sleep(slptm);
	printf(" worker #%d: work (eyeroll) done, exiting now\n", arg->thrdnum);

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
	int i, j, ret, stat=0, sleeptm=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	struct passit *passthis[NTHREADS]; /* must pass thread-safe and therefore
					      individual buffers to each worker
					      thread! */

	if (argc != 2) {
		fprintf(stderr, "Usage: %s num-seconds-to-have-workers-sleep\n"
				"                               [Minimum should be %d]\n",
				argv[0], NTHREADS);
		exit(EXIT_FAILURE);
	}

	sleeptm = atoi(argv[1]);
	if (sleeptm <= NTHREADS-1) {
		fprintf(stderr, "Usage: %s num-seconds-to-have-workers-sleep\n"
				"                               [Minimum should be %d]\n",
				argv[0], NTHREADS);
		exit(EXIT_FAILURE);
	}

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Thread creation loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: creating thread #%d ...\n", i);

		passthis[i] = calloc(1, sizeof(struct passit));
		if (!passthis[i])
			FATAL("calloc %d failed!\n", i);
		passthis[i]->thrdnum = i;
		passthis[i]->sleeptm = sleeptm;

		ret = pthread_create(&tid[i], &attr, worker, (void *)passthis[i]);
		if (ret) {
			free((void *)passthis[i]);
			FATAL("pthread_create() failed! [%d]\n", ret);
		}
	}
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (j = 0; j < NTHREADS; j++) {
		printf("main: joining (waiting) upon thread #%d ...\n", j);
		ret = pthread_join(tid[j], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		printf("Thread #%d successfully joined; it terminated with "
			"status=%d\n", j, stat);
		free((void *)passthis[j]);
	}

	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
}

/* vi: ts=8 */
