/*
 * ch11/defer_or_not.c
 * ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 11 : Signaling Part I
 ****************************************************************
 * Brief Description:
 *
 * Here we test two cases:
 * parameter=1:
 *  SA_NODEFER signal flag is *Cleared* : [default setting on Linux]
 *  During signal handling of SIGUSR1, if the same signal is continually
 *  delivered, the OS keeps *no* queue; however, one signal
 *  instance is kept pending, and will be delivered once the handler
 *  completes execution.
 * parameter=2:
 *  SA_NODEFER signal flag is *Set* : [not the default setting on Linux]
 *  During signal handling of SIGUSR1, if the same signal is continually
 *  delivered, the OS keeps *no* queue; however, because the
 *  SA_NODEFER flag is set, every signal instance will be processed immediately;
 *  IOW, the handler will be reentered. Context will be saved and later restored.
 *  This test case clearly demonstrates this.
 *
 * Useful to use the shell script 'bombard_sig.sh' to literally bombard
 * the process with (the same) signal repeatedly.
 *
 * For details, please refer the book, Ch 11.
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
static void sighdlr(int signum)
{
	int saved;

	fprintf(stderr, "\nsighdlr: signal %d,", signum);
	switch (signum) {
	case SIGUSR1:
		s ++; t ++;
		if (s >= MAX)
			s = 1;
		saved = s;
		fprintf(stderr, " s=%d ; total=%d; stack %p :", s, t, stack());
		DELAY_LOOP(saved+48, 5); /* +48 to get the equivalent ASCII value */
		fprintf(stderr, "*");
		break;
	default:;
	}
}

int main(int argc, char **argv)
{
	int flags=0;
	struct sigaction act;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s {option}\n\
option=1 : don't use (clear) SA_NODEFER flag (default sigaction style)\n\
option=2 : use (set) SA_NODEFER flag (will process signal immd)\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	flags = SA_RESTART;
	if (atoi(argv[1]) == 2) {
		flags |= SA_NODEFER;
		printf("PID %d: running with the SA_NODEFER signal flag Set\n",
				getpid());
	} else {
		printf("PID %d: running with the SA_NODEFER signal flag Cleared [default]\n",
				getpid());
	}

	memset(&act, 0, sizeof(act));
	act.sa_handler = sighdlr;
	act.sa_flags = flags;
	if (sigaction(SIGUSR1, &act, 0) == -1)
		FATAL("sigaction failed\n");

	printf("Process awaiting signals ...\n");
	while (1)
		(void)pause();
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
