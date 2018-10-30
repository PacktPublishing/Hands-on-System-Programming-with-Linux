/*
 * ch1:getreg.c
 *
 **********************************************************************
 * This program is part of the source code released for the book
 *  "Linux System Programming"
 *  (c) Kaiwan N Billimoria
 *  Packt Publishers
 *
 * From:
 *  Ch 1 : Linux System Architecture
 **********************************************************************
 * Brief Description:
 * Inline assembly to access the contents of a CPU register.
 * NOTE: this program is written to work on x86_64 only.
 * For details, please refer the book, Ch 1.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned long u64;

static u64 get_rcx(void)
{
	__asm__ __volatile__("movq $5, %rcx\n\t"
			     "movq %rcx, %rax");/* at&t syntax: movq <src_reg>, <dest_reg> */
	/* Tip: by moving a register value into RAX, it's returned by the function! */
}

int main(void)
{
	printf("Hello, inline assembly:\n rcx = 0x%lx\n", get_rcx());
	exit(0);
}

/* vi: ts=8 */
