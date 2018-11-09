/*
 * Assignment Solution
 * solutions_to_assgn/ch18/mt_copy.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 18 : Advanced File IO
 ****************************************************************

Q1. mt_copy.c
In Ch A 'File I/O Essentials' section 'Correctly using the read/write APIs', we
demonstrated a program to correctly copy a source to destination file (a very
simple 'cp' program, in effect). 
Enhance this program creating a fully multithreaded parallelized version of the
same - it hosuld be able to copy a given source file to a given destination file.
Use this strategy: divide up the work among N threads (the user can pass N as a
parameter); *each thread* performs 1/Nth of the IO, reading from src file from a
start offset for 1/N bytes and writing to the destination file (always just
overwritten) at the same offset for the same length (take care of writing any
remaining bytes as well). (Hint: use the pread(2), pwrite(2) APIs instead of
the traditional read(2), write(2) system calls).

Solution:
Quite an interesting program; it serves as a (simple) example of how one can
exploit threads to perform IO in parellel to a given file(s). We use the
pread(2), pwrite(2) syscalls to acheive this. 
Our basic strategy: 
the user passes the number of threads to use for IO (in a production app, we'd
decide). We spawn off thesethreads and have each of them (in their resp
'worker()' routine), perform a *part* of the IO to the file - we calculate, for
the given # of threads and the size of the file, how much read/write (IO) each
thread should do (basically, filesize/numthreads), and the starting offset for
each thread. This way, all N threads run in parallel, performing a part of the
work - an excellent example of a MT app. (In fact, enterprise-scale file IO with
products such as Netflix use similar - much more sophisticated of course - ideas).

Here, our program is not very optimized; we could do better in terms of
performance. But we just leave it as such; the only idea is to get key
concepts across.

A sample run:

$ dd if=/dev/urandom of=src bs=10231 count=1000
1000+0 records in
1000+0 records out
10231000 bytes (10 MB, 9.8 MiB) copied, 0.104296 s, 98.1 MB/s
$ ls -l src
-rw-r--r-- 1 kai kai 10231000 Nov  9 16:31 src
$ make mt_copy
gcc -O2 -Wall -UDEBUG -pthread -c ../../common.c -o common.o
gcc -O2 -Wall -UDEBUG -pthread -c mt_copy.c -o mt_copy.o
gcc -o mt_copy mt_copy.o common.o -lpthread
$ ./mt_copy 
Usage: ./mt_copy source-file destination-file number-of-threads [1 for verbose-mode]
$ ./mt_copy src dest 3 
$ ls -l dest 
-rw-r--r-- 1 kai kai 10231000 Nov  9 16:31 dest
$ diff src dest 
$ 

(Also try a run with passing '1' as the last (fourth) parameter,
turning verbose mode On. Above, 3 threads were spawned off; they ran in
parallel, each operating on one-third of the file; the first thread
on the first third, the 2nd thread on the next segment and the last thread
on the remaining 1/3 of the file).

 * For details, please refer the book, Ch 18, Advanced File I/O.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <pthread.h>
#include "../../common.h"

#define MIN_THRDS	2
#define MAX_THRDS	5

struct stToThread {
	/* thread # and total # of worker threads in the app */
	int thrdnum, thrdtotal;
	/* File descriptors: source and destination files */
	int fd_src, fd_dest;
	/* {offset, len} pair to perform IO at within the file */
	off_t start_offset;
	size_t len;
	/* Any remaining bytes */
	int rem;
};
static struct stToThread *ToThread[MAX_THRDS];
static int gVerbose = 0;

/* The fd_read() and fd_write() code below is identical to what we did in Ch 
 * A : 'File I/O Essentials' (available online here:
 *  https://www.packtpub.com/sites/default/files/downloads/File_IO_Essentials.pdf ).
 * Except that here, we use the pread(2)/pwrite(2) instead of the 'usual'
 * read(2)/write(2) syscalls.
 */
int fd_read(int fd, void *dbuf, size_t n, off_t off)
{
	ssize_t rd = 0;
	size_t tr = 0;

	do {
		rd = pread(fd, dbuf, n, off);
		if (rd < 0)
			return -1;
		if (rd == 0)
			break;
		tr += rd;
#ifdef DEBUG
		printf(" requested=%9lu, actualrd=%9zu, totalrd=%9zu\n", n, rd,
		       tr);
#endif
	} while (tr < n);
	return tr;
}

int fd_write(int fd, void *sbuf, size_t n, off_t off)
{
	ssize_t wr = 0;
	size_t tw = 0;

	do {
		wr = pwrite(fd, sbuf, n, off);
		if (wr < 0)
			return -1;
		tw += wr;
#ifdef DEBUG
		printf(" requested=%9zu, actualwr=%9zu, totalwr=%9lu\n", n, wr,
		       tw);
#endif
	} while (tw < n);
	return tw;
}

/* __do_mt_copy
 * Read from source file, write into dest file, *at the specified
 * {start_offset, length} pairs.
 */
static int __do_mt_copy(int fd_src, int fd_dest, off_t start_offset, size_t len)
{
	char *buf = NULL;
	ssize_t nr = 0, nw = 0;

	VPRINT("\n*** start off:len :: {%zu:%lu} ***\n", start_offset, len);

/* TODO / Fixme : 
 * If the length is greater than some reasonably large size, we should
 * loop around perhaps a page-sized buffer and perform the IO ...
 */
	buf = malloc(len);
	if (!buf) {
		WARN("malloc (%ld) failed!\n", len);
		return -2;
	}

	nr = fd_read(fd_src, buf, len, start_offset);
	VPRINT(" read %9ld ", nr);
	if (nr < 0) {
		WARN("read on src file failed\n");
		free(buf);
		return -3;
	} else if (nr == 0)   // EOF
		return 0;

	nw = fd_write(fd_dest, buf, nr, start_offset);
	VPRINT("wrote %9ld\n", nw);
	if (nw < 0) {
		WARN("write on dest file failed\n");
		free(buf);
		return -4;
	}

	free(buf);
	return 0;
}

static void cleanup_handler(void *arg)
{
	struct stToThread *pstToThread = (struct stToThread *)arg;
	assert(pstToThread);

	printf("+++ In %s():thrd #%d +++\n", __func__, pstToThread->thrdnum);
	//free(arg);
}

static void *worker(void *msg)
{
	struct stToThread *pstToThread = (struct stToThread *)msg;
	assert(pstToThread);

	/* Install a 'cleanup handler' routine */
	pthread_cleanup_push(cleanup_handler, pstToThread);

	/* ... do the work ... */
	VPRINT("%s():thrd #%d: copy from offset %zu for %lu bytes (rem=%d)\n",
		__func__, 
		pstToThread->thrdnum,
		pstToThread->start_offset,
		pstToThread->len,
		pstToThread->rem
		);
	__do_mt_copy(pstToThread->fd_src, pstToThread->fd_dest,
			pstToThread->start_offset, pstToThread->len);

	/* If there are bytes remaining, have the last thread perform the
	 * last rem IO segment */
	if (pstToThread->rem &&
		pstToThread->thrdnum == pstToThread->thrdtotal-1) {
		VPRINT(" @@@ last thrd #%d, rem = %d bytes\n",
			pstToThread->thrdnum, pstToThread->rem);
		__do_mt_copy(pstToThread->fd_src, pstToThread->fd_dest,
			pstToThread->start_offset + pstToThread->len,
			pstToThread->rem);
	}

	/* *Must* invoke the 'push's counterpart: the cleanup 'pop' routine;
	 * passing 0 as parameter just registers it, it does not actually pop
	 * off and execute the handler.
	 */
	pthread_cleanup_pop(0);
	pthread_exit((void *)0);
}

/*
 * do_mt_copy():
 * Our multithreaded (MT) version of a file copy program.
 */
static int do_mt_copy(int numthrds, int fd_src, int fd_dest, off_t fsize)
{
	int i, ret;
	pthread_t tid[MAX_THRDS];
	pthread_attr_t attr;
	void *stat=0;
	int rem = fsize%numthrds;

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Thread creation loop */
	for (i = 0; i < numthrds; i++) {
		ToThread[i] = calloc(1, sizeof(struct stToThread));
		if (!ToThread[i])
			FATAL("calloc stToThread[%d] failed!\n", i);

		/* --- Inititialize the structure param passed to the worker */
		/* thread # and total # of worker threads in the app */
		ToThread[i]->thrdnum = i;
		ToThread[i]->thrdtotal = numthrds;
		/* File descriptors: source and destination files */
		ToThread[i]->fd_src = fd_src;
		ToThread[i]->fd_dest = fd_dest;

		/* {offset, len} pair to perform IO at within the file */
		ToThread[i]->start_offset = i*(fsize/numthrds);
		/* Bug Tip: I first coded the above line as:
		ToThread[i]->start_offset = i*fsize/numthrds;
		... taking it to be fine. It wasn't; consider, i=2, fsize=14,
		numthrds=3; then it becomes = 2*14/3 which becomes 28/3 = 9
		(integer value); this is NOT correct. It should have been
		2*(14/3) = 2*4 = 8 !
		A typical example of how, when we make assumptions (here
		regarding order of evaluation of an arithmetic expresion), we
		can seriously fail.
		 */
		ToThread[i]->len = fsize/numthrds;

		/* Any remaining bytes */
		ToThread[i]->rem = rem;

		ret = pthread_create(&tid[i], &attr,
				     worker, (void *)ToThread[i]);
		if (ret)
			FATAL("[%d] pthread_create() failed! [%d]\n", i, ret);
		VPRINT("Thread #%d successfully created\n", i);
	}
	pthread_attr_destroy(&attr);

#if 0
	/* Cancel a thread? */
	if (argc >= 2 && atoi(argv[1]) == 1) {
		// Lets send a cancel request to thread A (the first worker thread)
		printf("%s: sending CANCEL REQUEST to worker thread 0 ...\n",
		       __func__);
		ret = pthread_cancel(tid[0]);
		if (ret)
			FATAL("pthread_cancel(thread 0) failed! [%d]\n", ret);
	}
#endif

	/* Thread join loop */
	for (i = 0; i < numthrds; i++) {
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("[%d] pthread_join() failed! [%d]\n", i, ret);
		else {
			VPRINT(" Thread #%d successfully joined; it "
			       "terminated with status=%ld\n", i, (long)stat);
			if (stat == PTHREAD_CANCELED)
				VPRINT("  : was CANCELED\n");
		}
		free(ToThread[i]);
	}
	/* all worker threads have terminated, just return to caller */
	return 0;
}

int main(int argc, char **argv)
{
	int numthrds, fdr, fdw, ret = 0;
	struct stat sb;

	if (argc < 4) {
		fprintf(stderr, "Usage: %s source-file destination-file"
			" number-of-threads [1 for verbose-mode]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	numthrds = atoi(argv[3]);
	if (numthrds < MIN_THRDS || numthrds > MAX_THRDS) {
		fprintf(stderr, "Usage: %s source-file destination-file"
			" number-of-threads [1 for verbose-mode]\n", argv[0]);
		fprintf(stderr, " number-of-threads invalid; should be in"
		" the range {%d,%d}\n", MIN_THRDS, MAX_THRDS);
		exit(EXIT_FAILURE);
	}
	if (argc == 5 && (!strcmp(argv[4], "1")))
		gVerbose = 1;

	/* Open source as read-only */
	fdr = open(argv[1], O_RDONLY);
	if (fdr < 0)
		FATAL("open on source file \"%s\" failed, aborting.\n",
		      argv[1]);

	/* Make the destination file mode match the source file's */
	if (stat(argv[1], &sb) < 0)
		FATAL("stat on %s failed\n", argv[1]);
	VPRINT("Source file \"%s\" mode=0%o size=%lu bytes\n",
		argv[1], sb.st_mode & ACCESSPERMS, sb.st_size);

	/* Create / overwrite destination as write-only for append;
	 * mode=<srcfile's> */
	fdw = open(argv[2], O_CREAT | O_WRONLY, sb.st_mode & ACCESSPERMS);
	if (fdw < 0)
		FATAL("open on destination file \"%s\" failed, aborting.\n",
		      argv[2]);

	VPRINT("# of threads to spawn: %d\n", numthrds);
	if ((ret = do_mt_copy(numthrds, fdr, fdw, sb.st_size)) < 0) {
		close(fdr);
		close(fdw);
		FATAL("docopy() failed: (stat=%d)\n", ret);
	}
	if (gVerbose == 1)
		printf("\n");

	close(fdr);
	close(fdw);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
