/*
 * ch7/rootsh_hack1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 7 : Process Credentials
 ****************************************************************
 * Brief Description:
 * For details, please refer the book, Ch 7.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

int main(int argc, char **argv)
{
	/* Just spawn a shell.
	 * If this process runs as root,
	 * then, <i>Evil Laugh</i>, we're now root!
	 */
	system("/bin/bash");
	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
