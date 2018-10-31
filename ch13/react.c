/*
 * ch13/react.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 13 : Timers
 ****************************************************************
 * Brief Description:
 * The 'Help' screen:
  
"Usage: ./react <freq-in-millisec> [verbose-mode:[0]|1]
  default: verbosity is off
  f.e.: ./react 100   => timeout every 100 ms, verbosity Off
      : ./react   5 1 => timeout every   5 ms, verbosity On

How fast can you react!?
Once you run this app with the freq-in-millisec parameter,
we shall start a timer anywhere between 1 and 5 seconds of
your starting it. Watch the screen carefully; the moment
the message "QUICK! Press ^C" appears, press ^C (Ctrl+c simultaneously)!
Your reaction time is displayed... Have fun!"
 
* The program sets up a (repeating) POSIX interval timer for the number of
* milliseconds the user specifies as the first parameter. We setup signal
* handlers for the following three signals:
* a) SIGRTMIN : RT signal sent when the POSIX interval timer expires
* b) SIGALRM  : delivered when our initial random start time expires ("the
*		element of surprise" :-) )
* c) SIGINT   : delivered when the user "reacts", by pressing ^C.
*
* In the SIGINT handler, we show the 'count' - the number of SIGRTMIN's so far
* delivered, and can thus calculate the approximate "reaction time" in ms.
*
* For details, please refer the book, Ch 13.
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <math.h>
#include "../common.h"

/*---------------- Globals, Macros ----------------------------*/
static int gVerbose = 0;
static long freq_ms = 0;
static struct itimerspec itv;
static timer_t timerid;
static volatile sig_atomic_t gTimerRepeats = 0, c = 0, first_time = 1,
    jumped_the_gun = 1;
static struct timespec tm_start, tm_end;

#define MIN_START_SEC	1
#define MAX_START_SEC	5

/*---------------- Functions ----------------------------------*/

/* 
 * The realtime signal (SIGRTMIN) - timer expiry - handler.
 * WARNING! Using the printf(3) in a signal handler is unsafe!
 * We do so for the purposes of this demo app only; do Not
 * use in production.
 */
static void timer_handler(int sig, siginfo_t * si, void *uc)
{
	char buf[] = ".";

	c++;
	if (gVerbose) {
		write(2, buf, 1);
#define SHOW_OVERRUN 1
#if (SHOW_OVERRUN == 1)
		{
			int ovrun = timer_getoverrun(timerid);
			if (ovrun == -1)
				WARN("timer_getoverrun");
			else {
				if (ovrun)
					printf("    overrun=%d [@count=%d]\n",
					       ovrun, c);
			}
		}
#endif
	}
}

/*
 * The signal handler for SIGINT; arrival here implies the user has
 * "reacted" - has pressed ^C.
 *
 * WARNING! Using the [f]printf(3) in a signal handler is unsafe!
 * We do so for the purposes of this demo app only; do not
 * use in production.
 */
static void userpress(int sig)
{
	struct timespec res;

	//--- timestamp it: end time
	if (clock_gettime(CLOCK_REALTIME, &tm_end) < 0)
		FATAL("clock_gettime (tm_end) failed\n");

	if (jumped_the_gun) {
		printf("\n:-/ Whoops, you jumped the gun! Unfair, aborting.\n");
		exit(EXIT_FAILURE);
	}

	if (first_time) {
		if (c == 0)
			printf
			    ("\n***** WOW! ***** Perfect reaction, well done.\n");
		else {
			printf("\n*** PRESSED ***\n");
			/* Calculate the delta; subtracting one struct timespec
			 * from another takes a little work. A retrofit ver of
			 * the 'timerspecsub' macro has been incorporated into
			 * our ../common.h header to do this.
			 */
			timerspecsub(&tm_end, &tm_start, &res);
			printf
			    (" Your reaction time is precisely %ld.%ld s.ns"
			     "  [~= %3.0f ms, count=%d]\n",
			     res.tv_sec, res.tv_nsec,
			     res.tv_sec * 1000 +
			     round((double)res.tv_nsec / 1000000), c);
		}
		first_time = 0;
	} else
		printf("\n ^C pressed [count=%d]\n", c);

	c = 0;

	if (!gTimerRepeats)
		exit(EXIT_SUCCESS);
}

static void arm_timer(timer_t tmrid, struct itimerspec *itmspec)
{
	VPRINT("Arming timer now\n");
	if (timer_settime(tmrid, 0, itmspec, NULL) == -1)
		FATAL("timer_settime failed\n");
	jumped_the_gun = 0;
}

/*
 * startoff
 * The signal handler for SIGALRM; arrival here implies the app has
 * "started" - we shall arm the interval timer here, it will start
 * running immediately. Take a timestamp now.
 */
static void startoff(int sig)
{
	char press_msg[] = "\n*** QUICK! Press ^C !!! ***\n";

	arm_timer(timerid, &itv);
	write(STDERR_FILENO, press_msg, strlen(press_msg));

	//--- timestamp it: start time
	if (clock_gettime(CLOCK_REALTIME, &tm_start) < 0)
		FATAL("clock_gettime (tm_start) failed\n");
}

static void show_tmval(struct itimerspec itv)
{
	printf("timer struct ::\n"
	       " it_value.tv_sec = %ld it_value.tv_nsec = %ld\n"
	       " it_interval.tv_sec = %ld it_interval.tv_nsec = %ld\n",
	       itv.it_value.tv_sec, itv.it_value.tv_nsec,
	       itv.it_interval.tv_sec, itv.it_interval.tv_nsec);
}

static int init(void)
{
	struct sigevent sev;
	struct rlimit rlim;
	struct sigaction act;

	// Trap SIGRTMIN : delivered on (interval) timer expiry
	memset(&act, 0, sizeof(act));
	act.sa_flags = SA_SIGINFO | SA_RESTART;
	act.sa_sigaction = timer_handler;
	if (sigaction(SIGRTMIN, &act, NULL) == -1)
		FATAL("sigaction SIGRTMIN failed\n");

	// Trap SIGINT : delivered on user pressing ^C
	memset(&act, 0, sizeof(act));
	act.sa_handler = userpress;
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &act, NULL) == -1)
		FATAL("sigaction SIGINT failed\n");

	// Trap SIGALRM : delivered on initial random alarm expiring
	memset(&act, 0, sizeof(act));
	act.sa_handler = startoff;
	act.sa_flags = SA_RESTART;
	if (sigaction(SIGALRM, &act, NULL) == -1)
		FATAL("sigaction SIGALRM failed\n");

	/* Create and init the timer */
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;
	sev.sigev_value.sival_ptr = &timerid;
	if (timer_create(CLOCK_REALTIME, &sev, &timerid) == -1)
		FATAL("timer_create failed\n");

	printf("Initializing timer to generate SIGRTMIN every %ld ms\n",
	       freq_ms);
	memset(&itv, 0, sizeof(struct itimerspec));
	itv.it_value.tv_sec = (freq_ms * 1000000) / 1000000000;
	itv.it_value.tv_nsec = (freq_ms * 1000000) % 1000000000;
	itv.it_interval.tv_sec = (freq_ms * 1000000) / 1000000000;
	itv.it_interval.tv_nsec = (freq_ms * 1000000) % 1000000000;

	if (gVerbose) {
		show_tmval(itv);
		if (show_blocked_signals() < 0)
			WARN("sigprocmask -query- failed\n");
	}

	/* Turn off core dump via resource limit;
	 * else, pressing ^\ (SIGQUIT) to quit will result in an
	 * unecessary core dump image file being created.
	 */
	memset(&rlim, 0, sizeof(rlim));
	if (prlimit(0, RLIMIT_CORE, &rlim, NULL) == -1)
		WARN("prlimit:core to zero failed\n");
	return 0;
}

/* random_start
 * The element of surprise: fire off an 'alarm' - resulting in SIGALRM being
 * delivered to us - in a random number between [min..max] seconds.
 */
static void random_start(int min, int max)
{
	unsigned int nr;

	alarm(0);
	srandom(time(0));
	nr = (random() % max) + min;

#define CHEAT_MODE	0
#if (CHEAT_MODE == 1)
	printf("Ok Cheater :-) get ready; press ^C in %ds ...\n", nr);
#endif

	alarm(nr);
}

static void help(char *name)
{
	fprintf(stderr,
		"Usage: %s <freq-in-millisec> [verbose-mode:[0]|1]\n"
		"  default: verbosity is off\n"
		"  f.e.: %s 100   => timeout every 100 ms, verbosity Off\n"
		"      : %s   5 1 => timeout every   5 ms, verbosity On\n"
		"\nHow fast can you react!?\n"
		"Once you run this app with the freq-in-millisec parameter,\n"
		"we shall start a timer anywhere between %d and %d seconds of\n"
		"your starting it. Watch the screen carefully; the moment\n"
		"the message \"QUICK! Press ^C\" appears, press ^C (Ctrl+c simultaneously)!\n"
		"Your reaction time is displayed... Have fun!\n",
		name, name, name, MIN_START_SEC, MAX_START_SEC);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		help(argv[0]);
		exit(EXIT_FAILURE);
	}
	if (argc == 3 && atoi(argv[2]) == 1)
		gVerbose = 1;
	freq_ms = atoll(argv[1]);
	if (freq_ms < 1) {
		help(argv[0]);
		exit(EXIT_FAILURE);
	}

	init();
	printf("[Verbose: %s]\n"
	       "We shall start a timer anytime between %d and %d seconds from now...\n",
	       gVerbose == 1 ? "Y" : "N", MIN_START_SEC, MAX_START_SEC);
	printf("\nGET READY ...\n"
	       " [ when the \"QUICK! Press ^C\" message appears, press ^C quickly as you can ]\n");

	random_start(MIN_START_SEC, MAX_START_SEC);

	while (1) {
		(void)pause();
		// pause() unblocked by signal delivery!
	}
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
