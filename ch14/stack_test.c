/*
 * ch14/stack_test.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A simple demo (and POC) on thread stack size. We query the default thread
 * (and thread stack guard) sizes and print them; we then change the thread
 * stack size to the parameter passed by the user (in KB).
 * The second worker thread then calls function danger(), which true to it's
 * name, allocates a dangerous amount of memory as a local variable - in effect
 * onm the thread stack. If we set the stack size low enough, the stack indeed
 * overflows resulting in a segmentation fault ('segfault').
 *
 * For details, please refer the book, Ch 14.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <time.h>
#include "../common.h"

#define NTHREADS     	     3
#define TSTACK     (1024*1024)    /* just a megabyte */

static void dummy(long day)
{
	printf("%s(): parameter val = %ld\n", __func__, day);
}

static void danger(void)
{
#define NEL   500
	long heavylocal[NEL][NEL], alpha=0;
	int i, j;
	long int k=0;

	srandom(time(0));

	printf("\n *** In %s(): here, sizeof long is %ld\n", __func__, sizeof(long));
	/* Turns out to be 8 on an x86_64; so the 2d-array takes up
	 *  500 * 500 * 8 = 2,000,000 ~= 2 MB.
	 *  So thread stack space of less than 2 MB should result in a segfault.
	 *  (On a test box, any value < 1960 KB = 2,007,040 bytes,
	 *  resulted in segfault).
	 */

	/* The compiler is quite intelligent; it will optimize away the
	 * heavylocal 2d array unless we actually use it! So lets do some
	 * thing with it...
	 */
	for (i=0; i<NEL; i++) {
		k = random() % 1000;
		for (j=0; j<NEL-1; j++)
			heavylocal[i][j] = k;
		/*printf("hl[%d][%d]=%ld\n", i, j, (long)heavylocal[i][j]);*/
	}

	for (i=0; i<NEL; i++)
		for (j=0; j<NEL; j++)
			alpha += heavylocal[i][j];
	dummy(alpha);
}

void * worker(void *data)
{
	long datum = (long)data;

	printf(" worker #%ld:\n", datum);
	if (datum != 1)
		pthread_exit((void *)1);
		
	danger();

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
	long i;
	int ret, stat=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	size_t stacksz=0, stackguardsz=0;
	int stack_set=0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s size-of-thread-stack-in-KB\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	stack_set = atoi(argv[1]) * 1024;
	if (stack_set <= 0 || stack_set > INT_MAX) {
		fprintf(stderr, "%s: invalid stack size (%u) provided, defaulting to %d bytes\n",
				argv[0], stack_set, TSTACK);
		stack_set = TSTACK;
	}
	
	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Query thread stack size */
	ret = pthread_attr_getstacksize(&attr, &stacksz);
	if (ret)
		FATAL("pthread_attr_getstack() failed! [%d]\n", ret);
	printf("Default thread stack size       : %10zu bytes\n", stacksz);

	/* Set thread stack size */
	ret = pthread_attr_setstacksize(&attr, stack_set);
	if (ret)
		FATAL("pthread_attr_setstack(%u) failed! [%d]\n", TSTACK, ret);
	printf("Thread stack size now set to    : %10u bytes\n", stack_set);

	/* Query thread stack guard size */
	ret = pthread_attr_getguardsize(&attr, &stackguardsz);
	if (ret)
		FATAL("pthread_attr_getstackguard() failed! [%d]\n", ret);
	printf("Default thread stack guard size : %10zu bytes\n\n", stackguardsz);

	// Thread creation loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: creating thread #%ld ...\n", i);
		ret = pthread_create(&tid[i], &attr, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: joining (waiting) upon thread #%ld ...\n", i);
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		else
			printf("Thread #%ld successfully joined; it terminated with "
				"status=%d\n", i, stat);
	}

	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
}

/* vi: ts=8 */
