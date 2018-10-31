/*
 * ch16/cleanup_hdlr.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading with Pthreads, Part 3
 ****************************************************************
 * Brief Description:
 * (Based on the previous program: mt_iobuf_rfct.c).
 * We have refactored the earlier A_iobuf/iobuf.c program to become thread
 * safe (and saved it as ch16/mt_iobuf_rfct.c). But: what if, while
 * the worker threads are performing IO, they get cancelled?
 * This program provides a 'cleanup handler' such that, even if they do
 * get cancelled, we handle the cleanup.
 * Here, to demonstrate, if the user passes '1' as the second parameter,
 * we deliberately cancel the first worker thread.
 *
 * For details, please refer the book, Ch 16.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <pthread.h>
#include "../common.h"

#define NREAD	      512
#define NTHREADS	2

static int gVerbose = 1;

static void testit_mt_refactored(FILE * wrstrm, FILE * rdstrm, int numio,
				 int thrdnum, char *iobuf)
{
	int i, syscalls = NREAD * numio / getpagesize();
	size_t fnr = 0;

	if (syscalls <= 0)
		syscalls = 1;
	VPRINT
	    ("[Thread #%d]: numio=%d   total rdwr=%u   expected # rw syscalls=%d\n"
	     " iobuf = %p\n", thrdnum, numio, NREAD * numio, syscalls, iobuf);

	for (i = 0; i < numio; i++) {
		fnr = fread(iobuf, 1, NREAD, rdstrm);
		if (!fnr)
			FATAL("fread on /dev/urandom failed\n");

		if (!fwrite(iobuf, 1, fnr, wrstrm)) {
			if (feof(wrstrm))
				return;
			if (ferror(wrstrm)) {
				free(iobuf);
				FATAL("fwrite on our file failed\n");
			}
		}
	}
}

struct stToThread {
	FILE *wrstrm, *rdstrm;
	int thrdnum, numio;
	char *iobuf;
};
static struct stToThread *ToThread[NTHREADS];

static void cleanup_handler(void *arg)
{
	printf("+++ In %s +++\n" " free-ing buffer %p\n", __func__, arg);
	free(arg);
}

static void *wrapper_testit_mt_refactored(void *msg)
{
	struct stToThread *pstToThread = (struct stToThread *)msg;
	assert(pstToThread);

	/* Allocate the per-thread IO buffer here, thus avoiding the global
	 * heap buffer completely! */
	pstToThread->iobuf = malloc(NREAD);
	if (!pstToThread->iobuf)
		FATAL("thread #%d: malloc(%d) failed!\n",
		      pstToThread->thrdnum, NREAD);

	/* Install a 'cleanup handler' routine */
	pthread_cleanup_push(cleanup_handler, pstToThread->iobuf);

	testit_mt_refactored(pstToThread->wrstrm, pstToThread->rdstrm,
			     pstToThread->numio, pstToThread->thrdnum,
			     pstToThread->iobuf);

	/* *Must* invoke the 'push's counterpart: the cleanup 'pop' routine;
	 * passing 0 as parameter just registers it, it does not actually pop
	 * off and execute the handler. Why not? Because that's precisely what
	 * the next API, the pthread_exit(3) will implicitly do!
	 */
	pthread_cleanup_pop(0);
	free(pstToThread->iobuf);

	// Required for pop-ping the cleanup handler!
	pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
	char *fname = "/tmp/iobuf_tmp";
	FILE *wfp = NULL, *rfp = NULL;
	int numio;
	int i, ret; //, stat = 0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	void *stat=0;

	if (argc != 3) {
		fprintf(stderr,
			"Usage: %s number-of-times-to-rdwr cancel-thread\n"
			" (2nd param: pass 1 to cancel the first worker thread\n"
			"  0 otherwise).\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	numio = atoi(argv[1]);
	if (numio <= 0) {
		fprintf(stderr,
			"Usage: %s number-of-times-to-rdwr cancel-thread\n"
			" (2nd param: pass 1 to cancel the first worker thread\n"
			"  0 otherwise).\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf
	    ("%s: using default stdio IO RW buffers of size %u bytes; # IOs=%d\n",
	     argv[0], getpagesize(), numio);

	unlink(fname);
	wfp = fopen(fname, "a");
	if (!wfp)
		FATAL("fopen on %s failed\n", fname);

	rfp = fopen("/dev/urandom", "r");
	if (!rfp)
		FATAL("fopen on /dev/urandom failed\n");

	// Init the thread attribute structure to defaults
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Thread creation loop
	for (i = 0; i < NTHREADS; i++) {
		ToThread[i] = calloc(1, sizeof(struct stToThread));
		if (!ToThread[i])
			FATAL("calloc stToThread[%d] failed!\n", i);

		ToThread[i]->thrdnum = i;
		ToThread[i]->wrstrm = wfp;
		ToThread[i]->rdstrm = rfp;
		ToThread[i]->numio = numio;
		ret = pthread_create(&tid[i], &attr,
				     wrapper_testit_mt_refactored,
				     (void *)ToThread[i]);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	if (atoi(argv[2]) == 1) {
		// Lets send a cancel request to thread A (the first worker thread)
		printf("%s: sending CANCEL REQUEST to worker thread 0 ...\n",
		       __func__);
		ret = pthread_cancel(tid[0]);
		if (ret)
			FATAL("pthread_cancel(thread 0) failed! [%d]\n", ret);
	}

	// Thread join loop
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
		free(ToThread[i]);
	}

	fclose(rfp);
	fclose(wfp);

	pthread_exit(NULL);
}

/* vi: ts=8 */
