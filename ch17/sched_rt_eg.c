/*
 * ch17/sched_rt_eg.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 17 : CPU Scheduling
 ****************************************************************
 * Brief Description:
 * This application demonstrates a key facet of the Linux OS: setting the
 * scheduling policy and priority of a thread to make it (soft) realtime.
 * The app has a total of three threads; the first is main() of course; it
 * will spawn two new threads:
 *
 * Thread 0 (main(), really): 
 *	- Runs as SCHED_OTHER (or SCHED_NORMAL). It:
 *	- Queries the priority range of SCHED_FIFO, printing out the values
 *	- Creates two worker threads; they will automatically inherit the
 *	  scheduling policy and priority of main.
 *	- Prints the character 'm' to the terminal in a loop
 *	- Terminates
 *
 * Worker Thread 1:
 *	- Changes it's scheduling policy to SCHED_RR, setting it's real-time 
 *         priority to the value passed on the command line
 *	- Sleeps for 2 seconds (thus blocking on IO, allowing main to get some
 *	  work done)
 *	- Prints the character '1' to the terminal in a loop
 *	- Terminates
 *
 * Worker Thread 2:
 *	 - Changes it's scheduling policy to SCHED_FIFO, setting it's real-time
 *          priority to the value passed on the command line plus 10
 *	 - Sleeps for 4 seconds (allowing Thread 1 to do some work)
 *	 - Prints the character '2' to the terminal in a loop
 *	 - Terminates.
 *
 * For details, please refer the book, Ch 17.
 */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include "../common.h"

#define NTHREADS     2
#define NUMWORK    200

/* Run with:
 * scheduling policy : SCHED_RR
 * realtime priority : rt_prio
 */
void *worker1(void *msg)
{
	struct sched_param p;
	/* The structure used is defined in linux/sched.h as:
	 * struct sched_param {
	 *      int sched_priority;
	 * };
	 */

	printf("  RT Thread p1 (%s():%d:LWP %d):\n"
	       " Setting sched policy to SCHED_RR and RT priority to %ld and sleeping for 2s ...\n",
	       __func__, __LINE__, getpid(), (long)msg);

	/* pthread_setschedparam(3) internally invokes the syscall
	 * sched_setscheduler(2).  */
	p.sched_priority = (long)msg;
	if (pthread_setschedparam(pthread_self(), SCHED_RR, &p))
		WARN("pthread_setschedparam failed\n");

	sleep(2);

	puts("  p1 working");
	DELAY_LOOP('1', NUMWORK);

	puts("  p1: exiting..");
	pthread_exit((void *)0);
}

/* Run with:
 * scheduling policy : SCHED_FIFO
 * realtime priority : rt_prio + 10
 */
void *worker2(void *msg)
{
	struct sched_param p;
	/* The structure used is defined in linux/sched.h as:
	 * struct sched_param {
	 *      int sched_priority;
	 * };
	 */
	long prio = (long)msg;

	printf("  RT Thread p2 (%s():%d:LWP %d):\n"
	       " Setting sched policy to SCHED_FIFO and RT priority to %ld and sleeping for 4s ...\n",
	       __func__, __LINE__, getpid(), (long)msg+10);

	/* pthread_setschedparam(3) internally invokes the syscall
	 * sched_setscheduler(2).  */
	p.sched_priority = prio + 10;
	if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &p))
		WARN("pthread_setschedparam failed\n");
	sleep(4);

	puts("  p2 working");
	DELAY_LOOP('2', NUMWORK);

	puts("  p2 exiting ...");
	pthread_exit((void *)0);
}

int main(int argc, char **argv)
{
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	int ret, min, max;
	long rt_prio = 1;

	if (argc == 1) {
		fprintf(stderr, "Usage: %s realtime-priority\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	min = sched_get_priority_min(SCHED_FIFO);
	if (min == -1)
		FATAL("sched_get_priority_min failure");
	max = sched_get_priority_max(SCHED_FIFO);
	if (max == -1)
		FATAL("sched_get_priority_max failure");
	printf("SCHED_FIFO: priority range is %d to %d\n", min, max);

	rt_prio = atoi(argv[1]);
	if ((rt_prio < min) || (rt_prio > (max - 10)))
		FATAL("%s: Priority value passed (%ld) out of range [%d-%d].\n",
		      argv[0], rt_prio, min, (max - 10));

	if (geteuid() != 0)
		printf("\nNote: to create true RT threads, you need to run this"
		       " program as superuser\n");

	/* Create the two (soft) RT worker threads */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	printf("main: creating RT worker thread #1 ...\n");
	ret = pthread_create(&tid[0], &attr, worker1, (void *)rt_prio);
	if (ret)
		FATAL("pthread_create() #1 failed! [%d]\n", ret);

	printf("main: creating RT worker thread #2 ...\n");
	ret = pthread_create(&tid[1], &attr, worker2, (void *)rt_prio);
	if (ret)
		FATAL("pthread_create() #2 failed! [%d]\n", ret);

	pthread_attr_destroy(&attr);

	DELAY_LOOP('m', NUMWORK+100);

	printf("\nmain: all done, app exiting ...\n");
	pthread_exit((void *)0);
}

/* vi: ts=8 */
