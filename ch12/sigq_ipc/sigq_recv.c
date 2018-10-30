/*
 * ch13/sigq_ipc/sigq_recv.c
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
 * This program is the 'consumer' / receiver.
 *
 * For details, please refer the book, Ch 13.
 */
#include "../../common.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

#define SIG_COMM  SIGRTMIN
#define SLP_SEC   3

static volatile sig_atomic_t data_recvd=0;
typedef struct {
	time_t timestamp;
	int signum;
	pid_t sender_pid;
	uid_t sender_uid;
	int data;
} rcv_data_t;
static rcv_data_t recv_data;

/* 
 * read_msg
 * Signal handler for SIG_COMM.
 * The signal's receipt implies a producer has sent us data;
 * read and place the details in the rcv_data_t structure.
 * For reentrant-safety, all signals are masked while this handler runs.
 */
static void read_msg(int signum, siginfo_t *si, void *ctx)
{
	time_t tm;

	if (time(&tm) < 0)
		WARN("time(2) failed\n");

	recv_data.timestamp = tm;
	recv_data.signum = signum;
	recv_data.sender_pid = si->si_pid;
	recv_data.sender_uid = si->si_uid;
	recv_data.data = si->si_value.sival_int;

	data_recvd = 1;
}

static void display_recv_data(void)
{
	char asctm[128];

	ctime_r(&recv_data.timestamp, asctm);
	printf ("Consumer [%d] received data @ %s"
		" signal #  : %2d\n"
		" Producer  : PID=%d : UID=%d\n"
		" data item : %d\n",
		getpid(),
		asctm,
		recv_data.signum,
		recv_data.sender_pid,
		recv_data.sender_uid,
		recv_data.data);
}

int main(int argc, char **argv)
{
	struct sigaction act;

	act.sa_sigaction = read_msg;
	sigfillset(&act.sa_mask); /* disallow all while handling */
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	if (sigaction(SIG_COMM, &act, 0) == -1)
		FATAL("sigaction failure");

	printf("%s: Hey, consumer here [%d]! Awaiting data from producer\n"
		"(will poll every %ds ...)\n",
		argv[0], getpid(), SLP_SEC);

	/* Poll ... not the best way, but just for this demo... */
	while(1) {
		r_sleep(SLP_SEC, 0);
		if (data_recvd) {
			display_recv_data();
			data_recvd = 0;
		}
	}

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
