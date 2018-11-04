/*
 * Assignment Solution
 * solutions_to_assgn/ch16/mt_primes1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading Part 3
 ****************************************************************
 * 
 * Q1. mt_primes1:
Design and write a multithreaded application that uses multiple threads to
generate different prime-number sequences (for example, the first thread
generates primes from 1 to 5000, the second from 5001 to 10000, and so
on).

Solution: Pl follow the code (with detailed comments) below.
We deliberately place the generated prime #s into a buffer; this buffer
is guaranteed to be unique per thread, and thus thread-safe.
Another reason for the buffer: in this prgram, we could dump the buffer to
stdout to display the generated primes. In the next assignment, we simply
dump it to a file!
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include "../../common.h"

#define NTHREADS	3
#define MEM_SIZE   100000 
	/* IMP / TODO / TOFIX
	 * A MEM_SIZE of 100,000 "should" be enough memory to hold around
	 * 25,000 prime numbers, but who know!?
	 * Pl check...
	 */

/* From Ch 3; slightly enhanced to generate primes in a given range.
 * Additionally, it's thread-safe (MT-Safe) as it is CPU bound only and
 * only uses local variables. Also, the thord param 'buf' is pre-alloced
 * and unique for each calling thread, thus making it reentrant-safe as well.
 *
 * [The only thing to watch out for is the possible overflow of 'buf'!]
 */
static size_t simple_primegen(unsigned int start, unsigned int limit, char *buf)
{
	unsigned int i, j, isprime=0;
	size_t num_primes = 0;
#define LEN 128
	char tmpbuf[LEN];

	assert(start <= limit);
	assert(buf);

	for (i = start; i <= limit; i++) {
		isprime = 1;
		for (j = 2; j < limit / 2; j++) {
			if ((i != j) && (i % j == 0)) {
				isprime = 0;
				break;
			}
		}
		if (isprime) {  // loop index 'i' is a prime!
			num_primes ++;
			// Place it in the result buffer
			snprintf(tmpbuf, LEN, "%6d, ", i);
			strncat(buf, tmpbuf, LEN);
		}
	}
	return num_primes;
}

struct stToThread {
	int thrdnum;
	unsigned int start_num, end_num;
	char *iobuf;
};
static struct stToThread *ToThread[NTHREADS];

static void disp_the_primes(struct stToThread * pstToThrd)
{
	assert(pstToThrd->iobuf);
	printf("\n===\n%s\n===\n", pstToThrd->iobuf);
}

static void *worker(void *msg)
{
	struct stToThread *pstToThread = (struct stToThread *)msg;
	assert(pstToThread);

	printf(" Worker #%2d : Generate primes from %9d to %9d : "
		"%6zu primes generated\n",
			pstToThread->thrdnum, pstToThread->start_num,
			pstToThread->end_num,
			simple_primegen(pstToThread->start_num, pstToThread->end_num,
				pstToThread->iobuf));
	pthread_exit((void *)0);
}


int main(int argc, char **argv)
{
	int i, ret;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	void *stat=0;

	unsigned int prime_range[NTHREADS][2] = {
		{1, 5000},
		{5001, 10000},
		{20000, 20101}
	};

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Thread creation loop */
	for (i = 0; i < NTHREADS; i++) {
		ToThread[i] = calloc(1, sizeof(struct stToThread));
		if (!ToThread[i])
			FATAL("calloc stToThread[%d] failed!\n", i);

		ToThread[i]->thrdnum = i;
		ToThread[i]->start_num = prime_range[i][0];
		ToThread[i]->end_num = prime_range[i][1];
		ToThread[i]->iobuf = calloc(MEM_SIZE, 1);
		if (!ToThread[i]->iobuf)
			FATAL("calloc MEM_SIZE failed!\n");
		ret = pthread_create(&tid[i], &attr,
				     worker, (void *)ToThread[i]);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	/* Thread join loop */
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		else {
			printf(" Thread #%d successfully joined; it "
			       "terminated with status=%ld\n", i, (long)stat);
			if (stat == PTHREAD_CANCELED)
				printf("  : was CANCELED\n");
		}
#if 0
		disp_the_primes(ToThread[i]);
#endif
		free(ToThread[i]->iobuf);
		free(ToThread[i]);
	}
	/* all worker threads have terminated, have main() call it a day! */
	pthread_exit(NULL);
}

/* vi: ts=8 */
