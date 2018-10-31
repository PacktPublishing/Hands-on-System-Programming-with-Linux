/*
 * ch16/mt_iobuf_tls.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading with Pthreads, Part 3
 ****************************************************************
 * Brief Description:
 * Here, we have ported the earlier <book-github-repo>/A_iobuf/iobuf.c program
 * to be multithreaded. Once this is done, multiple threads can run through
 * the code of the 'testit' function, which is *not* thread safe.
 * So, we have reworked the function (now renamed to 'testit_mt_tls') to use
 * a TLS (Thread-Local Storage) variable, thus rendering it thread safe.
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
static __thread char iobuf[NREAD];    // our TLS variable

static void testit_mt_tls(FILE * wrstrm, FILE * rdstrm, int numio, int thrdnum)
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
			if (ferror(wrstrm))
				FATAL("fwrite on our file failed\n");
		}
	}
}

struct stToThread {
	FILE *wrstrm, *rdstrm;
	int thrdnum, numio;
};
static struct stToThread *ToThread[NTHREADS];

static void *wrapper_testit_mt_tls(void *msg)
{
	struct stToThread *pstToThread = (struct stToThread *)msg;
	assert(pstToThread);
	testit_mt_tls(pstToThread->wrstrm, pstToThread->rdstrm,
		      pstToThread->numio, pstToThread->thrdnum);
	pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
	char *fname = "/tmp/iobuf_tmp";
	FILE *wfp = NULL, *rfp = NULL;
	int numio;
	int i, ret, stat = 0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s number-of-times-to-rdwr\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	numio = atoi(argv[1]);
	if (numio <= 0) {
		fprintf(stderr, "Usage: %s number-of-times-to-rdwr\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	printf
	    ("%s: using default stdio IO RW buffers of size %u bytes; # IOs=%d\n",
	     argv[0], getpagesize(), numio);

#if 0
	gbuf = malloc(NREAD);
	if (!gbuf)
		FATAL("malloc %zu failed!\n", NREAD);
#endif
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
				     wrapper_testit_mt_tls,
				     (void *)ToThread[i]);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		else
			printf(" Thread #%d successfully joined; it "
			       "terminated with status=%d\n", i, stat);
		free(ToThread[i]);
	}

	fclose(rfp);
	fclose(wfp);
	//free(gbuf);

	pthread_exit(NULL);
}

/* vi: ts=8 */
