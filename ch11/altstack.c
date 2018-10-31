/*
 * ch11/altstack.c
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
 * The code here is pretty much the same as 'defer_or_not.c' except
 * that we use an alternate signal stack (via the sigaltstack(2) sys
 * call). We pass the SA_ONSTACK flag to make use of it.. The user
 * is asked for the alt signal stack size (in KB) as a parameter.
 *
 * Also, we have added a neat feature (to the previous version: sending
 * the process SIGUSR2 will have it print out the first and most recent
 * stack pointer address; as well, it will calculate and display the
 * 'delta' - in effect, the amount of stack memory used so far by the
 * application!
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

#define	MAX	9

static volatile sig_atomic_t s=0, t=0;
static volatile unsigned long stk_start=0, stk=0;
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

/* The idea here is to have this handler running code for a while,
 * with signals unmasked - including the one we're handling. This is
 * what the SA_NODEFER flag achieves. So, we do the DELAY_LOOP_SILENT()
 * macro...
 */
static void sighdlr(int signum)
{
	if (t == 0)
		stk_start = (unsigned long)stack();

	switch (signum) {
	case SIGUSR1:
		stk = (unsigned long)stack();
		s ++; t ++;
		if (s >= MAX)
			s = 1;
		fprintf(stderr, " s=%d ; total=%d; stack %p\n", s, t, stack());
		/* Spend some time inside the signal handler ... */
		DELAY_LOOP_SILENT(5);
		break;
	case SIGUSR2:
		fprintf(stderr, "*** signal %d:: stack@: t0=%lx last=%lx : delta=%ld ***\n",
			signum, stk_start, stk, (stk_start-stk));
		break;
	case SIGSEGV:
		fprintf(stderr, "*** signal %d:: stack@: t0=%lx last=%lx : delta=%ld ***\n",
			signum, stk_start, stk, (stk_start-stk));
		abort();
	}
}

static void setup_altsigstack(size_t stack_sz)
{
	stack_t ss;

	printf("Alt signal stack size = %zu\n", stack_sz);
	ss.ss_sp = malloc(stack_sz);
	if (!ss.ss_sp)
		FATAL("malloc(%zu) for alt sig stack failed\n", stack_sz);
	ss.ss_size = stack_sz;
	ss.ss_flags = 0;
	if (sigaltstack(&ss, NULL) == -1)
		FATAL("sigaltstack for size %zu failed!\n", stack_sz);
}

int main(int argc, char **argv)
{
	size_t altstacksz=0;
	struct sigaction act;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s {alternate-signal-stack-size-in-KB}\n", 
				argv[0]);
		exit(EXIT_FAILURE);
	}

	altstacksz = atoi(argv[1])*1024;
	setup_altsigstack(altstacksz);

	memset(&act, 0, sizeof(act));
	act.sa_handler = sighdlr;
	act.sa_flags = SA_NODEFER | SA_ONSTACK | SA_RESTART;
	if (sigaction(SIGUSR1, &act, 0) == -1)
		FATAL("sigaction SIGUSR1 failed\n");

	/* Trap SIGUSR2 : use the handler to print signal stack space used */
	memset(&act, 0, sizeof(act));
	act.sa_handler = sighdlr;
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGUSR2, &act, 0) == -1)
		FATAL("sigaction SIGUSR2 failed\n");

	/* Trap SIGSEGV : in case we overflow the stack and crash!
	 * use the handler to print signal stack space used */
	memset(&act, 0, sizeof(act));
	act.sa_handler = sighdlr;
	act.sa_flags = SA_RESTART | SA_ONSTACK;
	if (sigaction(SIGSEGV, &act, 0) == -1)
		FATAL("sigaction SIGSEGV failed\n");

	printf("Running: signal SIGUSR1 flags: SA_NODEFER | SA_ONSTACK | SA_RESTART\n");
	printf("Process awaiting signals ...\n");
	while (1)
		(void)pause();
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
