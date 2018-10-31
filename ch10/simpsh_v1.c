/*
 * ch10/simpsh_v1.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 10 : Process Creation
 ****************************************************************
 * Brief Description:
 * A small 'project': build a tiny, minimally functional shell.
 * No fancy features :) (we're serious: NO redirection, piping,
 * builtins, etc etc).
 * Still, it works: at the prompt, enter a *single-word* command,
 * and it shall obey! You will see the output of said command.
 * It's really a small demo of the UNIX "fork-exec with wait"
 * semantics.
 *
 * For details, please refer the book, Ch 10.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../common.h"

#define CMD_MAXLEN   128

static int gVerbose = 0;
const char *PromptStr = ">> ";

/* Display the prompt and receive user input */
static char *getcmd(char *cmdstr)
{
	printf("%s", PromptStr);
	fflush(stdout);

	if (!fgets(cmdstr, CMD_MAXLEN - 1, stdin))
		return NULL;
	/* fgets embeds the newline in the penultimate byte;
	 * get rid of it */
	cmdstr[strlen(cmdstr) - 1] = '\0';

	return cmdstr;
}

static void do_simpsh(void)
{
	pid_t ret;
	char *cmd;

	cmd = calloc(CMD_MAXLEN, sizeof(char));
	if (!cmd)
		FATAL("calloc failed!\n");

	while (1) {
		if (!getcmd(cmd)) {
			free(cmd);
			FATAL("getcmd(): EOF or failed\n");
		}

		/* Stopping condition */
		if (!strncmp(cmd, "quit", 4))
			break;

		if (cmd[0] == 0)    /* user pressed [Enter] */
			continue;

		/* Wield the powerful fork-exec-wait semantic ! */
		switch ((ret = fork())) {
		case -1:
			free(cmd);
			FATAL("fork failed, aborting!\n");
		case 0:	/* Child */
			VPRINT
			    (" Child process (%7d) exec-ing cmd \"%s\" now..\n",
			     getpid(), cmd);
			if (execlp(cmd, cmd, (char *)0) == -1) {
				WARN("child: execlp failed\n");
				free(cmd);
				exit(EXIT_FAILURE);
			}
			/* should never reach here */
			exit(EXIT_FAILURE);	// just to avoid gcc warnings
		default: /* Parent */
			VPRINT("Parent process (%7d) issuing the wait...\n",
			       getpid());
			/* sync: child runs first, parent waits for
			 * child's death */
			if (wait(0) < 0) 
				FATAL("wait failed, aborting..\n");
		} // switch
	} // while(1)
	free(cmd);
}

static inline void usage(char *name)
{
	printf("Usage: %s [-v]|[--help]\n"
	       " -v : verbose mode\n"
	       " --help : display this help screen.\n", name);
}

int main(int argc, char **argv)
{
	if (argc >= 2) {
		if (!strncmp(argv[1], "--help", 6)) {
			usage(argv[0]);
			exit(EXIT_SUCCESS);
		}
		if (!strncmp(argv[1], "-v", 2))
			gVerbose = 1;
	}

	do_simpsh();
	VPRINT("Parent process (%7d) exiting...\n", getpid());
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
