/*
 * Assignment Solution
 * solutions_to_assgn/ch10/pdf_rdr.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 9 : Process Execution
 ****************************************************************
Q1. 'pdf_rdr':

Enhance the ch9/pdfrdr_exec.c program (from the previous chapter, Process
Execution) - the one that execs a PDF reader app such that the given PDF
document is displayed. Earlier, the predecessor was lost to the successor; now,
rewrite the program with proper fork-exec-wait semantics such that the parent app
retains control. Also, programatically verify whether the PDF reader succeeded
or not (and if not, display the error exit status it returns).

 * For details, please refer the book, Ch 10.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../../common.h"

/* NOTE! We assume that the PDF reader app 'evince' is
 * installed at this location
 */
const char *pdf_reader_app="/usr/bin/evince";
static int gVerbose = 1;

/* Below function code from ch10/simpshv2.c ; slightly modified */
static void interpret_wait(pid_t child, int wstatus)
{
	int exstat=0;

	VPRINT("Child (PID %7d) status changed:\n", child);
	/* Here, we show the exit status that the evince app
	 * terminated with.
	 */
	if (WIFEXITED(wstatus)) {
		exstat = WEXITSTATUS(wstatus);
		VPRINT(" normal termination: exit status: %d\n", exstat);
		if (exstat == 0)
			VPRINT("  child exited with success!\n");
		else
			VPRINT("  child failed; failed exit status = %d\n", exstat);
	}
	if (WIFSIGNALED(wstatus)) {
		VPRINT(" abnormal termination: killer signal: %d",
				WTERMSIG(wstatus));
		if (WCOREDUMP(wstatus))
			VPRINT(" : core dumped\n");
		else
			VPRINT("\n");
	}
	if (WIFSTOPPED(wstatus))
		VPRINT(" stopped: stop signal: %d\n",
				WSTOPSIG(wstatus));
	if (WIFCONTINUED(wstatus))
		VPRINT(" (was stopped), resumed (SIGCONT)\n");
}

static int exec_pdf_reader_app(char *pdfdoc)
{
	char * const pdf_argv[] = {"evince", pdfdoc, 0};

	if (execv(pdf_reader_app, pdf_argv) < 0) {
		WARN("execv failed\n");
		return -1;
	}
	return 0; /* never reached */
}

int main(int argc, char **argv)
{
	int wstat=0;
	pid_t cpid;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s {pathname_of_doc.pdf}\n"
			, argv[0]);
		exit(EXIT_FAILURE);
	}
	
	/* Lets use the 'fork-exec-wait' semantic! */
	switch (fork()) {
	case -1 : FATAL("fork failed\n");
		  exit(EXIT_FAILURE); // not reqd, to surpress gcc warning
	case 0 : /* the child process */
		printf("Child %d: now exec-ing '%s %s'\n",
				getpid(), pdf_reader_app, argv[1]);
		if (exec_pdf_reader_app(argv[1]) < 0)
			FATAL("exec pdf function failed\n");
	default : /* the parent process */
		printf("Parent %d: waiting to clear the zombie...\n",
				getpid());
		/* sync: child runs first, parent waits
		 * for child's death */
		if ((cpid = wait(&wstat)) < 0)
			FATAL("wait(2) failed, aborting..\n");
		interpret_wait(cpid, wstat);
	}
	
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
