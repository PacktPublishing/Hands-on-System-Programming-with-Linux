/*
 * common.h
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:
 *  'Common' code.
 ****************************************************************
 * Brief Description:
 * This is the 'common' code that gets compiled into all binary
 * executables.
 */
#ifndef __HOLSP_COMMON_H__
#define __HOLSP_COMMON_H__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

/*--- 'Regular' headers ---*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/*---*/

/*--- Function prototypes ---*/
int show_blocked_signals(void);
int handle_err(int fatal, const char *fmt, ...);
int r_sleep(time_t sec, long nsec);


/*--- Macros ---*/
#define NON_FATAL    0

#define WARN(warnmsg, args...) do {                           \
	handle_err(NON_FATAL, "!WARNING! %s:%s:%d: " warnmsg, \
	   __FILE__, __FUNCTION__, __LINE__, ##args);         \
} while(0)
#define FATAL(errmsg, args...) do {                           \
	handle_err(EXIT_FAILURE, "FATAL:%s:%s:%d: " errmsg,   \
	   __FILE__, __FUNCTION__, __LINE__, ##args);         \
} while(0)

/*------------------------ DELAY_LOOP --------------------------------*/
static inline void beep(int what)
{
	char buf[1];
	buf[0] = what;
	(void)write(STDOUT_FILENO, buf, 1);
}

/* 
 * DELAY_LOOP macro
 * @val        : ASCII value to print
 * @loop_count : times to loop around
 */
#define HZ  250
#define DELAY_LOOP(val,loop_count)                                             \
{                                                                              \
	int c=0, m;                                                            \
	unsigned int for_index,inner_index;                                    \
	                                                                       \
	for(for_index=0;for_index<loop_count;for_index++) {                    \
		beep((val));                                                   \
		c++;                                                           \
		for(inner_index=0;inner_index<HZ*1000;inner_index++)           \
			for(m=0;m<30;m++);                                     \
		}                                                              \
	/*printf("c=%d\n",c);*/                                                \
}
#define DELAY_LOOP_SILENT(loop_count)                                          \
{                                                                              \
	int c=0, m;                                                            \
	unsigned int for_index,inner_index;                                    \
	                                                                       \
	for(for_index=0;for_index<loop_count;for_index++) {                    \
		c++;                                                           \
		for(inner_index=0;inner_index<HZ*1000;inner_index++)           \
			for(m=0;m<30;m++);                                     \
		}                                                              \
	/*printf("c=%d\n",c);*/                                                \
}

/*-------------------- VPRINT : verbose print ----------------------------*/
/* IMP! 
 * The VPRINT macro depends on the caller having a (usually global) variable
 * named 'gVerbose'. IFF it exists and is True, the printf gets emitted.
 */
#define VPRINT(msg, args...) do {                      \
	if (gVerbose)                                  \
		printf("%s:%s:%d: " msg,               \
	   __FILE__, __FUNCTION__, __LINE__, ##args);  \
} while(0)

/*------------------------ timerspecsub ----------------------------------*/
/*
 * Macro: timerspecsub
 * Perform (on struct timespec's): result=(a-b).
 * Modified from the original 'timersub' macro defined in the <sys/time.h> header.
 * From the original man page entry:
 * "timersub() subtracts the time value in b from the time value in a, and
 * places the result in the timeval pointed to by res. The result is
 * normalized such that res->tv_usec (now becomes 'nsec') has a value in the
 * range 0 to 999,999. (now 999,999,999)"
 */
#define timerspecsub(a, b, result)					      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_nsec = (a)->tv_nsec - (b)->tv_nsec;			      \
    if ((result)->tv_nsec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_nsec += 1000000000;					      \
    }									      \
  } while (0)

/*--------------- Mutex Lock and Unlock ----------------------------------*/
#define LOCK_MTX(mtx) do {                                       \
	int ret=0;                                               \
	if ((ret = pthread_mutex_lock(mtx)))                     \
		FATAL("pthread_mutex_lock failed! [%d]\n", ret); \
} while(0)

#define UNLOCK_MTX(mtx) do {                                       \
	int ret=0;                                                 \
	if ((ret = pthread_mutex_unlock(mtx)))                     \
		FATAL("pthread_mutex_unlock failed! [%d]\n", ret); \
} while(0)

#endif
