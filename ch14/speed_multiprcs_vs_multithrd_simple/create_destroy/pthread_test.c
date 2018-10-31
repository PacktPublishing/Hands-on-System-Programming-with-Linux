/*
 * ch14/speed_multiprcs_vs_multithrd_simple/create_destroy/pthread_test.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A quick test: speed of fork/wait/exit vs 
 * pthread_create/pthread_join/pthread_exit.
 *
 * For details, please refer the book, Ch 14.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../../common.h"

#define NTHREADS 60000

void *do_nothing(void *data)
{
	unsigned long t = 0xb00da;
	pthread_exit(NULL);
}

int main(void)
{
	int ret, i;
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_create(&tid, &attr, do_nothing, NULL);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);

		/* Wait for the thread to terminate */
		ret = pthread_join(tid, NULL);
		if (ret)
			FATAL("pthread_join() failed! [%d]\n", ret);
	}

	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}

/* vi: ts=8 */
