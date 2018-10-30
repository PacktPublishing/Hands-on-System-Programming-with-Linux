/*
 * ch1:getreg_cr0.c
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
 * Brief Desription:
 * Inline assembly to access the contents of a CPU register.
 * NOTE: this program is written to work on x86_64 only.
 * For details, please refer the book, Ch 1.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned long u64;

static u64 get_cr0(void)
{
	/* Pro Tip: x86 ABI: query a register's value by moving it's value into RAX.
	 * [RAX] is returned by the function! */
	__asm__ __volatile__("movq %cr0, %rax");
	/* at&t syntax: movq <src_reg>, <dest_reg> */
}

int main(void)
{
	printf("Hello, inline assembly:\n [CR0] = 0x%lx\n", get_cr0());
	exit(0);
}
