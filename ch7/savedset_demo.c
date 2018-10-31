/*
 * ch7/savedset_demo.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 7 : Process Credentials
 ****************************************************************
 * Brief Description:
 * Demo switching between privileged state and 'regular' creds via
 * the saved-set.
 *
 * For details, please refer the book, Ch 7.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

#define SHOW_CREDS() do {		\
  printf("RUID=%d EUID=%d\n"		\
         "RGID=%d EGID=%d\n",		\
		getuid(), geteuid(),    \
		getgid(), getegid());   \
} while (0)

int main(int argc, char **argv)
{
	uid_t saved_setuid;

	printf("t0: Init:\n");
	SHOW_CREDS();
	if (0 != geteuid())
		FATAL("Not a setuid-root executable,"
			" aborting now ...\n"
			"[TIP: do: sudo chown root %s ;"
			" sudo chmod u+s %s\n"
			" and rerun].\n"
			, argv[0], argv[0], argv[0]);
	printf(" Ok, we're effectively running as root! (EUID==0)\n");

	/* Save the EUID, in effect the "saved set UID", so that
	 * we can switch back and forth
	 */
	saved_setuid = geteuid();

	printf("t1: Becoming my original self!\n");
	if (seteuid(getuid()) == -1)
		FATAL("seteuid() step 2 failed!\n");
	SHOW_CREDS();

	printf("t2: Switching to privileged state now...\n");
	if (seteuid(saved_setuid) == -1)
		FATAL("seteuid() step 3 failed!\n");
	SHOW_CREDS();
	if (0 == geteuid())
		printf(" Yup, we're root again!\n");
	else
		FATAL(" should be but aren't root; something wrong, aborting!\n");

	printf("t3: Switching back to unprivileged state now ...\n");
	if (seteuid(getuid()) == -1)
		FATAL("seteuid() step 4 failed!\n");
	SHOW_CREDS();

	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
