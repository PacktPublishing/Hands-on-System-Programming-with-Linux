/*
 * ch11/sig1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 11 : Signalling Part I
 ****************************************************************
 * Brief Description:
 * Put a process in an infinite loop, run it and then, "manually"
 * send it a signal via the keyboard.
 * This program does _nothing_ in terms of signalling except demonstrate
 * that, when unhandled, the default action of the SIGINT (^C, value 2)
 * signal is to kill the foreground process.
 *
 * For details, please refer the book, Ch 11.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "../common.h"

int main(void)
{
	unsigned long int i = 1;

	while (1) {
		printf("Looping, iteration #%02ld ...\n", i++);
		(void)sleep(1);
	}

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
