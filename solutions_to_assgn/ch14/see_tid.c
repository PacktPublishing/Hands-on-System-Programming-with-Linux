/*
 * assessments/ch15/see_tid.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:
 *  Ch 15 : Multithreading Part I - The Essentials
 ****************************************************************
 * The question:
 * [Advanced] see_tid.c : To obtain the PID (Process ID) of the calling
 * process, one issues the getpid(2) system call.
 * a) What about obtaining the OS-level ID, i.e., the Thread ID (TID) of the
 * calling thread? (Hint: (i) Don't confuse this TID with the library-layer
 * threadid - the seconds value-result parameter to pthread_create; they're
 * different. (ii) There is a Linux-specific way to obtain it)
 * b) Having found this API, use it in an application to print both the PID
 * and TID [Warning! this code will be very Linux specific and non-portable!].
 *
 * Ans.
 * a) to get the TID, use the system call gettid(2) ; Linux-specific.
 * b) BUT, there is no glibc wrapper for this syscall! So how does one issue
 * it? Via the "indirect system call" syscall(2).
 * See the code below; a re-implementation of the cr8_so_many_threads.c program.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>	/* Required for syscall(2) */
#include "../../common.h"

void *worker(void *data)
{
	long datum = (long)data;
	printf("Worker thread #%5ld [PID %d * TID %ld *]: pausing now...\n",
	       datum, getpid(), syscall(SYS_gettid));
	(void)pause();
	printf(" #%ld: work done, exiting now\n", datum);
	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	long i;
	int ret;
	pthread_t tid;
	long numthrds = 0;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s number-of-threads-to-create\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}
	numthrds = atol(argv[1]);
	if (numthrds <= 0) {
		fprintf(stderr, "Usage: %s number-of-threads-to-create\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < numthrds; i++) {
		ret = pthread_create(&tid, NULL, worker, (void *)i);
		if (ret)
			FATAL
			    ("pthread_create() PID %d failed @ thread #%d! [ret=%d]\n",
			     getpid(), i, ret);
	}
	pthread_exit(NULL);
}

/* vi: ts=8 */
