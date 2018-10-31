/*
 * ch8/set_pcap.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 8 : Process Capabilities
 ****************************************************************
 * Brief Description:
 * This program briefly demonstrates how a process can add or drop
 * capabilities (that are of course within it's permitted capset).
 * Run with option '1', it adds the CAP_SETUID capability and
 * "proves" it via a simple test function (test_setuid()). 
 * The other interesting bit: since the _file_ already has two
 * capabilities embedded within it (we do a setcap(8) in the Makefile),
 * we actually need to drop the CAP_SYS_ADMIN capability (from the
 * effective set); the code is shown below.
 *
 * With option '2', we want two capabilities- CAP_SETUID and
 * CAP_SYS_ADMIN; it will work as these are embedded into the
 * effective and permitted capsets.
 *
 * For details, refer to Ch 8.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/capability.h>
#include <signal.h>
#include "../common.h"

static void boing(int sig)
{
	printf("*(boing!)*\n");
}

static void drop_caps_be_normal(void)
{
	cap_t none;

	/* cap_init() guarantees all caps are cleared */
	if ((none = cap_init()) == NULL)
		FATAL("cap_init() failed, aborting...\n");
	if (cap_set_proc(none) == -1) {
		cap_free(none);
		FATAL("cap_set_proc('none') failed, aborting...\n");
	}
	cap_free(none);

	/* Become your normal true self again! */
	if (setuid(getuid()) < 0)
		FATAL("setuid to lower privileges failed, aborting..\n");
}

static void test_setuid(void)
{
	printf("%s:\nRUID = %d EUID = %d\n", __FUNCTION__, getuid(), geteuid());
	if (seteuid(0) == -1)
		WARN("seteuid(0) failed...\n");
	printf("RUID = %d EUID = %d\n", getuid(), geteuid());
}

static void usage(char **argv, int stat)
{
	fprintf(stderr, "Usage: %s 1|2\n"
		" 1 : add just one capability - CAP_SETUID\n"
		" 2 : add two capabilities - CAP_SETUID and CAP_SYS_ADMIN\n"
		"Tip: run it in the background so that capsets can be looked up\n",
		argv[0]);
	exit(stat);
}

int main(int argc, char **argv)
{
	int opt, ncap;
	cap_t mycaps;
	cap_value_t caps2set[2];

	if (argc < 2)
		usage(argv, EXIT_FAILURE);

	opt = atoi(argv[1]);
	if (opt != 1 && opt != 2)
		usage(argv, EXIT_FAILURE);

	/* Simple signal handling for the pause... */
	if (signal(SIGINT, boing) == SIG_ERR)
		FATAL("signal() SIGINT failed, aborting...\n");
	if (signal(SIGTERM, boing) == SIG_ERR)
		FATAL("signal() SIGTERM failed, aborting...\n");

	if (!CAP_IS_SUPPORTED(CAP_SETUID))
		FATAL
		    ("CAP_SETUID capability not supported on system, aborting...\n");
	if (opt == 2) {
		if (!CAP_IS_SUPPORTED(CAP_SYS_ADMIN))
			FATAL
			    ("CAP_SYS_ADMIN capability not supported on system, aborting...\n");
	}
	//--- Set the required capabilities in the Thread Eff capset
	mycaps = cap_get_proc();
	if (!mycaps)
		FATAL("cap_get_proc() for CAP_SETUID failed, aborting...\n");

	if (opt == 1) {
		ncap = 1;
		caps2set[0] = CAP_SETUID;
	} else if (opt == 2) {
		ncap = 2;
		caps2set[0] = CAP_SETUID;
		caps2set[1] = CAP_SYS_ADMIN;
	}
	if (cap_set_flag(mycaps, CAP_EFFECTIVE, ncap, caps2set, CAP_SET) == -1) {
		cap_free(mycaps);
		FATAL("cap_set_flag() failed, aborting...\n");
	}

	/* For option 1, we need to explicitly CLEAR the CAP_SYS_ADMIN capability;
	 * this is because, if we don't, it's still there as it's a file capability
	 * embedded into the binary, thus becoming part of the process Eff+Prm
	 * capsets. Once cleared, it only shows up only in the Prm and not in
	 * the Eff capset!
	 */
	if (opt == 1) {
		caps2set[0] = CAP_SYS_ADMIN;
		if (cap_set_flag(mycaps, CAP_EFFECTIVE, 1, caps2set, CAP_CLEAR)
		    == -1) {
			cap_free(mycaps);
			FATAL
			    ("cap_set_flag(clear CAP_SYS_ADMIN) failed, aborting...\n");
		}
	}

	/* Have the caps take effect on the process.
	 * Without sudo(8) or file capabilities, it fails - as expected.
	 * But, we have set the file caps to CAP_SETUID (in the Makefile),
	 * thus the process gets that capability in it's effective and
	 * permitted capsets (as we do a '+ep'; see below):
	 *  sudo setcap cap_setuid,cap_sys_admin+ep ./set_pcap
	 */
	if (cap_set_proc(mycaps) == -1) {
		cap_free(mycaps);
		FATAL
		    ("cap_set_proc(CAP_SETUID/CAP_SYS_ADMIN) failed, aborting...\n",
		     (opt == 1 ? "CAP_SETUID" : "CAP_SETUID,CAP_SYS_ADMIN"));
	}
	if (opt == 1)
		printf("PID %6d now has CAP_SETUID capability.\n", getpid());
	else if (opt == 2)
		printf("PID %6d now has CAP_SETUID,CAP_SYS_ADMIN capability.\n",
		       getpid());

	printf("Pausing #1 ...\n");
	pause();
	test_setuid();
	cap_free(mycaps);

	printf
	    ("Now dropping all capabilities and reverting to original self...\n");
	drop_caps_be_normal();
	test_setuid();

	printf("Pausing #2 ...\n");
	pause();
	printf(".. done, exiting.\n");
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
