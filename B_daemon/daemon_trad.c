/*
 * B_daemon/daemon_trad.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch B : Daemon Processes
 ****************************************************************
 * Brief Description:
 * A quick demo of 'daemoninzing' a process into a traditional
 * SysV 'old-style' daemon process. (Lightly tested).
 * Ref: https://www.freedesktop.org/software/systemd/man/daemon.html
 *
 * For details, please refer the book, Ch B, Daemon Processes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common.h"

#define	SET_TO_NULDEV(orig_fd, new_fd) do {                       \
	if (dup2(orig_fd, new_fd) < 0) {                          \
		WARN("dup2(" #orig_fd ", " #new_fd ") failed\n"); \
		return -9;                                        \
	}                                                         \
} while(0)

/* 
 * The 'traditional' SysV daemon process.
 * Ref: man 7 daemon
 * Note: for the reader's convenience, we have reproduced the
 * numbered steps required to set up a 'traditional-style' SysV
 * daemon process directly from the man page on daemon(7). The 'step'
 * is mentioned here as a numbered comment; the actual code to
 * implement that step follows the comment.
 */
static int daemonize_traditional(void)
{
	int i, fd_null;
	struct rlimit rlim;
	struct sigaction act;
	sigset_t sigmask;

	/* 1. Close all open file descriptors except standard input, output,
	 * and error (i.e. the first three file descriptors 0, 1, 2). This
	 * ensures that no accidentally passed file descriptor stays around
	 * in the daemon process. On Linux, this is best implemented by
	 * iterating through /proc/self/fd, with a fallback of iterating from
	 * file descriptor 3 to the value returned by getrlimit()
	 * for RLIMIT_NOFILE. */
	if (prlimit(0, RLIMIT_NOFILE, 0, &rlim) < 0) {
		WARN("prlimit RLIMIT_NOFILE failed\n");
		return -1;
	}
	printf("%d:%s: [+] step 1 : close fd's (3 to %ld)\n",
	       getpid(), __func__, rlim.rlim_max);
	for (i = 3; i < rlim.rlim_max; i++)
		(void)close(i);

	/* 2. Reset all signal handlers to their default. This is best done
	 * by iterating through the available signals up to the limit of
	 * _NSIG and resetting them to SIG_DFL.
	 */
	printf("%d:%s: [+] step 2 : reset signals to default\n",
	       getpid(), __func__);
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_DFL;
	for (i = 1; i < _NSIG; i++) {
		if ((i == SIGKILL) ||	// skip the nonmaskable & NTPL signals
		    (i == SIGSTOP) || (i == 32) || (i == 33))
			continue;
		if (sigaction(i, &act, 0) == -1) {
			WARN("sigaction %d failed\n", i);
			return -2;
		}
	}

	/* 3. Reset the signal mask using sigprocmask(). */
	printf("%d:%s: [+] step 3 : reset signal mask\n", getpid(), __func__);
	if (sigemptyset(&sigmask) < 0) {
		WARN("sigemptyset failed\n");
		return -3;
	}

	/* 4. Sanitize the environment block, removing or resetting
	 * environment variables that might negatively impact
	 * daemon runtime. */

	/* 5. Call fork(), to create a background process. */
	printf("%d:%s: [+] step 5 : first fork\n", getpid(), __func__);
	switch (fork()) {
	case -1:
		WARN("fork #1 failed!\n");
		return -5;
	case 0:		// First child
		/* 6. In the child, call setsid() to detach from any terminal and
		 * create an independent session. */
		printf("%d:%s: [+] step 6 : create new session\n",
		       getpid(), __func__);
		if (setsid() < 0) {
			WARN("setsid failed\n");
			return -6;
		}

		/* 7. In the child, call fork() again, to ensure that the daemon
		 * can never re-acquire a terminal again. */
		printf("%d:%s: [+] step 7 : second fork\n", getpid(), __func__);
		switch (fork()) {
		case -1:
			WARN("fork #2 failed!\n");
			return -7;
		case 0:	// Second child; *the ACTUAL DAEMON*
			printf(" %d:%s: the ACTUAL DAEMON !\n",
			       getpid(), __func__);

			/* 9. In the daemon process, connect /dev/null to
			 * standard input, output, and error. */
			printf
			    ("%d:%s: [+] step 9 : connect null dev to fd's 0,1,2\n",
			     getpid(), __func__);
			fd_null = open("/dev/null", O_RDWR);
			if (fd_null < 0) {
				WARN("open on null dev failed\n");
				return -9;
			}
			/* From this point on, the printf's disappear! */
#if 1
			SET_TO_NULDEV(fd_null, STDIN_FILENO);
			SET_TO_NULDEV(fd_null, STDOUT_FILENO);
			SET_TO_NULDEV(fd_null, STDERR_FILENO);
#endif
			close(fd_null);

			/* 10. In the daemon process, reset the umask to 0, so that the file
			 * modes passed to open(), mkdir() and suchlike directly control the
			 * access mode of the created files and directories. */
			printf
			    ("%d:%s: [+] step 10 : set umask to 0x0\n",
			     getpid(), __func__);
			umask(0x0);

			/* 11. In the daemon process, change the current directory to the root
			 * directory (/), in order to avoid that the daemon involuntarily
			 * blocks mount points from being unmounted. */
			printf
			    ("%d:%s: [+] step 11 : set cwd to '/'\n",
			     getpid(), __func__);
			if (chdir("/") < 0) {
				WARN("chdir to '/' failed\n");
				return -11;
			}

			/* 12. In the daemon process, write the daemon PID (as returned by
			 * getpid()) to a PID file, for example /run/foobar.pid (for a
			 * hypothetical daemon "foobar") to ensure that the daemon cannot be
			 * started more than once. This must be implemented in race-free
			 * fashion so that the PID file is only updated when it is verified
			 * at the same time that the PID previously stored in the PID file
			 * no longer exists or belongs to a foreign process. */
			printf
			    ("%d:%s: [-] step 12 : write PID safely to run file\n"
			     " <not implemented here>\n",
			     getpid(), __func__);

			/* Recommended to use the Linux 'open file description locks'
			 * via the fcntl(2); pl see
			 * https://gavv.github.io/blog/file-locks/#open-file-description-locks-fcntl
			 * for details. 
			 * Not done here.*/

			/* 13. In the daemon process, drop privileges,
			 * if possible and applicable. */
			printf
			    ("%d:%s: [-] step 13 : drop privileges\n"
			     " <not implemented here>\n",
			     getpid(), __func__);

			/* 14. From the daemon process, notify the original process started
			 * that initialization is complete. This can be implemented via an
			 * unnamed pipe or similar communication channel that is created
			 * before the first fork() and hence available in both the original
			 * and the daemon process. */
			printf
			    ("%d:%s: [-] step 14 : inform original process we're ready\n"
			     " <not implemented here>\n",
			     getpid(), __func__);

			pause();
			exit(EXIT_SUCCESS);

		default:	// parent
			;
		}		// second fork

		/* 8. Call exit() in the first child, so that only the second
		 * child (the actual daemon process) stays around. This
		 * ensures that the daemon process is re-parented to init/PID
		 * 1, as all daemons should be. */
		printf("%d:%s: [+] step 8 : first child exit\n",
		       getpid(), __func__);
		exit(EXIT_SUCCESS);

	default:		// (original) parent
		/* 15. Call exit() in the original process. The process that invoked
		 * the daemon must be able to rely on that this exit() happens after
		 * initialization is complete and all external communication channels
		 * are established and accessible. */
		printf(
		"%d:%s: [+] step 15 : original parent exit when daemon is fully initialized\n",
		       getpid(), __func__);
		exit(EXIT_SUCCESS);
	}	// first fork

	return 0;
}

#if 0
static inline void usage(char *name)
{
	fprintf(stderr, "Usage: %s lock-or-not\n"
		" 0 : do Not lock (buggy!)\n" " 1 : do lock (correct)\n", name);
}
#endif

int main(int argc, char **argv)
{
	int ret;
	/*
	   if (argc != 2) {
	   usage(argv[0]);
	   exit(EXIT_FAILURE);
	   } */

	if ((ret = daemonize_traditional()) < 0)
		printf("Failed to daemonize (traditional): err code %d\n", ret);
}

/* vi: ts=8 */
