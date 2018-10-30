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
 * Brief Description:
 * A simple demo: we make several nested function calls, just so we
 * can use GDB to attach to the process and query it's process stack.
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
	printf("In function %s\n"
		"\t(bye, pl go '~/' now).\n", __FUNCTION__);
	printf("\n Now blocking on pause()...\n"
		" Connect via GDB's 'attach' and then issue the 'bt' command"
		" to view the process stack\n");
	pause(); /*process blocks here until it receives a signal */
}
static void bar(void)
{
	printf("In function %s\n", __FUNCTION__);
	bar_is_now_closed();
}
static void foo(void)
{
	printf("In function %s\n", __FUNCTION__);
	bar();
}

int main(int argc, char **argv)
{
	printf("In function %s\n", __FUNCTION__);
	foo();
	exit (EXIT_SUCCESS);
}
/* vi: ts=8 */
