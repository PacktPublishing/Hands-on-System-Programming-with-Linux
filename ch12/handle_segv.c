/*
 * ch12/handle_segv.c
 * ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 12 : Signaling Part II
 ****************************************************************
 * Brief Description:
 *
 * Make a usermode process segfault by accessing invalid user and/or
 * kernel-space addresses..
 * This will cause the MMU to trigger an exception condition (Page Fault
 * on x86, Data Abort on ARM), which will lead to the OS's page fault
 * handler being invoked. *It* will determine the actual fault (minor
 * or major, good or bad) and, in this case, being a usermode 'bad'
 * fault, will send SIGSEGV to the process that invoked the fault - us!
 *
 * For details, please refer the book, Ch 12.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <ucontext.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include "../common.h"

/*---------------- Typedef's, constants, etc ------------------------*/
typedef unsigned int u32;
typedef long unsigned int u64;

/*---------------- Macros -------------------------------------------*/
#define APPNAME "handle_segv"

#define ADDR_FMT "%lx"
#if __x86_64__   /* 64-bit; __x86_64__ works for gcc */
 #define ADDR_TYPE u64
 static u64 invalid_uaddr = 0xdeadfaceL;
 static u64 invalid_kaddr = 0xffff0b9ffacedeadL;
#else
 #define ADDR_TYPE u32
 static u32 invalid_uaddr = 0xfacedeadL;
 static u32 invalid_kaddr = 0xdeadfaceL;
#endif

/*---------------- Functions ----------------------------------------*/

/* NOTE! This code is arch-specific: ONLY works for x86[_64] */
static inline void dump_regs(void *ucontext)
{
#define FMT "%016llx"

	ucontext_t *uctx = (ucontext_t *)ucontext;
	fprintf(stderr,
		" RAX = 0x" FMT "   RBX = 0x" FMT "   RCX = 0x" FMT "\n"
		" RDX = 0x" FMT "   RSI = 0x" FMT "   RDI = 0x" FMT "\n"
		" RBP = 0x" FMT "   R8  = 0x" FMT "   R9  = 0x" FMT "\n"

		" R10 = 0x" FMT "   R11 = 0x" FMT "   R12 = 0x" FMT "\n"
		" R13 = 0x" FMT "   R14 = 0x" FMT "   R15 = 0x" FMT "\n"
		" RSP = 0x" FMT "\n"
		"\n RIP = 0x" FMT "   EFLAGS = 0x" FMT "\n"

		" TRAP#  = %02lld                ERROR  = %02lld\n"
		/* CR[0,1,3,4] unavailable */
		" CR2 = 0x" FMT "\n"

		, uctx->uc_mcontext.gregs[REG_RAX]
		, uctx->uc_mcontext.gregs[REG_RBX]
		, uctx->uc_mcontext.gregs[REG_RCX]

		, uctx->uc_mcontext.gregs[REG_RDX]
		, uctx->uc_mcontext.gregs[REG_RSI]
		, uctx->uc_mcontext.gregs[REG_RDI]

		, uctx->uc_mcontext.gregs[REG_RBP]
		, uctx->uc_mcontext.gregs[REG_R8]
		, uctx->uc_mcontext.gregs[REG_R9]

		, uctx->uc_mcontext.gregs[REG_R10]
		, uctx->uc_mcontext.gregs[REG_R11]
		, uctx->uc_mcontext.gregs[REG_R12]

		, uctx->uc_mcontext.gregs[REG_R13]
		, uctx->uc_mcontext.gregs[REG_R14]
		, uctx->uc_mcontext.gregs[REG_R15]

		, uctx->uc_mcontext.gregs[REG_RSP]
		, uctx->uc_mcontext.gregs[REG_RIP]
		, uctx->uc_mcontext.gregs[REG_EFL]
		, uctx->uc_mcontext.gregs[REG_TRAPNO]
		, uctx->uc_mcontext.gregs[REG_ERR]
		, uctx->uc_mcontext.gregs[REG_CR2]
		);
}

static void myfault(int signum, siginfo_t * si, void *ucontext)
{
	fprintf(stderr,
		"%s:\n------------------- FATAL signal ---------------------------\n",
		APPNAME);
	fprintf(stderr," %s: received signal %d. errno=%d\n"
	       " Cause/Origin: (si_code=%d): ",
	       __func__, signum, si->si_errno, si->si_code);

	switch (si->si_code) {
	/* Possible values si_code can have for SIGSEGV */
	case SEGV_MAPERR:
		fprintf(stderr,"SEGV_MAPERR: address not mapped to object\n");
		break;
	case SEGV_ACCERR:
		fprintf(stderr,"SEGV_ACCERR: invalid permissions for mapped object\n");
		break;
	/* SEGV_BNDERR and SEGV_PKUERR result in compile failure? */
#if 0
	case SEGV_BNDERR: /* 3.19 onward */
		fprintf(stderr,"SEGV_BNDERR: failed address bound checks\n");
	case SEGV_PKUERR: /* 4.6 onward */
		fprintf(stderr,"SEGV_PKUERR: access denied by memory-protection keys\n");
#endif
	/* Other possibilities for si_code; here just to show them... */
	case SI_USER:
		fprintf(stderr,"user\n");
		break;
	case SI_KERNEL:
		fprintf(stderr,"kernel\n");
		break;
	case SI_QUEUE:
		fprintf(stderr,"queue\n");
		break;
	case SI_TIMER:
		fprintf(stderr,"timer\n");
		break;
	case SI_MESGQ:
		fprintf(stderr,"mesgq\n");
		break;
	case SI_ASYNCIO:
		fprintf(stderr,"async io\n");
		break;
	case SI_SIGIO:
		fprintf(stderr,"sigio\n");
		break;
	case SI_TKILL:
		fprintf(stderr,"t[g]kill\n");
		break;
	default:
		fprintf(stderr,"-none-\n");
	}

	fprintf(stderr," Faulting instr or address = 0x" ADDR_FMT "\n",
			(ADDR_TYPE) si->si_addr);
	fprintf(stderr, " --- Register Dump [x86_64] ---\n");
	dump_regs(ucontext);
	fprintf(stderr,
		"------------------------------------------------------------\n");
	psiginfo(si, "psiginfo helper");
	fprintf(stderr,
		"------------------------------------------------------------\n");

	/* 
	 * Placeholders for real-world apps:
	 *  crashed_write_to_log();
	 *  crashed_perform_cleanup();
	 *  crashed_inform_enduser();
	 *
	 * Now have the kernel generate the core dump by:
	 *  Reset the SIGSEGV to glibc default, and,
	 *  Re-raise it!
	 */
	if (signal(SIGSEGV, SIG_DFL) == SIG_ERR)
		FATAL("signal -reverting SIGSEGV to default- failed");
	if (raise(SIGSEGV))
		FATAL("raise SIGSEGV failed");
}

static void usage(char *nm)
{
	fprintf(stderr, "Usage: %s u|k r|w\n"
		"u => user mode\n"
		"k => kernel mode\n"
		" r => read attempt\n" " w => write attempt\n", nm);
}

int main(int argc, char **argv)
{
	struct sigaction act;

	if (argc != 3) {
		usage(argv[0]);
		exit(1);
	}

	memset(&act, 0, sizeof(act));
	act.sa_sigaction = myfault;
	act.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGSEGV, &act, 0) == -1)
		FATAL("sigaction SIGSEGV failed\n");

	if ((tolower(argv[1][0]) == 'u') && tolower(argv[2][0] == 'r')) {
		ADDR_TYPE *uptr = (ADDR_TYPE *) invalid_uaddr;	// arbitrary userspace virtual addr
		printf("Attempting to read contents of arbitrary usermode va uptr = 0x" ADDR_FMT ":\n",
		     (ADDR_TYPE) uptr);
		printf("*uptr = 0x" ADDR_FMT "\n", *uptr);	// just reading
	} else if ((tolower(argv[1][0]) == 'u') && tolower(argv[2][0] == 'w')) {
		ADDR_TYPE *uptr = (ADDR_TYPE *) & main;
		printf
		    ("Attempting to write into arbitrary usermode va uptr (&main actually) = 0x" ADDR_FMT ":\n",
		     (ADDR_TYPE) uptr);
		*uptr = 0x2A;	// writing
	} else if ((tolower(argv[1][0]) == 'k') && tolower(argv[2][0] == 'r')) {
		ADDR_TYPE *kptr = (ADDR_TYPE *) invalid_kaddr; 	// arbitrary kernel virtual addr
		printf
		    ("Attempting to read contents of arbitrary kernel va kptr = 0x" ADDR_FMT ":\n",
		     (ADDR_TYPE) kptr);
		printf("*kptr = 0x" ADDR_FMT "\n", *kptr);	// just reading
	} else if ((tolower(argv[1][0]) == 'k') && tolower(argv[2][0] == 'w')) {
		ADDR_TYPE *kptr = (ADDR_TYPE *) invalid_kaddr; 	// arbitrary kernel virtual addr
		printf
		    ("Attempting to write into arbitrary kernel va kptr = 0x" ADDR_FMT ":\n",
		     (ADDR_TYPE) kptr);
		*kptr = 0x2A;	// writing
	} else
		usage(argv[0]);
	exit(0);
}

/* vi: ts=8 */
