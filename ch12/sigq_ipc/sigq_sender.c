/*
 * ch13/sigq_ipc/sigq_sender.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Linux System Programming"
 *  (c) Kaiwan N Billimoria
 *  Packt Publishers
 *
 * From:
 *  Ch 13 : Signaling Part II
 ****************************************************************
 * Brief Description:
 * A quick demo of IPC-via-signalling; we build a simple
 * 'producer-consumer' app; the 'producer' (sender) process sends a
 * data item - an integer value - to the 'consumer' process (the
 * receiver).
 * This program is the 'producer' / sender.
 *
 * For details, please refer the book, Ch 13.
 */
#include "../../common.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define SIG_COMM  SIGRTMIN

static int send_peer(pid_t target, int sig, int val)
{
	union sigval sv;

	if (kill(target, 0) < 0)
		return -1;

	sv.sival_int = val;
	if (sigqueue(target, sig, sv) == -1)
		return -2;
	return 0;
}

int main (int argc, char **argv)
{
	int ret=0;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s pid-to-send-to value-to-send[int]\n", argv[0]);
		exit (1);
	}
	ret = send_peer(atol(argv[1]), SIG_COMM, atoi(argv[2]));
	switch (ret) {
		case -1 : FATAL("Target PID invalid or no permission\n");
		case -2 : FATAL("sigqueue failed\n");
		case 0  : printf("Producer [%d]: sent signal %d to PID %ld with data item %d\n",
				getpid(), SIG_COMM, atol(argv[1]), atoi(argv[2]));
	}
	exit (EXIT_SUCCESS);
}

/* vi: ts=4 */
