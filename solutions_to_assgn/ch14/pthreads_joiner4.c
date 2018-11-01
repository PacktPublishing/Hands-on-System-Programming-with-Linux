/*
 * solutions_to_assgn/ch14/pthreads_joiner4.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Q5. Join with Timeout: Modify the pthreads_joiner3.c application (call it
 * pthreads_joiner4.c) to block upon the worker threads for only a specific
 * maximum number of seconds; pass both this number and the 'thread number'
 * (the loop index) as a parameter to the app. 
 *
 * Eg. run this as:
 *  ./pthreads_joiner4 10 3
 *
 * [Refer Ch 14 for details, thank you].
 */
#include "../../common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>

#define NTHREADS	3

struct passit {
	int thrdnum;
	int sleeptm;
};

void * worker(void *data)
{
	struct passit *arg = (struct passit *)data; // retrieve the parameter
	int slptm = arg->sleeptm - arg->thrdnum;

	printf(" worker #%d: will sleep for %ds now ...\n", arg->thrdnum, slptm);
	sleep(slptm);
	printf(" worker #%d: work (eyeroll) done, exiting now\n", arg->thrdnum);

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
	int i, j, ret, stat=0, sleeptm=0, join_tmout=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	struct passit *passthis[NTHREADS]; /* must pass thread-safe and therefore
					      individual buffers to each worker
					      thread! */

	if (argc != 3) {
		fprintf(stderr, "Usage: %s num-seconds-to-have-workers-sleep num-seconds-to-join-for-max\n"
				"                           [Minimum should be %d]\n",
				argv[0], NTHREADS);
		exit(EXIT_FAILURE);
	}

	sleeptm = atoi(argv[1]);
	join_tmout = atoi(argv[2]);
	if ((sleeptm <= NTHREADS-1) || 
	    (join_tmout <= NTHREADS-1)) {
		fprintf(stderr, "Usage: %s num-seconds-to-have-workers-sleep num-seconds-to-join-for-max\n"
				"                           [Minimum should be %d]\n",
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
		struct timespec tmspec;

		printf("=== main: joining (waiting) upon thread #%d [for max %ds] ...\n",
				j, join_tmout);

		// get current timestamp
		if (clock_gettime(CLOCK_REALTIME, &tmspec) < 0)
			FATAL("clock_gettime(2) %d failed\n", j);
		tmspec.tv_sec += join_tmout;
		ret = pthread_timedjoin_np(tid[j], (void **)&stat, &tmspec);
		if (ret) {
			WARN("*** pthread_timedjoin_np() failed [%d] ***\n", ret);
			if (ret == ETIMEDOUT)
				fprintf(stderr, " reason: timeout\n");
		}
		else {
			printf("Thread #%d successfully joined; it terminated with "
				"status=%d\n", j, stat);
			/* Subtle: we _only_ free the param buffer if the join
			 * is successful. This implies a (insignificant) leak
			 * if the join times out!
			 */
			free((void *)passthis[j]);
		}
	}

	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
	/* Whoa! the passthis pointers will get lost here and some worker
	 * threads might *still* be alive and referring them! - behavior
	 * becomes undefined. 
	 * (Try running the debug ver via valgrind with the '--leak-check=full
	 * --show-leak-kinds=all' options; it shows "possibly lost" and "still
	 *  reachable" blocks..).
	 * One more reason why a blocking join is the way to go.
	 */
}

/* vi: ts=8 */
