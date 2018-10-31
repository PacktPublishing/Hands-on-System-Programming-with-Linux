/*
 * ch13/runwalk_timer.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 13 : Timers
 ****************************************************************
 * Brief Description:
 *
 * A simple "run-walk" timer.
 * The idea: running continously is hard, esp for beginners. Often, coaches
 * have the newbie runner follow a useful "run-walk" strategy; run for some
 * given amount of time, then take a walk break for a given time period, then
 * repeat - run again, walk again, indefinitely, or until your target
 * distance/time goal is met!
 *
 * This simple program will allow you to setup the number of seconds to run,
 * and to walk. It will timeout accordingly.. 
 * (Many runner-GPS watches (like Garmin, TomTom, etc) support this kind of
 * functionality).
 *
 * Here, we use a simple 'one-shot' POSIX timer to do the job. We set the timer
 * to use signal notification as the timer expiry notification mechanism, we
 * setup a signal handler for a RT signal. Next, we initially set the POSIX
 * timer to expire after the 'run period', then, in the signal handler, we
 * _reset_ (re-arm) the timer to expire after the 'walk period' seconds. This
 * essentially repeats forever, or until the user aborts the program (!).
 *
 * Note: 
 * - Tip: set the 'verbose' level HIGH to see what's going on in detail (it's
 *   set to level 'LOW' by default).
 * - links into the RT library.
 *
 * For details, please refer the book, Ch 13.
 */
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/time.h>
#include "../common.h"

/*---------------- Macros -------------------------------------------*/
#define VER	"1.0"
#define PRG_BANNER "************* Run Walk Timer *************\n"
#define WALK_MIN_GRANULARITY 1	
	 // minute, but used as seconds interval in which to display time rem..

/*---------------- Typedef's, constants, etc ------------------------*/
enum verbosity_level {
	OFF = 0,
	LOW,
	HIGH
};
static volatile sig_atomic_t verbose = LOW;

enum type {
	RUN = 0,
	WALK
};

// Our app context data structure
typedef struct {
	int trun, twalk;
	int type;
	struct itimerspec *itmrspec;
	timer_t timerid;
} sRunWalk;

/*------------------- Functions -------------------------------------*/

static inline void BUZZ(char *msg)
{
	fprintf(stderr, "\n*** Bzzzz!!! %s ***", msg);
	/* Those developing a smartwatch/running watch: add code here to
	 * vibrate it, make a sound, etc! */
}

static void its_time(int signum, siginfo_t *si, void *uctx)
{
	// Gain access to our app context
	volatile sRunWalk *ps = (sRunWalk *)si->si_value.sival_ptr;

	assert(ps);
	if (verbose == HIGH)
		printf("%s: signal %d. runwalk ptr: %p"
		       " Type: %s. Overrun: %d\n",
		       __func__, signum,
		       ps,
		       ps->type == WALK ? "Walk" : "Run", timer_getoverrun(ps->timerid)
		    );

	memset(ps->itmrspec, 0, sizeof(sRunWalk));
	if (ps->type == WALK) {
		BUZZ("  RUN!");
		ps->itmrspec->it_value.tv_sec = ps->trun;
		printf("       for %4d  seconds\n", ps->trun);
	}
	else {
		BUZZ(" WALK!");
		ps->itmrspec->it_value.tv_sec = ps->twalk;
		printf("       for %4d  seconds\n", ps->twalk);
	}
	ps->type = !ps->type; // toggle the type

	// Reset: re-arm the one-shot timer
	if (timer_settime(ps->timerid, 0, ps->itmrspec, NULL) < 0)
		FATAL("timer_settime failed");
}

/*------------------ Our POSIX Timer setup
 * Setup a 'one-shot' POSIX Timer; initially set it to expire upon
 * 'run time' seconds elapsing.
 */
static void runwalk_timer_init_and_arm(sRunWalk * ps)
{
	struct sigaction act;
	struct sigevent runwalk_evp;

	assert(ps);

	act.sa_sigaction = its_time;
	act.sa_flags = SA_SIGINFO;
	sigfillset(&act.sa_mask);
	if (sigaction(SIGRTMIN, &act, 0) < 0)
		FATAL("sigaction: SIGRTMIN");

	memset(ps->itmrspec, 0, sizeof(sRunWalk));
	ps->type = RUN;
	ps->itmrspec->it_value.tv_sec = ps->trun;

	runwalk_evp.sigev_notify = SIGEV_SIGNAL;
	runwalk_evp.sigev_signo = SIGRTMIN;
	// Pass along the app context structure pointer
	runwalk_evp.sigev_value.sival_ptr = ps;

	// Create the runwalk 'one-shot' timer
	if (timer_create(CLOCK_REALTIME, &runwalk_evp, &ps->timerid) < 0)
		FATAL("timer_create");

	// Arm timer; will exire in ps->trun seconds, triggering the RT signal
	if (timer_settime(ps->timerid, 0, ps->itmrspec, NULL) < 0)
		FATAL("timer_settime failed");
}

static void bye(int signum)
{
	char *saybye = "\n+++ Good job, bye! +++\n";
	write(STDOUT_FILENO, saybye, strlen(saybye));
	exit(EXIT_SUCCESS);
}

static inline void help_and_die(char **argv)
{
	fprintf(stderr,
		"Usage: %s Run-for[sec] Walk-for[sec] [verbosity-level=0|[1]|2]\n"
		" Verbosity Level ::  0 = OFF   [1 = LOW]   2 = HIGH\n",
		argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct sigaction act;
	sRunWalk runwalk;
	struct itimerspec runwalk_curval;

	if (argc < 3)
		help_and_die(argv);
	if (argc == 4) {
		verbose = atoi(argv[3]);
		if (verbose < 0 || verbose > HIGH)
			help_and_die(argv);
	}

	// Trap the SIGINT (^C)
	memset(&act, 0, sizeof(act));
	act.sa_handler = bye;
	if (sigaction(SIGINT, &act, 0) < 0)
		FATAL("sigaction: SIGINT");

	memset(&runwalk, 0, sizeof(sRunWalk));

	errno = 0;
	runwalk.trun = strtoul(argv[1], 0, 0);
	if (errno == ERANGE || errno == EINVAL)
		FATAL("Invalid input [strtoul]\n");

	errno = 0;
	runwalk.twalk = strtoul(argv[2], 0, 0);
	if (errno == ERANGE || errno == EINVAL)
		FATAL("Invalid input [strtoul]\n");

	// Negative numbers are converted to their abs by strtoul
	if (runwalk.trun <= 0 || runwalk.twalk <= 0)
		help_and_die(argv);

	printf("%s                  Ver %s\n", PRG_BANNER, VER);

	runwalk.itmrspec = calloc(1, sizeof(struct itimerspec));
	if (!runwalk.itmrspec)
		FATAL("calloc failed");

	printf("\nGet moving... Run for %d seconds\n", runwalk.trun);
	if (verbose == HIGH)
		printf("trun=%4d twalk=%4d; app ctx ptr = %p\n",
		       runwalk.trun, runwalk.twalk, &runwalk);

	runwalk_timer_init_and_arm(&runwalk);

	while (1) {
		if (verbose == HIGH) {
			if (timer_gettime(runwalk.timerid, &runwalk_curval) < 0)
				FATAL("timer_gettime failed");

			// show in s.ms (not nanosec)
			printf("runwalk: %4ld.%lds\n",
			       runwalk_curval.it_value.tv_sec,
			       runwalk_curval.it_value.tv_nsec / 1000000);
		} else if (verbose == LOW) {
			printf(".");
			fflush(stdout);
		}

		if (verbose != OFF)
			/* Ha! here, we really *want* the sleep to be 'disturbed'
			 * by the RT signal; so just use the normal sleep(3), not our
			 * safe version r_sleep.
			 */
			sleep(WALK_MIN_GRANULARITY);
			//r_sleep(WALK_MIN_GRANULARITY, 0);
		else
			(void)pause();
	}
	// control should never reach here

	(void)pause();
	free(runwalk.itmrspec);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
