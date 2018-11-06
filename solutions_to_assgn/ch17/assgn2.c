/*
 * Assignment Solution
 * solutions_to_assgn/ch17/assgn2.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 17 : CPU Scheduling on Linux
 ****************************************************************
 * 
[Q1. Write a multithreaded app that has a few threads running in a tight
CPU-bound loop; then, do the following:
Use the chrt(1) utility to query and modify the various thread's scheduling
policy and real-time priority.
Use the taskset(1) utility to query and modify the various thread's CPU
affinity masks.]

Q2. Enhance the above app's code to perform the preceding operations itself,
that is, use system calls (or the pthread wrapper APIs) to modify thread's
scheduling policy / real-time priority and CPU affinity mask.

Solution:
Study the code, and importantly, the comments within it.

Interestingly (as mentioned below), the existing glibc headers do not seem to
define the struct sched_attr nor cater to the sched_[get|set]attr(2) system
calls. So, we need to define this ourselves and keep a wrapper using syscall(2)
to invoke the system calls. Apparently, this is a known issue.
Also, chrt(1)'s source code uses this very approach!
  https://github.com/karelzak/util-linux/blob/master/schedutils/chrt.c
So, long story short, we copy the same (OSS) code from chrt here and use it.


Test run (we build and run the 'debug' ver as then, the DELAY_LOOP[_SILENT]() 
macro takes effect):

$ ./assgn2_dbg 
With this little program, you can specify which thread of three worker threads
to change to a different scheduling poicy and real-time priority (also specified).

Usage: ./assgn2_dbg thread# sched-policy realtime-priority ; where:
  parameter 1 : thread#      : is any of 0, 1, or 2
  parameter 2 : sched-policy : is any of 0, 1, or 2 (for SCHED_OTHER, SCHED_FIFO or SCHED_RR resp)
  parameter 3 : realtime-prio: is any # between {1, 99}
$ 

$ sudo taskset 01 ./assgn2_dbg 2 1 81
*** Worker Thread #2 ***
*** Worker Thread #1 ***
*** Worker Thread #0 ***
0 1 2 Scheduling
 Thread #    Policy            Prio    Nice    Flags
 ---------   ------            ----    ----    -----
[Thread #0] SCHED_OTHER         0        0      0x0
[Thread #1] SCHED_OTHER         0        0      0x0
[Thread #2] SCHED_OTHER         0        0      0x0
* Now making thread #2 sched policy SCHED_FIFO and realtime-prio 81 *
2 [Thread #2] SCHED_FIFO         81        0      0x0
2 2 2 2 2 1 0 2 2 2 2 2 2 0 1 2 2 2 2 2 2 1 0 2 2 2 2 2 2 0 1 2 2 2 2 2 
1 0 1 0 1 0 1 0 0 1 0 1 0 1 1 0 1 0 0 1 0 1 1 0 0 1 0 1 0 1 1 0 0 1 0 1 0 1 0 1 1 0 1 0 1 0 0 1 1 0 

 Thread #0 successfully joined; it terminated with status=0
 Thread #1 successfully joined; it terminated with status=0
 Thread #2 successfully joined; it terminated with status=0
$ 

Notice how we correctly change only the specified thread's sched policy and RT
prio above to whatever the user wants. Also notice how, when the RT thread runs
the other starve; however, we "leak" some CPU bandwidth by sleeping for 100ms
(see the code below). 
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <pthread.h>
#include "../../common.h"
#include <sched.h>

#define NTHREADS	3

struct stToThread {
	int thrdnum;
	/* Add other members as required */
};
static struct stToThread *ToThread[NTHREADS];

static pthread_t tid[NTHREADS];	// lib-level thread ID
static pid_t tpid[NTHREADS];	// kernel-level thread PID

#include <sys/syscall.h>
static pid_t gettid(void)
{
	return syscall(SYS_gettid);
}

static void cleanup_handler(void *arg)
{
	printf("+++ In %s +++\n", __func__);
	//free(arg);
}

static void *worker(void *msg)
{
	struct stToThread *pstToThread = (struct stToThread *)msg;
	int i;

	assert(pstToThread);
	// Populate our actual kernel-level PID (TID)
	tpid[pstToThread->thrdnum] = gettid();

	/* Install a 'cleanup handler' routine */
	pthread_cleanup_push(cleanup_handler, pstToThread);

	/* Just have the worker thread perform some CPU intensive work;
	 * we do so by invoking our DELAY_LOOP_SILENT() macro..
	 */
	printf("*** Worker Thread #%d ***\n", pstToThread->thrdnum);
	for (i = 0; i < 30; i++) {
		DELAY_LOOP_SILENT(25);
		printf("%d ", pstToThread->thrdnum);
		fflush(stdout);
		r_sleep(0, 100000000);	/* sleep for 100ms allowing a little "leakage"
					   of CPU bandwidth to the non-RT threads, poor fellows... :-) */
	}
	printf("\n");

	/* *Must* invoke the 'push's counterpart: the cleanup 'pop' routine;
	 * passing 0 as parameter just registers it, it does not actually pop
	 * off and execute the handler.
	 */
	pthread_cleanup_pop(0);
	pthread_exit((void *)0);
}

/*--- IMPortant!
 The existing glibc headers do not seem to define the struct sched_attr nor
 cater to the sched_[get|set]attr(2) system calls. So, we need to define this
 ourselves and keep a wrapper using syscall(2) to invoke the system calls.
 Apparently, this is a known issue (see this SO Q&A :
 https://stackoverflow.com/questions/45692699/build-against-newer-linux-headers-than-libc-is-built-using )
 and, as the above link points out, chrt(1)'s source code uses this very
 approach!
 Ref: https://github.com/karelzak/util-linux/blob/master/schedutils/chrt.c

 So, long story short, we copy the same (OSS) code from chrt here and use it.
 */
#if defined (__linux__) && !defined(HAVE_SCHED_SETATTR)
#include <sys/syscall.h>
#endif

/* usable kernel-headers, but old glibc-headers */
#if defined (__linux__) && !defined(SYS_sched_setattr) && defined(__NR_sched_setattr)
#define SYS_sched_setattr __NR_sched_setattr
#endif

#if defined (__linux__) && !defined(SYS_sched_getattr) && defined(__NR_sched_getattr)
#define SYS_sched_getattr __NR_sched_getattr
#endif

#if defined (__linux__) && !defined(HAVE_SCHED_SETATTR) && defined(SYS_sched_setattr)
#define HAVE_SCHED_SETATTR

typedef int int32_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

struct sched_attr {
	uint32_t size;
	uint32_t sched_policy;
	uint64_t sched_flags;

	/* SCHED_NORMAL, SCHED_BATCH */
	int32_t sched_nice;

	/* SCHED_FIFO, SCHED_RR */
	uint32_t sched_priority;

	/* SCHED_DEADLINE (nsec) */
	uint64_t sched_runtime;
	uint64_t sched_deadline;
	uint64_t sched_period;
};

static int sched_setattr(pid_t pid, const struct sched_attr *attr,
			 unsigned int flags)
{
	return syscall(SYS_sched_setattr, pid, attr, flags);
}

static int sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size,
			 unsigned int flags)
{
	return syscall(SYS_sched_getattr, pid, attr, size, flags);
}
#endif
//---

static void get_show_sched(int thrdnum, pid_t threadid)
{
	struct sched_attr schdattr;

	schdattr.size = sizeof(struct sched_attr);
	if (sched_getattr(threadid, &schdattr, sizeof(struct sched_attr), 0) <
	    0)
		WARN("sched_getattr(2) on thread %d failed; exited?\n",
		     threadid);

	printf("[Thread #%d] %-16s ",
	       thrdnum,
	       (schdattr.sched_policy == SCHED_FIFO) ? "SCHED_FIFO" :
	       (schdattr.sched_policy == SCHED_RR) ? "SCHED_RR" :
	       (schdattr.sched_policy == SCHED_OTHER) ? "SCHED_OTHER" :
	       (schdattr.sched_policy == SCHED_DEADLINE) ? "SCHED_DEADLINE" :
	       (schdattr.sched_policy == SCHED_BATCH) ? "SCHED_BATCH" :
	       "<unknown>");
	printf("  %2d       %2d      0x%lx",
	       schdattr.sched_priority,
	       schdattr.sched_nice, schdattr.sched_flags);
	printf("\n");
}

#include <errno.h>
static void set_sched(int thrdnum, int policy, int rtprio)
{
	struct sched_attr schdattr;

	schdattr.size = sizeof(struct sched_attr);
	schdattr.sched_policy = policy;
	schdattr.sched_priority = rtprio;

	if (tpid[thrdnum]) {
		if (sched_setattr(tpid[thrdnum], &schdattr, 0) < 0)
			WARN("sched_setattr(2) on thread #%d failed: "
			     "usually, either permissions issue (not root) or thread"
			     " already exited\n", thrdnum);
	}
}

static void change_sched_policy_prio(int thrdnum, int thrd2change, int policy,
				     int rtprio)
{
	if (tpid[thrdnum])
		get_show_sched(thrdnum, tpid[thrdnum]);

	/* Change the required thread's scheduling policy and priority */
	if (thrdnum == thrd2change) {
		printf
		    ("* Now making thread #%d sched policy %s and realtime-prio %d *\n",
		     thrd2change,
		       (policy == 0) ? "SCHED_OTHER"
		     : (policy == 1) ? "SCHED_FIFO"
		     : (policy == 2) ? "SCHED_RR" : "<unknown>", rtprio);

		set_sched(thrd2change, policy, rtprio);
		get_show_sched(thrd2change, tpid[thrd2change]);
	}
}

static inline void usage(char *nm)
{
	fprintf(stderr,
		"With this little program, you can specify which thread of three worker threads\n"
		"to change to a different scheduling poicy and real-time priority (also specified).\n"
		"\nUsage: %s thread# sched-policy realtime-priority ; where:\n"
		"  parameter 1 : thread#      : is any of 0, 1, or 2\n"
		"  parameter 2 : sched-policy : is any of 0, 1, or 2 (for SCHED_OTHER, SCHED_FIFO or SCHED_RR resp)\n"
		"  parameter 3 : realtime-prio: is any # between {%d, %d}\n",
		nm, sched_get_priority_min(SCHED_FIFO),
		sched_get_priority_max(SCHED_FIFO)
	    );
}

#define ARGS_FAIL(param) do {		 \
	if (param != -1)		 \
		fprintf(stderr, "### %s: (at least) parameter %d is invalid, pl correct & retry.\n\n", \
			argv[0], param); \
	usage(argv[0]);			 \
	exit (EXIT_FAILURE);		 \
} while(0)

int main(int argc, char **argv)
{
	int thrdnum, policy, rtprio;
	int i, ret;
	pthread_attr_t attr;
	void *stat = 0;

	//--- Argument validation
	if (argc != 4)
		ARGS_FAIL(-1);

	thrdnum = atoi(argv[1]);
	if (thrdnum < 0 || thrdnum >= NTHREADS)
		ARGS_FAIL(1);

	policy = atoi(argv[2]);
	if (policy < 0 || policy >= 3)
		ARGS_FAIL(2);

	rtprio = atoi(argv[3]);
	if (rtprio < sched_get_priority_min(SCHED_FIFO)
	    || rtprio > sched_get_priority_max(SCHED_FIFO))
		ARGS_FAIL(3);

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Thread creation loop */
	for (i = 0; i < NTHREADS; i++) {
		ToThread[i] = calloc(1, sizeof(struct stToThread));
		if (!ToThread[i])
			FATAL("calloc stToThread[%d] failed!\n", i);

		ToThread[i]->thrdnum = i;
		ret = pthread_create(&tid[i], &attr,
				     worker, (void *)ToThread[i]);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	sleep(1);		// silly; give the threads some time to get started...
	printf("Scheduling\n"
	       " Thread #    Policy            Prio    Nice    Flags\n"
	       " ---------   ------            ----    ----    -----\n");
	for (i = 0; i < NTHREADS; i++)
		change_sched_policy_prio(i, thrdnum, policy, rtprio);

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
	/* all worker threads have terminated, have main() call it a day! */
	pthread_exit(NULL);
}

/* vi: ts=8 */
