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
 * Q6. Make a copy of the ch14/struct_as_param.c program; deliberately insert
 * a defect into the application by using exactly one allocated structure (not
 * three) and passing it to each of the worker threads (this time, it should be
 * "racy" and will (eventually) fail).
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
	int i, j;
	int ret, stat=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	volatile struct passit *passthis;
	int sleeptm=0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s num-seconds-to-wait-for-workers-max\n"
				"                           [Minimum should be %d]\n",
				argv[0], NTHREADS);
		exit(EXIT_FAILURE);
	}

	sleeptm = atoi(argv[1]);
	if (sleeptm <= NTHREADS-1) {
		fprintf(stderr, "Usage: %s num-seconds-to-wait-for-workers-max\n"
				"                           [Minimum should be %d]\n",
				argv[0], NTHREADS);
		exit(EXIT_FAILURE);
	}
	passthis = calloc(1, sizeof(struct passit));
	if (!passthis)
		FATAL("calloc failed!\n");

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Thread creation loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: creating thread #%d ...\n", i);

		/*
		 * BUG! the passthis pointer is the SAME MEMORY BUFFER for all
		 * the threads; hence, it gets modified on the fly and is Not thread-safe!
		 */
		passthis->thrdnum = i;
		passthis->sleeptm = sleeptm;
		printf("*********** passthis->thrdnum=%d passthis->sleeptm=%d\n",
				passthis->thrdnum, passthis->sleeptm);
		ret = pthread_create(&tid[i], &attr, worker, (void *)passthis);
		if (ret) {
			free((void *)passthis);
			FATAL("pthread_create() failed! [%d]\n", ret);
		}
	}
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (j = 0; j < NTHREADS; j++) {
		//printf("main: joining (waiting) upon thread #%d ...\n", j);
		ret = pthread_join(tid[j], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		//printf("Thread #%d successfully joined; it terminated with "
		//	"status=%d\n", j, stat);
	}

	free((void *)passthis);
	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
}

/* vi: ts=8 */
