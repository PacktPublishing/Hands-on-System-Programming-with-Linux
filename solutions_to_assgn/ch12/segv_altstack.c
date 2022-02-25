/*
 * Assignment Solution
 * solutions_to_assgn/ch12/segv_altstack.c
 * ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 ****************************************************************

Q1. 'segv_altstack' : As seen in the previous chapter, the ch11/altstack.c
program can suffer a segmentation fault when it's alternate signal stack
is overflowed (by too many signals). 
Write a SIGSEGV handler in the sa_sigaction prototype style to properly
handle the case. It should display the signal origin (si_code) value and
meaning as well as the faulting instruction or memory reference (address)
that caused the segfault to occur.

 * (Useful to use the shell script 'bombard_sig.sh' to literally bombard
 * the process with (the same) signal repeatedly).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/file.h>
#include "../../common.h"

#define APPNAME "segv_altstack"

typedef unsigned int u32;
typedef long unsigned int u64;

#define	MAX	9
#if __x86_64__   /* 64-bit; __x86_64__ works for gcc */
 #define ADDR_TYPE u64
 #define ADDR_FMT "%016lx"
#else
 #define ADDR_TYPE u32
 #define ADDR_FMT "%08lx"
#endif

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
		DELAY_LOOP_SILENT(5); /* +48 to get the equivalent ASCII value */
		break;
	case SIGUSR2:
		fprintf(stderr, "*** signal %d:: stack@: t0=%lx last=%lx : delta=%ld ***\n",
			signum, stk_start, stk, (stk_start-stk));
		break;
	}
}

static void fatal_sig_handler(int signum, siginfo_t *si, void *ucontext)
{
	switch (signum) {
	case SIGSEGV: 
		fprintf(stderr,
			"%s:\n------------------- FATAL signal ---------------------------\n"
			" signal %d :: stack@: t0=%lx last=%lx : delta=%ld\n"
			" total signals processed=%d\n",
			APPNAME, signum, stk_start, stk, (stk_start-stk), t);
		fprintf(stderr, " Faulting insn/memory address: %p\n", si->si_addr);
		fprintf(stderr, " Origin: ");
		switch (si->si_code) {
		case SEGV_MAPERR:
			fprintf(stderr, "SEGV_MAPERR : Address not mapped to object.\n");
			break;
		case SEGV_ACCERR:
			fprintf(stderr, "SEGV_ACCERR : Invalid permissions for mapped object.\n");
			break;
#if 0
		case SEGV_BNDERR: // 3.19 onwards
			fprintf(stderr, " SEGV_BNDERR : Failed address bound checks..\n");
			break;
		case SEGV_PKUERR: // 4.6 onwards
			fprintf(stderr, " SEGV_PKUERR : Access was denied by memory protection keys.\n");
			break;
#endif
		}
		fprintf(stderr,
			"------------------------------------------------------------\n");
		/* Have the kernel generate the core dump:
		 *  Reset the SIGSEGV to glibc default, and,
		 *  Re-raise it!
		 */
		if (signal(SIGSEGV, SIG_DFL) == SIG_ERR)
			FATAL("signal -reverting SIGSEGV to default- failed");
		if (raise(SIGSEGV))
			FATAL("raise SIGSEGV failed");
		break;
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

static void signal_init(char *str_size)
{
	size_t altstacksz=0;
	struct sigaction act;

	altstacksz = atoi(str_size)*1024;
	setup_altsigstack(altstacksz);

	memset(&act, 0, sizeof(act));
	act.sa_handler = sighdlr;
	act.sa_flags = SA_NODEFER | SA_ONSTACK | SA_RESTART;
#if 0
	/* If we compile this in, it works! -as SIGUSR1 will be masked
	 * while being processed; hence, we do not overflow the sigaltstack
	 * in this case...
	 */
	sigfillset(&act.sa_mask); /* disallow all signals while handling */
#endif
	if (sigaction(SIGUSR1, &act, 0) == -1)
		FATAL("sigaction SIGUSR1 failed\n");

	/* Trap SIGUSR2 : use the handler to print signal stack space used */
	memset(&act, 0, sizeof(act));
	act.sa_handler = sighdlr;
	act.sa_flags = SA_RESTART;
	sigfillset(&act.sa_mask); /* disallow all signals while handling */
	if (sigaction(SIGUSR2, &act, 0) == -1)
		FATAL("sigaction SIGUSR2 failed\n");

	/* Trap SIGSEGV : in case we overflow the stack and crash!
	 * This time, we use the SA_SIGINFO flag so that we get more information
	 * via the siginfo_t pointer!
	 */
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = fatal_sig_handler;
	act.sa_flags = SA_RESTART | SA_ONSTACK | SA_SIGINFO;
	sigfillset(&act.sa_mask); /* disallow all signals while handling */
	if (sigaction(SIGSEGV, &act, 0) == -1)
		FATAL("sigaction SIGSEGV failed\n");
}

static void bug1(void)
{
	ADDR_TYPE *ptr = &bug1;
	printf("%s:%s: bug1=%p\n"
		"Now attempting to write to text (&bug1) ...\n",
			__FILE__, __func__, bug1);
	*(ptr) = 0x2A;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s {alternate-signal-stack-size-in-KB}\n", 
				argv[0]);
		exit(EXIT_FAILURE);
	}
	signal_init(argv[1]);

	//bug1();
	printf("Process awaiting signals ...\n");
	while (1)
		(void)pause();
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
