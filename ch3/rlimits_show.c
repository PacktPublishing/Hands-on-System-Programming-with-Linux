/*
 * ch3/rlimit_show.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:
 *  Ch 3 : Resource Limits
 ****************************************************************
 * Brief Description:
 * Query and display all process resource limits, using the prlimit(2)
 * system call.
 * For details, please refer the book, Ch 3.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../common.h"

/*---------------- Globals, Macros ----------------------------*/
#define ARRAY_LEN(arr) (sizeof((arr))/sizeof((arr)[0]))

/*---------------- Typedef's, constants, etc ------------------*/

/*---------------- Functions ----------------------------------*/
static void query_rlimits(void)
{
	unsigned i;
	struct rlimit rlim;
	struct rlimpair {
		int rlim;
		char *name;
	};
	struct rlimpair rlimpair_arr[] = {
		{RLIMIT_CORE, "RLIMIT_CORE"},
		{RLIMIT_DATA, "RLIMIT_DATA"},
		{RLIMIT_NICE, "RLIMIT_NICE"},
		{RLIMIT_FSIZE, "RLIMIT_FSIZE"},
		{RLIMIT_SIGPENDING, "RLIMIT_SIGPENDING"},
		{RLIMIT_MEMLOCK, "RLIMIT_MEMLOCK"},
		{RLIMIT_NOFILE, "RLIMIT_NOFILE"},
		{RLIMIT_MSGQUEUE, "RLIMIT_MSGQUEUE"},
		{RLIMIT_RTTIME, "RLIMIT_RTTIME"},
		{RLIMIT_STACK, "RLIMIT_STACK"},
		{RLIMIT_CPU, "RLIMIT_CPU"},
		{RLIMIT_NPROC, "RLIMIT_NPROC"},
		{RLIMIT_AS, "RLIMIT_AS"},
		{RLIMIT_LOCKS, "RLIMIT_LOCKS"},
	};
	char tmp1[16], tmp2[16];

	printf("RESOURCE LIMIT                 SOFT              HARD\n");
	for (i = 0; i < ARRAY_LEN(rlimpair_arr); i++) {
		if (prlimit(0, rlimpair_arr[i].rlim, 0, &rlim) == -1)
			FATAL("prlimit[%d] failed\n", i);

		snprintf(tmp1, 16, "%ld", rlim.rlim_cur);
		snprintf(tmp2, 16, "%ld", rlim.rlim_max);
		printf("%-18s:  %16s  %16s\n",
		       rlimpair_arr[i].name,
		       (rlim.rlim_cur == -1UL ? "unlimited" : tmp1),
		       (rlim.rlim_max == -1UL ? "unlimited" : tmp2)
		    );
	}
}

int main(void)
{
	query_rlimits();
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
