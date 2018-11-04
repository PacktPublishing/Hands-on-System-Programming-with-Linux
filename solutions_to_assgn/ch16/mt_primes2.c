/*
 * Assignment Solution
 * solutions_to_assgn/ch16/mt_primes2.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading Part 3
 ****************************************************************
 * 
Q2. mt_primes2:
Further enhance the above app to:
a) Merge its output into a file 
b) Provide the ability to have the end user cancel a thread's work.
Provide cleanup handler(s) as required.

Q1 repeated below as well:
Q1. Design and write a multithreaded application that uses multiple threads to
generate different prime-number sequences (for example, the first thread
generates primes from 1 to 5000, the second from 5001 to 10000, and so
on).

Solution: 
Pl follow the code (with detailed comments) below.
We deliberately place the generated prime #s into a buffer; this buffer
is guaranteed to be unique per thread, and thus thread-safe.
Another reason for the buffer: in the previous program, we dumped the buffer to
stdout to display the generated primes. Now, we simply dump it to a file.
To do so, we introduce an 'fd' member into the per-thread structure.

To show an example of thread cancellation in action, we set up an artifact: if
user passes a parameter '1' to this app, we shall deliberately have the main()
thread cancel (only) the third worker thread (thread #2). To ensure it happens,
we have the third worker thread sleep for half a sec before starting actual
work. Also, to figure whether and which thread was canceled, we keep a global;
thus, we protect access to it with a mutex lock. It's actually pedantic here,
as we only ever access the global once in a critical context; nevertheless, we
keep the code to be 'correct'.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../common.h"

#define PRM_DATAFILE	"primes.dat"
#define NTHREADS	3
#define MEM_SIZE   100000 
	/* IMP / TODO / TOFIX
	 * A MEM_SIZE of 100,000 "should" be enough memory to hold around
	 * 25,000 prime numbers, but who know!?
	 * Pl check...
	 */

static pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;
static int gCanceledThread = 0;

/* From Ch 3; slightly enhanced to generate primes in a given range.
 * Additionally, it's thread-safe (MT-Safe) as it is CPU bound only and
 * only uses local variables. Also, the third param 'buf' - where we place the
 * primes in the range [start, limit] - is pre-allocated and unique for each
 * calling thread, thus making it reentrant-safe as well.
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
	int fd;
};
static struct stToThread *ToThread[NTHREADS];

static void write_the_primes(struct stToThread **pstToThrd)
{
	int i, len;
	
	for (i = 0; i < NTHREADS; i++) {
		if (i == gCanceledThread)
			continue;
		assert(pstToThrd[i]->iobuf);
		/*printf("\n===\n%s\n===\n", pstToThrd[i]->iobuf); */

		len = strlen(pstToThrd[i]->iobuf);
		if (write(pstToThrd[i]->fd, pstToThrd[i]->iobuf, len) < 0)
			WARN("thread #%d; write(2) of iobuf to the data file failed.\n",
				pstToThrd[i]->thrdnum);
	}
}

static void cleanup_handler(void *arg)
{
	struct stToThread *pstToThread = (struct stToThread *)arg;

	/* Taking the mutex here is pedantic; nevertheless ... */
	LOCK_MTX(&mylock);
	gCanceledThread = pstToThread->thrdnum;
	UNLOCK_MTX(&mylock);
	printf("+++ In cleanup func %s (thread #%d canceled!) +++\n",
			__func__, pstToThread->thrdnum);
	free(pstToThread->iobuf);
	free(pstToThread);
}

static void *worker(void *msg)
{
	int len;
	size_t numprimes=0;
	char tmp[256];
	struct stToThread *pstToThread = (struct stToThread *)msg;
	assert(pstToThread);

	/* Install a 'cleanup handler' routine */
	pthread_cleanup_push(cleanup_handler, pstToThread);

	if (pstToThread->thrdnum == 2)
		r_sleep(0, 5000000);
	numprimes = simple_primegen(pstToThread->start_num, pstToThread->end_num,
			pstToThread->iobuf);
	snprintf(tmp, 255, " Worker #%2d : Generate primes from %9d to %9d : "
		"%6zu primes generated\n",
		pstToThread->thrdnum, pstToThread->start_num,
		pstToThread->end_num, numprimes);
	printf("%s", tmp);

	/* The primes have been filled into the buffer pstToThread->iobuf */
	len = strlen(pstToThread->iobuf);
	printf("len = %d\n", len);

	/* *Must* invoke the 'push's counterpart: the cleanup 'pop' routine;
	 * passing 0 as parameter just registers it, it does not actually pop
	 * off and execute the handler.
	 */
	pthread_cleanup_pop(0);
	pthread_exit((void *)0);
}


int main(int argc, char **argv)
{
	int i, ret, fd;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	void *stat=0;

	unsigned int prime_range[NTHREADS][2] = {
		{1, 5000},
		{5001, 10000},
		{20000, 20101}
	};

	/* Open the data file */
	if ((fd = open(PRM_DATAFILE, O_CREAT|O_WRONLY, 0644)) < 0)
		FATAL("Opening data file \"%s\" failed\n", PRM_DATAFILE);

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
		ToThread[i]->fd = fd;

		ret = pthread_create(&tid[i], &attr,
				     worker, (void *)ToThread[i]);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	if (argc >= 2 && atoi(argv[1]) == 1) {
		// Lets send a cancel request to thread #2 (the third worker thread)
		printf("%s: sending CANCEL REQUEST to worker thread 2 now ...\n",
		       __func__);
		ret = pthread_cancel(tid[2]);
		if (ret)
			FATAL("pthread_cancel(thread 2) failed! [%d]\n", ret);
	}

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
	}

	write_the_primes(ToThread);

	/* Cleanup */
	for (i = 0; i < NTHREADS; i++) {
		/* main() runs alone now, no need for the mutex */
		if (i == gCanceledThread)
			continue;
		free(ToThread[i]->iobuf);
		free(ToThread[i]);
	}
	close (fd);
	pthread_exit(NULL);
}

/* vi: ts=8 */
