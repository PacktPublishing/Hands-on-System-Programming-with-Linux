/*
 * ch14/disp_defattr_pthread.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * Query and display the default thread attributes that a pthread will be
 * created with if the attribute structure parameter to pthread_create(3)
 * is passed as NULL.
 *
 * For details, please refer the book, Ch 14.
 */
#include "../common.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static void display_thrd_attr(pthread_attr_t *attr)
{
	int detachst=0;
	int sched_scope=0, sched_inh=0, sched_policy=0;
	struct sched_param sch_param;
	size_t guardsz=0, stacksz=0;
	void *stackaddr;

	// Query and display the 'Detached State'
	if (pthread_attr_getdetachstate(attr, &detachst))
		WARN("pthread_attr_getdetachstate() failed.\n");
	printf("Detach State : %s\n",
		(detachst == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
		(detachst == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
		 "<unknown>");

	//--- Scheduling Attributes
	printf("Scheduling \n");
	// Query and display the 'Scheduling Scope'
	if (pthread_attr_getscope(attr, &sched_scope))
		WARN("pthread_attr_getscope() failed.\n");
	printf(" Scope       : %s\n",
		(sched_scope == PTHREAD_SCOPE_SYSTEM) ? "PTHREAD_SCOPE_SYSTEM" :
		(sched_scope == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" :
		 "<unknown>");

	// Query and display the 'Scheduling Inheritance'
	if (pthread_attr_getinheritsched(attr, &sched_inh))
		WARN("pthread_attr_getinheritsched() failed.\n");
	printf(" Inheritance : %s\n",
		(sched_inh == PTHREAD_INHERIT_SCHED) ? "PTHREAD_INHERIT_SCHED" :
		(sched_inh == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
		 "<unknown>");

	// Query and display the 'Scheduling Policy'
	if (pthread_attr_getschedpolicy(attr, &sched_policy))
		WARN("pthread_attr_getschedpolicy() failed.\n");
	printf(" Policy      : %s\n",
		(sched_policy == SCHED_FIFO)  ? "SCHED_FIFO" :
		(sched_policy == SCHED_RR)    ? "SCHED_RR" :
		(sched_policy == SCHED_OTHER) ? "SCHED_OTHER" :
		 "<unknown>");

	// Query and display the 'Scheduling Priority'
	if (pthread_attr_getschedparam(attr, &sch_param))
		WARN("pthread_attr_getschedparam() failed.\n");
	printf(" Priority    : %d\n", sch_param.sched_priority);

	//--- Thread Stack Attributes
	printf("Thread Stack \n");
	// Query and display the 'Guard Size'
	if (pthread_attr_getguardsize(attr, &guardsz))
		WARN("pthread_attr_getguardsize() failed.\n");
	printf("  Guard Size : %9zu bytes\n", guardsz);

	/* Query and display the 'Stack Size':
	 * 'stack location' will be meaningless now as there is no
	 * actual thread created yet!
	 */
	if (pthread_attr_getstack(attr, &stackaddr, &stacksz))
		WARN("pthread_attr_getstack() failed.\n");
	printf("  Stack Size : %9zu bytes\n", stacksz);
}

int main(void)
{
	int ret;
	pthread_attr_t attr;

	ret = pthread_getattr_default_np(&attr);
	if (ret)
		FATAL("pthread_getattr_default_np() failed! [%d]\n", ret);
	printf("Linux Default Thread Attributes:\n");
	display_thrd_attr(&attr);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
