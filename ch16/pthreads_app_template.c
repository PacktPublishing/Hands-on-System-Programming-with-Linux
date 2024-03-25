/*
 * ch16/pthreads_app_template.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 16 : Multithreading Part 3
 ****************************************************************
 * 
 * A quick and very much *incomplete* generic template for writing Pthreads
 * apps!
 * _Please Note_ that this is Not meant to be a full working 'model' Pthreads
 *  app; nevertheless, it might serve the reader as a useful starting point to
 * build upon; please refer to and use it in that spirit.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "../common.h"

#define FATAL_ERR	1
#define NON_FATAL	0

#define MTX_LOCK(mutex, fatal)	do {				\
	int err;							\
	if ((err = pthread_mutex_lock(mutex))) {				\
		fprintf(stderr, "pthread_mutex_lock failed! : %s\n", strerror (err));	  \
		/* any clean up can go here ... */			\
		if (fatal) {						\
			fprintf(stderr, "Aborting now...\n");		\
			exit(1);					\
		}							\
		else { 						\
			fprintf(stderr, "*WARNING* pthread_mutex_lock failed!\n");	\
		}									\
	}										\
} while (0)

#define MTX_UNLOCK(mutex, fatal)	do {		\
	int err;							\
	if ((err = pthread_mutex_unlock(mutex))) {					\
		fprintf (stderr, "pthread_mutex_unlock failed! : %s\n", strerror (err));		\
		/* any clean up can go here ... */					\
		if (fatal) {								\
			fprintf(stderr, "Aborting now...\n");				\
			exit (1);							\
		}									\
		else {									\
			fprintf(stderr, "*WARNING* pthread_mutex_unlock failed!\n");	\
		}									\
	}										\
} while (0)

#define NTHREADS	3
#define PTHREAD_STACK	(8*1024*1024)

static int signal_handled = -1;
static pthread_mutex_t sig_mutex = PTHREAD_MUTEX_INITIALIZER;

struct stToThread {
	int thrdnum;
	int start_num, end_num;
	char *iobuf;
};
static struct stToThread *ToThread[NTHREADS];


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
	
	// init ... can disable cancellation here
	// ...
	// can enable cancellation here

	/* Install a 'cleanup handler' routine */
	pthread_cleanup_push(cleanup_handler, pstToThread);


	/* ... do the work ... */


	/* *Must* invoke the 'push's counterpart: the cleanup 'pop' routine;
	 * passing 0 as parameter just registers it, it does not actually pop
	 * off and execute the handler.
	 */
	pthread_cleanup_pop(0);
	pthread_exit((void *)0);
}

/*
 * signal_handler() is the thread that handles all signal catching
 * on behalf of all the other threads; it does this by waiting for signals
 * on sigwait().
 *
 * This is fine for the "normal" POSIX thread/signal handling but poses a
 * special case on Linux. This is as the LinuxThreads implementation does
 * not have a concept of sending a signal to the overall "process". On Linux,
 * every process is essentially a thread. So if one thread calls sigwait() &
 * all other threads block all signals, only the signals specifically sent to
 * the sigwait-ing thread will be processed. So, depending on your application,
 * this could mean that on LinuxThreads, you have no choice but to install
 * an asynchronous signal handler for each thread.
 */
static void *signal_handler(void *arg)
{
	sigset_t sigset;
	int sig;

	printf("Dedicated signal_handler() thread alive..\n");
	while (1) {
		/* Wait for any/all signals */
		if (sigfillset(&sigset) == -1) {
			perror("signal_handler: sigfillset failed");
			/* clean up */
			exit(1);
		}
		/*
		 * Can use the sigwaitinfo(2) (or even the sigtimedwait(2)) syscalls
		 * to get detailed info on what happened from the kernel !
		 */
		if (sigwait(&sigset, &sig) < 0) {
			perror("signal_handler: sigwait failed");
			/* clean up */
			exit(1);
		}
		/* Note on sigwait():
		 *    sigwait suspends the calling thread until one of (any of)
		 *    the  signals  in set is delivered to the calling thread.
		 *    It then stores the number of the signal received in the
		 *    location  pointed  to  by "sig" and returns. The  signals
		 *    in set must be blocked and not ignored on entrance to
		 *    sigwait(). If the delivered signal has a  signal handler
		 *    function attached, that function is *not* called.
		 */

		/* We've caught a signal!
		 * Here, as a demo, we're setting a global (hence the locking) to the
		 * signal caught. In a 'real' app, handle the signal here itself..
		 */
		switch (sig) {
		case SIGINT:
			MTX_LOCK(&sig_mutex, FATAL_ERR);
			signal_handled = SIGINT;
			MTX_UNLOCK(&sig_mutex, FATAL_ERR);
			break;

		case SIGQUIT:
			MTX_LOCK(&sig_mutex, FATAL_ERR);
			signal_handled = SIGQUIT;
			MTX_UNLOCK(&sig_mutex, FATAL_ERR);
			break;

		case SIGIO:
			MTX_LOCK(&sig_mutex, FATAL_ERR);
			signal_handled = SIGIO;
			MTX_UNLOCK(&sig_mutex, FATAL_ERR);
			break;
			/* ... */
		default:
			MTX_LOCK(&sig_mutex, FATAL_ERR);
			signal_handled = sig;
			MTX_UNLOCK(&sig_mutex, FATAL_ERR);
			break;
		}		// switch
		printf("!!! %s(): caught signal #%d !!!\n", __func__, sig);
	}			// while
	return (void *)0;
}

/*
 * Handler for the synchronous / fatal signals:
 * SIGSEGV / SIGBUS / SIGABRT / SIGFPE / SIGILL / SIGIOT
 */
static void fatal_sigs_handler(int signum, siginfo_t * siginfo, void *rest)
{
	static volatile sig_atomic_t c = 0;

	printf("\n*** %s(): [%d] PID %d", __func__, ++c, getpid());
	printf("	received signal %d. errno=%d\n"
	       " Cause/Origin: (si_code=%d): ",
	       signum, siginfo->si_errno, siginfo->si_code);

	switch (siginfo->si_code) {
	case SI_USER:
		printf("user\n");
		break;
	case SI_KERNEL:
		printf("kernel\n");
		break;
	case SI_QUEUE:
		printf("queue\n");
		break;
	case SI_TIMER:
		printf("timer\n");
		break;
	case SI_MESGQ:
		printf("mesgq\n");
		break;
	case SI_ASYNCIO:
		printf("async io\n");
		break;
	case SI_SIGIO:
		printf("sigio\n");
		break;
	case SI_TKILL:
		printf("t[g]kill\n");
		break;
	// Other poss values si_code can have for SIGSEGV
	case SEGV_MAPERR:
		printf("SEGV_MAPERR: address not mapped to object\n");
		break;
	case SEGV_ACCERR:
		printf("SEGV_ACCERR: invalid permissions for mapped object\n");
		break;
/*
 * Now it seems fine...
 * OLD: SEGV_BNDERR and SEGV_PKUERR result in compile failure ??  * Qs asked on SO here:
 *https://stackoverflow.com/questions/45229308/attempting-to-make-use-of-segv-bnderr-and-segv-pkuerr-in-a-sigsegv-signal-handle
*/
#if 1
	case SEGV_BNDERR:	/* 3.19 onward */
		printf("SEGV_BNDERR: failed address bound checks\n");
	case SEGV_PKUERR:	/* 4.6 onward */
		printf
		    ("SEGV_PKUERR: access denied by memory-protection keys\n");
#endif
	default:
		printf("-none-\n");
	}
	printf(" Faulting addr=%p\n", siginfo->si_addr);

	/*
	 * Can reset signal action to default and raise it on ourself,
	 * to get the kernel to emit a core dump
	 */
	if (signal(SIGSEGV, SIG_DFL) == SIG_ERR)
		fprintf(stderr, "signal -reverting SIGSEGV to default- failed");
	if (raise(SIGSEGV))
		fprintf(stderr, "raise SIGSEGV failed");

#if 1
	exit(1);
#else
	abort();
#endif
}

/**
 * setup_altsigstack - Helper function to set alternate stack for sig-handler
 * @stack_sz:	required stack size
 *
 * Return: 0 on success, -ve errno on failure
 */
int setup_altsigstack(size_t stack_sz)
{
	stack_t ss;

	printf("Alt signal stack size = %zu bytes\n", stack_sz);
	ss.ss_sp = malloc(stack_sz);
	if (!ss.ss_sp) {
		printf("malloc(%zu) for alt sig stack failed\n", stack_sz);
		return -ENOMEM;
	}

	ss.ss_size = stack_sz;
	ss.ss_flags = 0;
	if (sigaltstack(&ss, NULL) == -1) {
		printf("sigaltstack for size %zu failed!\n", stack_sz);
		return -errno;
	}
	printf("Alt signal stack uva (user virt addr) = %p\n", ss.ss_sp);

	return 0;
}

/*
 * Signal Handling in MT apps:
 * Strategy:
 *  - Mask (block) all signals in the main thread
 *  - Now any thread created by main inherit it's signal mask, which means that
 *    all signals will be blocked out in all subsequently created threads;
 *    EXCEPTION:
 *      Do NOT handle the kernel synchronous signals this way - the ones sent
 *      to the faulting thread on a fault/bug:
 *	 SIGSEGV / SIGBUS / SIGABRT [/SIGIOT] / SIGFPE / SIGILL
 *	For them, use the usual sigaction(2) style handling.
 *  - Create a separate signal handling thread that only catches all
 *    required signals & handles them; it catches signals by calling sigwait().
 */
static void init_signal_handling_for_mt(void)
{
	sigset_t sigset;	// used for signal mask
	struct sigaction act;

	/*
	 * Block all, well, most, signals here in the main thread.
	 * Now all subsequently created threads also block all signals.
	 */
	if (sigfillset(&sigset) < 0) {
		perror("sigfillset");
		exit(1);
	}
	/* Do NOT block the synchronous (or 'fatal') signals - the ones sent
	 * to the faulting thread on a fault/bug:
	 * SIGSEGV / SIGBUS / SIGABRT [/SIGIOT] / SIGFPE / SIGILL
	 */
	if (sigdelset(&sigset, SIGSEGV) < 0) {
		perror("sigdelset");
		exit(1);
	}
	if (sigdelset(&sigset, SIGBUS) < 0) {
		perror("sigdelset");
		exit(1);
	}
	if (sigdelset(&sigset, SIGABRT) < 0) {
		perror("sigdelset");
		exit(1);
	}
	if (sigdelset(&sigset, SIGFPE) < 0) {
		perror("sigdelset");
		exit(1);
	}
	if (sigdelset(&sigset, SIGILL) < 0) {
		perror("sigdelset");
		exit(1);
	}
	/* Right, lets mask all the other signals ... */
	if (pthread_sigmask(SIG_BLOCK, &sigset, NULL)) {
		perror("main: pthread_sigmask failed");
		/* clean up */
		exit(1);
	}

	/* Use a separate stack for signal handling via the SA_ONSTACK;
	 * This is critical, especially for handling the SIGSEGV; think on it, what
	 * if this process crashes due to stack overflow; then it will receive the
	 * SIGSEGV from the kernel (when it attempts to eat into unmapped memory
	 * following the end of the stack)! The SIGSEGV signal handler must now run
	 * But where? It cannot on the old stack - it's now corrupt! Hence, the
	 * need for an alternate signal stack !
	 */
	if (setup_altsigstack(10 * 1024 * 1024) < 0) {
		fprintf(stderr, "setting up alt sig stack failed\n");
		exit(1);
	}

	/* Handle the kernel synchronous (fatal) signals via sigaction(2) */
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = fatal_sigs_handler;
	act.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGSEGV, &act, 0) == -1) {
		perror("sigaction");
		exit(1);
	}
	if (sigaction(SIGBUS, &act, 0) == -1) {
		perror("sigaction");
		exit(1);
	}
	if (sigaction(SIGABRT, &act, 0) == -1) {
		perror("sigaction");
		exit(1);
	}
	if (sigaction(SIGFPE, &act, 0) == -1) {
		perror("sigaction");
		exit(1);
	}
	if (sigaction(SIGILL, &act, 0) == -1) {
		perror("sigaction");
		exit(1);
	}
}


int main(int argc, char **argv)
{
	int i, ret;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	void *stat=0;

#if 0
	/* RT apps: lock memory */
	printf("RT app: lock memory\n");
	if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
		fprintf(stderr, "mlockall failed: %m\n");
		exit(-2);
	}
#endif

	init_signal_handling_for_mt();

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	/* Set a specific stack size; PTHREAD_STACK_MIN is only 16 KB, so lets
	 * make it larger (8 MB)
	 */
	printf("Set pthread stack size to %d MB\n", PTHREAD_STACK/(1024*1024));
	ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK);
	if (ret) {
		fprintf(stderr, "pthread setstacksize failed [%d]\n", ret);
		exit(EXIT_FAILURE);
	}

	/* Signals : Create a dedicated signal handling thread */
	if (pthread_create(&tid[0], &attr, signal_handler, NULL)) {
		fprintf(stderr, "%s: signal handler thread creation failure..\n", argv[0]);
		perror("main: pthread_create");
		/* clean up */
		exit(1);
	}

	/* Thread creation loop */
	for (i = 1; i < NTHREADS; i++) {
		ToThread[i] = calloc(1, sizeof(struct stToThread));
		if (!ToThread[i])
			FATAL("calloc stToThread[%d] failed!\n", i);

		ToThread[i]->thrdnum = i;
		ToThread[i]->start_num = 1;
		ToThread[i]->end_num = 50;
		ret = pthread_create(&tid[i], &attr,
				     worker, (void *)ToThread[i]);
		if (ret)
			FATAL("[%d] pthread_create() failed! [%d]\n", i, ret);
		printf("Thread #%d successfully created\n", i);
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
	for (i = 1; i < NTHREADS; i++) {
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("[%d] pthread_join() failed! [%d]\n", i, ret);
		else {
			printf(" Thread #%d successfully joined; it "
			       "terminated with status=%ld\n", i, (long)stat);
			if (stat == PTHREAD_CANCELED)
				printf("  : was CANCELED\n");
		}
		free(ToThread[i]);
	}
	ret = pthread_join(tid[0], (void **)&stat); // the dedicated signal handler thread
	if (ret)
		WARN("[0] pthread_join() failed! [%d]\n", ret);

	/* all worker threads have terminated, have main() call it a day! */
	pthread_exit(NULL);
}

/* vi: ts=8 */
