/*
 * ch2:stacker.c
 * 
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:
 *  Ch 2 : Virtual Memory
 ****************************************************************
 * Updates:
 * 20181226: to 'show' a good approximation of the stack location, display
 * the address of a local variable (which of course is on the stack).
 * (Thanks to F Ritchie for pointing this out!).
 *
 * Brief Description:
 * A simple demo: we make several nested function calls, just so we
 * can use GDB to attach to the process and query it's process stack.
 *
 * For details, please refer the book, Ch 2.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

static void bar_is_now_closed(void)
{
	int localvar = 5;

	printf("In function %20s; &localvar = %p\n"
		"\t(bye, pl go '~/' now).\n",
		__FUNCTION__, &localvar);
	printf("\n Now blocking on pause()...\n"
		" Connect via GDB's 'attach' and then issue the 'bt' command"
		" to view the process stack\n");
	pause(); /*process blocks here until it receives a signal */
}
static void bar(void)
{
	int localvar = 5;

	printf("In function %20s; &localvar = %p\n", __FUNCTION__, &localvar);
	bar_is_now_closed();
}
static void foo(void)
{
	int localvar = 5;

	printf("In function %20s; &localvar = %p\n", __FUNCTION__, &localvar);
	bar();
}

int main(int argc, char **argv)
{
	int localvar = 5;

	printf("In function %20s; &localvar = %p\n", __FUNCTION__, &localvar);
	foo();
	exit (EXIT_SUCCESS);
}
/* vi: ts=8 */
