/*
 * ch18/sgio_simple.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 18 : Advanced File IO
 ****************************************************************
 * Brief Description:
 * A demo of using the performance-superior scatter-gather (SG) IO
 * technique over the traditional {lseek, write} syscalls pairs to
 * perform discontiguous writes to a file.
 * We show both techniques, depending on the parameter passed.
 * Also, as an aside, we demo how using the 'goto' statement for
 * performing 'centralized error handling' is a superior technique!
 *
 * For details, please refer the book, Ch 18.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include "../common.h"

/*
 * How we'd like the file to look:
 * +------------+-----------+------------+-----------+------------+-----------+
 * |            | ... A ... |            | ... B ... |            | ... C ... |
 * +------------+-----------+------------+-----------+------------+-----------+
 * | A_HOLE_LEN |   A_LEN   | B_HOLE_LEN |   B_LEN   | C_HOLE_LEN |  C_LEN    |
 * +------------+-----------+------------+-----------+------------+-----------+
 *              ^                        ^                        ^
 *              A_START_OFF              B_START_OFF              C_START_OFF
 */
#define A_HOLE_LEN     10
#define A_START_OFF    A_HOLE_LEN
#define A_LEN          20

#define B_HOLE_LEN    100
#define B_START_OFF   (A_HOLE_LEN+A_LEN+B_HOLE_LEN)
#define B_LEN          30

#define C_HOLE_LEN     20
#define C_START_OFF   (A_HOLE_LEN+A_LEN+B_HOLE_LEN+B_LEN+C_HOLE_LEN)
#define C_LEN          42

char *gbufA, *gbufB, *gbufC, *gbuf_hole;

#if 0
/* The traditional way of error handling (see below pl) */
static void setup_buffers(void)
{
	/* buffer 'A': len A_LEN bytes, content 'A' */
	gbufA = malloc(A_LEN);
	if (!gbufA)
		FATAL("malloc on gbufA failed\n");
	memset(gbufA, 'A', A_LEN);

	/* buffer 'B': len B_LEN bytes, content 'B' */
	gbufB = malloc(B_LEN);
	if (!gbufB) {
		free(gbufA);
		FATAL("malloc on gbufB failed\n");
	}
	memset(gbufB, 'B', B_LEN);

	/* buffer 'C': len C_LEN bytes, content 'C' */
	gbufC = malloc(C_LEN);
	if (!gbufC) {
		free(gbufB);
		free(gbufA);
		FATAL("malloc on gbufC failed\n");
	}
	memset(gbufB, 'C', C_LEN);

	/* buffer 'hole': len A_HOLE_LEN + B_HOLE_LEN + C_HOLE_LEN bytes, content nul */
	gbuf_hole = malloc(A_HOLE_LEN + B_HOLE_LEN + C_HOLE_LEN);
	if (!gbuf_hole) {
		free(gbufC);
		free(gbufB);
		free(gbufA);
		FATAL("malloc on gbuf_hole failed\n");
	}
	memset(gbuf_hole, 0, A_HOLE_LEN + B_HOLE_LEN + C_HOLE_LEN);
}
#else
/* The Linux kernel way of error handling - via a 'centralized exit' path
 * via local goto's. Not convinced of it's efficacy? *Please* do read:
 * https://www.kernel.org/doc/html/v4.15/process/coding-style.html#centralized-exiting-of-functions
 * http://koblents.com/Ches/Links/Month-Mar-2013/20-Using-Goto-in-Linux-Kernel-Code/
 */
static int setup_buffers_goto(void)
{
	int ret = -1;

	printf("In %s()\n", __func__);

	/* Yes, we can kind of optimize the below code by using macros;
	 * but here, pedantically, we just want to keep the code very
	 * clear and readable.
	 */

	/* buffer 'A': len A_LEN bytes, content 'A' */
	gbufA = malloc(A_LEN);
	if (!gbufA)
		goto out;
	memset(gbufA, 'A', A_LEN);

	/* buffer 'B': len B_LEN bytes, content 'B' */
	ret = -2;
	gbufB = malloc(B_LEN);
	if (!gbufB)
		goto out_freeA;
	memset(gbufB, 'B', B_LEN);

	/* buffer 'C': len C_LEN bytes, content 'C' */
	ret = -3;
	gbufC = malloc(C_LEN);
	if (!gbufC)
		goto out_freeB;
	memset(gbufC, 'C', C_LEN);

	/* buffer 'hole': len A_HOLE_LEN + B_HOLE_LEN + C_HOLE_LEN bytes, content nul */
	ret = -4;
	gbuf_hole = malloc(A_HOLE_LEN + B_HOLE_LEN + C_HOLE_LEN);
	if (!gbuf_hole)
		goto out_freeC;
	memset(gbuf_hole, 0, A_HOLE_LEN + B_HOLE_LEN + C_HOLE_LEN);

	return 0;
out_freeC:
	free(gbufC);
out_freeB:
	free(gbufB);
out_freeA:
	free(gbufA);
out:
	return ret;
}
#endif

#define SEEK_POS(mark) \
	printf("%s: curr seek position: %ld\n", \
			mark, lseek(fd, 0, SEEK_CUR));

/*
 * How we'd like the file to look:
 * +------------+-----------+------------+-----------+------------+-----------+
 * |            | ... A ... |            | ... B ... |            | ... C ... |
 * +------------+-----------+------------+-----------+------------+-----------+
 * | A_HOLE_LEN |   A_LEN   | B_HOLE_LEN |   B_LEN   | C_HOLE_LEN |  C_LEN    |
 * +------------+-----------+------------+-----------+------------+-----------+
 *              ^                        ^                        ^
 *              A_START_OFF              B_START_OFF              C_START_OFF
 *
 * This function will achieve three writes the traditional way, by explicitly
 * seeking to each location and then invoking the write syscall. The 'holes'
 * though, will automatically appear as the lseek skips bytes..
 */ 
static int wr_discontig_the_normal_way(int fd)
{
	printf("In %s()\n", __func__);

	/* A: {seek_to A_START_OFF, write gbufA for A_LEN bytes} */
	if (lseek(fd, A_START_OFF, SEEK_SET) < 0)
		FATAL("lseek A failed\n");
	if (write(fd, gbufA, A_LEN) < 0)
		FATAL("write A failed\n");

	/* B: {seek_to B_START_OFF, write gbufB for B_LEN bytes} */
	if (lseek(fd, B_START_OFF, SEEK_SET) < 0)
		FATAL("lseek B failed\n");
	if (write(fd, gbufB, B_LEN) < 0)
		FATAL("write B failed\n");

	/* C: {seek_to C_START_OFF, write gbufC for C_LEN bytes} */
	if (lseek(fd, C_START_OFF, SEEK_SET) < 0)
		FATAL("lseek C failed\n");
	if (write(fd, gbufC, C_LEN) < 0)
		FATAL("write C failed\n");

	return 0;
}

static void show_iovec(struct iovec *vec, int nvec)
{
	int i;
	
	if (nvec <= 0 || !vec)
		return;

	printf("iovec#      Base addr   Length\n");
	for (i=0; i<nvec; i++)
		printf("%2d: %16p  %6ld\n", i, vec[i].iov_base, vec[i].iov_len);
}

/*
 * How we'd like the file to look:
 * +------------+-----------+------------+-----------+------------+-----------+
 * |            | ... A ... |            | ... B ... |            | ... C ... |
 * +------------+-----------+------------+-----------+------------+-----------+
 * | A_HOLE_LEN |   A_LEN   | B_HOLE_LEN |   B_LEN   | C_HOLE_LEN |  C_LEN    |
 * +------------+-----------+------------+-----------+------------+-----------+
 *              ^                        ^                        ^
 *              A_START_OFF              B_START_OFF              C_START_OFF
 *
 * This function will achieve all six writes with _one_ system call, the
 * SG-IO writev, and atomically!
 */ 
static int wr_discontig_the_better_SGIO_way(int fd)
{
	struct iovec iov[6];
	int i=0;

	printf("In %s()\n", __func__);

	/* We don't want to call lseek of course; so we emulate the seek
	 * by introducing segments that are just "holes" in the file. */

	/* A: {seek_to A_START_OFF, write gbufA for A_LEN bytes} */
	iov[i].iov_base = gbuf_hole;
	iov[i].iov_len = A_HOLE_LEN;
	i ++;
	iov[i].iov_base = gbufA;
	iov[i].iov_len = A_LEN;

	/* B: {seek_to B_START_OFF, write gbufB for B_LEN bytes} */
	i ++;
	iov[i].iov_base = gbuf_hole;
	iov[i].iov_len = B_HOLE_LEN;
	i ++;
	iov[i].iov_base = gbufB;
	iov[i].iov_len = B_LEN;

	/* C: {seek_to C_START_OFF, write gbufC for C_LEN bytes} */
	i ++;
	iov[i].iov_base = gbuf_hole;
	iov[i].iov_len = C_HOLE_LEN;
	i ++;
	iov[i].iov_base = gbufC;
	iov[i].iov_len = C_LEN;
	i ++;

#if 0
	show_iovec(iov, i);
#endif
	/* Perform all six discontiguous writes in order and atomically! */
	if (writev(fd, iov, i) < 0)
		return -1;
	return 0;
}

const char *testfile = "tmptest";

int main(int argc, char **argv)
{
	int fd, opt;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s use-method-option\n"
				" 0 = traditional lseek/write method\n"
				" 1 = better SG IO method\n"
				, argv[0]);
		exit(EXIT_FAILURE);
	}
	opt = atoi(argv[1]);
	if (opt != 0 && opt != 1) {
		fprintf(stderr, "Usage: %s use-method-option\n"
				" 0 = traditional lseek/write method\n"
				" 1 = better SG IO method\n"
				, argv[0]);
		exit(EXIT_FAILURE);
	}

#if (USE_LOCAL_GOTO == 0)
	setup_buffers();
#else
	if (setup_buffers_goto() < 0)
		FATAL("Buffer setup failed\n");
#endif

	unlink(testfile);
	umask(0);
	fd = open(testfile, O_CREAT|O_RDWR,
			S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
	if (fd < 0)
		FATAL("open on %s failed\n", argv[1]);

	if (opt == 0)
		wr_discontig_the_normal_way(fd);
	else
		wr_discontig_the_better_SGIO_way(fd);

	close(fd);
	free(gbufC);
	free(gbufB);
	free(gbufA);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
