/*
 * ch12/rtsigs_waiter.c
 * ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 12 : Signaling Part II
 ****************************************************************
 * Brief Description:
 * Aim: to test how realtime signals are delivered after being unblocked.
 * We have a process trap 3 RT signals of varying priorities; we then
 * arrange for the RT sigs to be *blocked* when the handler runs (by calling
 * sigfillset before the sigaction). 
 * 
 * Useful to use the shell script 'bombard_sigrt.sh' to literally bombard
 * the process with multiple realtime signals repeatedly.
 *
 * We find that the first RT sig sent is delivered immediately and processed;
 * as we deliberately have the handler running for a while, the remaining
 * RT sigs that have been sent are not lost, rather, they are *queued* for
 * delivery. Once the previous handler is done, the next RT sig in the Q is
 * delivered and processed. A key point: the delivery of RT signals from the
 * queue is in *priority order* - first the lower-numbered RT sigs to higher-
 * numbered RT sigs.
 *
 * For details, please refer the book, Ch 12.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/file.h>
#include "../common.h"

#define	MAX	10
static volatile sig_atomic_t s=0, t=0;

/* 
 * stack(): return the current value of the stack pointer register.
 * The trick/hack: on x86 CPU's, the ABI tells us that the return
 * value is always in the accumulator (EAX/RAX); so we just initialize
 * it to the stack pointer (using inline assembly)!
 */
void *stack(void)
{
	if (__WORDSIZE == 32) {
		__asm__("movl %esp, %eax");
	} else if (__WORDSIZE == 64) {
		__asm__("movq %rsp, %rax");
	}
/* Accumulator holds the return value */
}

/* 
 * Strictly speaking, should not use fprintf here as it's not
 * async-signal safe; indeed, it sometimes does not work well!
 */
#undef SHOW_MASKED
/*#define SHOW_MASKED*/
static void rt_sighdlr(int signum)
{
#ifdef SHOW_MASKED
	if (show_blocked_signals() < 0)
		WARN("sigprocmask -query- failed\n");

#endif
	fprintf(stderr, "\nsighdlr: signal %d,", signum);
	if ((signum == SIGRTMAX-5) ||
	    (signum == SIGRTMIN+5) ||
	    (signum == SIGRTMAX)) {
		s ++; t ++;
		if (s >= MAX)
			s = 1;
		fprintf(stderr, " s=%d ; total=%d; stack %p :", s, t, stack());
		//DELAY_LOOP('o', 8);
		DELAY_LOOP_SILENT(8);
		fprintf(stderr, "*");
	}
}

int main(void)
{
	struct sigaction act;

	printf("Trapping the three realtime signals\n");
	memset(&act, 0, sizeof(act));
	act.sa_handler = rt_sighdlr;
	act.sa_flags = SA_RESTART;
	/* Set sigmask to all 1's, thus masking all signals
	   while this handler runs */
	sigfillset(&act.sa_mask);

	if (sigaction(SIGRTMAX-5, &act, 0) == -1)
		FATAL("sigaction %s failed\n", "SIGRTMAX-5");
	if (sigaction(SIGRTMIN+5, &act, 0) == -1)
		FATAL("sigaction %s failed\n", "SIGRTMIN+5");
	if (sigaction(SIGRTMAX, &act, 0) == -1)
		FATAL("sigaction %s failed\n", "SIGRTMAX");

	printf("Process awaiting signals ...\n");
	while (1)
		(void)pause();
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
