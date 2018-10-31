/*
 * ch14/speed_multiprcs_vs_multithrd_simple/matrixmul/prcs_matrixmul.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A quick test: matrix multiplication via: 
 * - process model (this program); operations done in sequence;
 * - threaded model; operations done in parallel.
 *
 * For details, please refer the book, Ch 14.
 */
#include <stdio.h>
#include <stdlib.h>
#include "../../../common.h"

#define N	1024 //32   // number of rows/columns
int a[N][N], b[N][N], c[N][N];

int main(void)
{
	int i, j, k;

	// Initialize the matrices 'a' and 'b'
	for (i = 0; i < N; i++)	{ 
		for (j = 0; j < N; j++) {
			a[i][j] = 2;
			b[i][j] = 3;
		}
	}

	// Matrix multiplication: c = a*b
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			for (k = 0; k < N; k++)
				c[i][j] = c[i][j] + a[i][k] * b[k][j];

#if 0
	printf("The resultant matrix is:\n");
	for (i = 0; i < N; i++) {
		printf(" [row %3d]  ", i+1);
		for (j = 0; j < N; j++)
			printf("%d ", c[i][j]);
		printf(" [row %3d]\n", i+1);
	}
#endif
}
