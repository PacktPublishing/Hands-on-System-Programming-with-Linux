/*
 * solutions_to_assgn/ch1/getreg_cr2.c
 ****************************************************************
 * This program is part of the source code released for the book
 *  "Linux System Programming"
 *  (c) Kaiwan N Billimoria
 *  Packt Publishers
 *
 * From:
 *  Ch 1 : Linux System Architecture
 ****************************************************************
 * Assignment #5:
 * Write a C program to access the contents of the CR2 control
 * register on the Intel x86_64 system (Tip: use inline assembly).
 * Does it work? Why?
 *
 * NOTE: this program is written to work on x86_64 only.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned long u64;

static u64 get_cr2(void)
{
	/* Pro Tip: x86 ABI: query a register's value by moving it's value into RAX.
	 * [RAX] is returned by the function! */
	__asm__ __volatile__("movq %cr2, %rax"); 
		/* at&t syntax: movq <src_reg>, <dest_reg> */
}

int main(void)
{
	printf("Hello, inline assembly:\n [CR2] = 0x%lx\n", get_cr2());
	exit(0);
}
