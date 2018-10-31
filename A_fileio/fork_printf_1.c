/*
 * A_iobuf/fork_printf_1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:   A_iobuf : File IO Essentials
 ****************************************************************
 * Brief Description:
 * First call printf(3), then fork(2); that's it.
 * (Read chapter A and see A_iobuf/fork_printf_2.c to get the point :-).
 *
 * For details, please refer the book, Ch A, File I/O Essentials.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include "../common.h"

int main(int argc, char **argv)
{
	fork();
	printf("Hello, world. ");
}

/* vi: ts=8 */
