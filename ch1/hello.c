/*
 * ch1:hello.c
 *
 **********************************************************************
 * This program is part of the source code released for the book
 *  "Hands-On System Programming with Linux", by Kaiwan N Billimoria
 *  Packt Publishers
 *
 * From:
 *  Ch 1 : Linux System Architecture
 **********************************************************************
 * Brief Description:
 * A quick 'Hello, World'-like program to demonstrate using objdump(1)
 * to show the corresponding assembly and machine language.
 * For details, please refer the book, Ch 1.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int a;

	printf("Hello, Linux System Programming, World!\n");
	a = 5;
	exit(0);
}

/* vi: ts=8 */
