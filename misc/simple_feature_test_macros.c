/*
 * misc/feature_test_macros.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 ****************************************************************
 * Brief Description:
 * Check for and print the value of several feature test macros 
 * (FTMs). To interpret, pl read the man page on 
 * feature_test_macros(7).
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include "../common.h"

int main(int argc, char **argv)
{
	printf("C STD = %ldL\n", __STDC_VERSION__);
	if (__STDC_VERSION__ == 201112L)
		printf(" C11 [latest ver as of date]\n");
	
	printf("=========== Simple Feature Test Macros ===========\n");

#ifdef _POSIX_C_SOURCE
	printf("_POSIX_C_SOURCE is defined         : value = %ldL\n", _POSIX_C_SOURCE);
#else
	printf("_POSIX_C_SOURCE is undefined\n");
#endif

#ifdef _POSIX_SOURCE
	printf("_POSIX_SOURCE is defined           : value = %d\n", _POSIX_SOURCE);
#else
	printf("_POSIX_SOURCE is undefined\n");
#endif

#ifdef __STRICT_ANSI__
	printf("__STRICT_ANSI__ is defined         : value = %d\n", __STRICT_ANSI__);
#else
	printf("__STRICT_ANSI__ is undefined\n");
#endif

#ifdef _XOPEN_SOURCE
	printf("_XOPEN_SOURCE is defined           : value = %d\n", _XOPEN_SOURCE);
#else
	printf("_XOPEN_SOURCE is undefined\n");
#endif

#ifdef _XOPEN_SOURCE_EXTENDED
	printf("_XOPEN_SOURCE_EXTENDED is defined  : value = %d\n", _XOPEN_SOURCE_EXTENDED);
#else
	printf("_XOPEN_SOURCE_EXTENDED is undefined\n");
#endif

#ifdef _ISOC99_SOURCE
	printf("_ISOC99_SOURCE is defined          : value = %d\n", _ISOC99_SOURCE);
#else
	printf("_ISOC99_SOURCE is undefined\n");
#endif

#ifdef _ISOC11_SOURCE
	printf("_ISOC11_SOURCE is defined          : value = %d\n", _ISOC11_SOURCE);
#else
	printf("_ISOC11_SOURCE is undefined\n");
#endif

#ifdef _LARGEFILE64_SOURCE
	printf("_LARGEFILE64_SOURCE is defined     : value = %d\n", _LARGEFILE64_SOURCE);
#else
	printf("_LARGEFILE64_SOURCE is undefined\n");
#endif

#ifdef _LARGEFILE_SOURCE
	printf("_LARGEFILE_SOURCE is defined       : value = %d\n", _LARGEFILE_SOURCE);
#else
	printf("_LARGEFILE_SOURCE is undefined\n");
#endif

#ifdef _FILE_OFFSET_BITS
	printf("_FILE_OFFSET_BITS is defined       : value = %d\n", _FILE_OFFSET_BITS);
#else
	printf("_FILE_OFFSET_BITS is undefined\n");
#endif

#ifdef _BSD_SOURCE
	printf("_BSD_SOURCE is defined             : value = %d\n", _BSD_SOURCE);
#else
	printf("_BSD_SOURCE is undefined\n");
#endif

#ifdef _SVID_SOURCE
	printf("_SVID_SOURCE is defined            : value = %d\n", _SVID_SOURCE);
#else
	printf("_SVID_SOURCE is undefined\n");
#endif

#ifdef _DEFAULT_SOURCE
	printf("_DEFAULT_SOURCE is defined         : value = %d\n", _DEFAULT_SOURCE);
#else
	printf("_DEFAULT_SOURCE is undefined\n");
#endif

#ifdef _ATFILE_SOURCE
	printf("_ATFILE_SOURCE is defined          : value = %d\n", _ATFILE_SOURCE);
#else
	printf("_ATFILE_SOURCE is undefined\n");
#endif

#ifdef _GNU_SOURCE
	printf("_GNU_SOURCE is defined\n");
#else
	printf("_GNU_SOURCE is undefined\n");
#endif

#ifdef _REENTRANT
	printf("[Obsolete] _REENTRANT is defined   : value = %d\n", _REENTRANT);
#else
	printf("_REENTRANT is undefined\n");
#endif

#ifdef _THREAD_SAFE
	printf("[Deprecated] _THREAD_SAFE is defined : value = %d\n", _THREAD_SAFE);
#else
	printf("_THREAD_SAFE is undefined\n");
#endif

#ifdef _FORTIFY_SOURCE
	printf("_FORTIFY_SOURCE is defined         : value = %d\n", _FORTIFY_SOURCE);
#else
	printf("_FORTIFY_SOURCE is undefined\n");
#endif

#ifdef __SI_HAVE_SIGSYS
	printf("Signal SIGSYS is defined         : value = %d\n", SIGSYS);
#else
	printf("Signal SIGSYS is undefined\n");
#endif

	exit (EXIT_SUCCESS);
}

/* vi: ts=8 */
