/*
 * ch8/query_pcap.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 8 : Process Capabilities
 ****************************************************************
 * For details, please refer the book, Ch 8.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/capability.h>
#include "../common.h"

int main(int argc, char **argv)
{
	pid_t pid;
	cap_t pcaps;
	char *caps_text = NULL;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s PID\n"
			" PID: process to query capabilities of\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	pid = atoi(argv[1]);

#if 0
	printf("System supports CAP_SETFCAP? %s\n",
	       (CAP_IS_SUPPORTED(CAP_SETFCAP) ? "yes" : "no"));
#endif
	pcaps = cap_get_pid(pid);
	if (!pcaps)
		FATAL("cap_get_pid failed; is process %d valid?\n", pid);

	caps_text = cap_to_text(pcaps, NULL);
	if (!caps_text)
		FATAL("caps_to_text failed\n", argv[1]);

	printf("Process %6d : capabilities are: %s\n", pid, caps_text);
	cap_free(caps_text);

	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
