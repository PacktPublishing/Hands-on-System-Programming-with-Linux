/*
 * ch15/pshared_mutex_demo.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 15 : Multithreading Part II - Synchronization
 ****************************************************************
 * Brief Description:
 * This small application creates two shared memory regions:
 * - one, to act as a 'shared space' for a 'process-shared' mutex lock,
 * - two, to act as a simple buffer to store IPC 'messages'.
 * We initialize a mutex with the 'process-shared' attribute so that it can
 * be used between threads of differing processes to synchronize access; here,
 * we fork and have a thread of the original parent process and the newly born
 * child process compete for the mutex lock. Once they (sequentially) obtain
 * it, they write a message into the second shared memory segment. At the end
 * of the app, we destroy the resources and display the shared 'comm' memory
 * buffer.
 *
 * For details, please refer the book, Ch 15.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../common.h"

#define NTHREADS	  1
#define NUM_PSMUTEX	  1
#define GBUFSIZE	128

static pthread_once_t *mutex_init_once;
/*static pthread_once_t mutex_init_once=PTHREAD_ONCE_INIT;
 * Ideally, this is how the pthread_once_t variable should be statically
 * initialized; we want to use it within a shared memory segment though,
 * so we set it up there and init it at runtime - not correct, according
 * to POSIX!
 */
static pthread_mutexattr_t mtx_attr;
static pthread_mutex_t *shmtx;

static int gshm_id, gshmbuf_id;
static char * gshmbuf;

static inline void print_line(const char *buf, int start, int numbytes)
{
	int j;

	printf ("%08d  ", start);
	for (j=start; j<start+numbytes; j++) {
			printf ("%02x ", (unsigned char)buf[j]);
	}

	/* Print in ASCII as well */
	printf ("          ");
	for (j=start; j<start+numbytes; j++) {
		if (isprint ((int)buf[j]))
			printf ("%c", (unsigned char)buf[j]);
		else
			printf (".");
	}
	printf ("\n");
}

static void showbuf(const char *buf)
{
	int i;

	for (i=0; i < GBUFSIZE/16; i++)
		print_line(buf, i*16, 16);
}

static void * worker(void *data)
{
	long datum = (long)data;
	printf("Worker thread #%ld [%d] running ...\n", datum, getpid());
	sleep(1);
	printf(" [thrd %ld]: attempting to take the shared mutex lock...\n", datum);

	LOCK_MTX(shmtx);
	/*--- critical section begins */
	printf(" [thrd %ld]: got the (shared) lock!\n", datum);
	/* Lets write into the shmem buffer; first, a 5-byte 'signature',
	   followed by a message. */
	memset(&gshmbuf[0]+25, 't', 5);
	snprintf(&gshmbuf[0]+31, 32, "thread %d here!\n", getpid());
	/*--- critical section ends */
	UNLOCK_MTX(shmtx);

	printf("#%ld: work done, exiting now\n", datum);
	pthread_exit(NULL);
}

/* Create a (SysV IPC) shared memory region of 'size' bytes.
 * The first parameter is a value-result style one; it will hold the
 * shared memory ID.
 * If the shmem region already exists, just pass it's key value in
 * the 'key' parameter to gain access to it.
 */
static void * shmem_setup(int *shmid, char *name, key_t key, int size)
{
	void *shmaddr;

	*shmid = shmget(IPC_PRIVATE, size, (IPC_CREAT|IPC_EXCL|0600));
	if (*shmid < 0 ) {
		/* Already created? then just access it */
		*shmid = shmget(key, 0, 0);
		if (*shmid < 0 )
			FATAL("shmget failed\n");
	}
	printf ("%s:%d: shmem segment successfully created / accessed. ID=%d\n",
		name, getpid(), *shmid);

	/* Attach to the shmem segment */
	shmaddr = shmat(*shmid, 0, 0);
	if(shmaddr == (void *)-1)
		FATAL("shmat failed\n");
	printf ("%s:%d: Attached successfully to shmem segment at %p\n",
		name, getpid(), (void *)shmaddr);

	return shmaddr;
}

/* Initialize the mutex object with the process-shared attribute;
 * will be called only once.
 */
static void init_mutex(void)
{
	int ret=0;

	printf("[pthread_once(): calls %s(): from PID %d]\n",
			__func__, getpid());
	ret = pthread_mutexattr_init(&mtx_attr);
	if (ret)
		FATAL("pthread_mutexattr_init failed [%d]\n", ret);

	ret = pthread_mutexattr_setpshared(&mtx_attr, PTHREAD_PROCESS_SHARED);
	if (ret)
		FATAL("pthread_mutexattr_setpshared failed [%d]\n", ret);

	// TODO - robust mutex
	// TODO - error check mutex
	
	ret = pthread_mutex_init(shmtx, &mtx_attr);
	if (ret)
		FATAL("pthread_mutex_init failed [%d]\n", ret);
}

int main(int argc, char **argv)
{
	long i;
	int ret, stat=0;
	void *shmaddr;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	pid_t cpid=0;

	/* Setup a shared memory region for the process-shared mutex lock.
	 * A bit of complexity due to the fact that we use the space within for:
	 * a) memory for 1 process-shared mutex
	 * b) 32 bytes of padding (not strictly required)
	 * c) memory for 1 pthread_once_t variable.
	 * We need the last one for performing guaranteed once-only
	 * initialization of the mutex object.
	 */
	shmaddr = shmem_setup(&gshm_id, argv[0], 0, 
			(NUM_PSMUTEX*sizeof(pthread_mutex_t) + 32 + sizeof(pthread_once_t)));
	if (!shmaddr)
		FATAL("shmem setup 1 failed\n");
	//system("ipcs -m"); // peek at all shmem regions

	/* Associate the shared memory segment with the mutex and
	 * the pthread_once_t variable. */
	shmtx = (pthread_mutex_t *)shmaddr;
	mutex_init_once = (pthread_once_t *)shmaddr + (NUM_PSMUTEX*sizeof(pthread_mutex_t)) + 32;
	*mutex_init_once = PTHREAD_ONCE_INIT; /* see below comment on pthread_once */

	/* Setup a second shared memory region to be used as a comm buffer */
	gshmbuf = shmem_setup(&gshmbuf_id, argv[0], 0, GBUFSIZE);
	if (!gshmbuf)
		FATAL("shmem setup 2 failed\n");
	memset(gshmbuf, 0, GBUFSIZE);

	/* Initialize the mutex; here, we come across a relevant issue: this
	 * mutex object is already instantiated in a shared memory region that
	 * other processes might well have access to. So who will initialize
	 * the mutex? (it must be done only once).
	 * Enter the pthread_once(3) API: it guarantees that, given a
	 * 'once_control' variable (1st param), the 2nd param - a function
	 * pointer, that function will be called exactly once.
	 *
	 * However: the reality is that the pthread_once is meant to be used
	 * between the threads of a process. Also, POSIX requires that the
	 * initialization of the 'once_control' is done statically; here, we
	 * have performed it at runtime...
	 */
	ret = pthread_once(mutex_init_once, init_mutex);
	if (ret)
		FATAL("pthread_once failed [%d]\n", ret);
	
	ret = pthread_attr_init(&attr);
	if (ret)
		FATAL("pthread_attr_init failed [%d]\n", ret);

	/* Create the thread(s) */
	for (i = 0; i < NTHREADS; i++) {
		ret = pthread_create(&tid[i], &attr, worker, (void *)i);
		if (ret)
			FATAL("pthread_create() failed! [%d]\n", ret);
	}
	pthread_attr_destroy(&attr);

	/* Fork off a child process, and use the process-shared mutex
	 * within it */
	switch(cpid = fork()) {
		case -1 : FATAL("fork failed!\n");
		case 0  : // Child process
			  ret = pthread_once(mutex_init_once, init_mutex);
			  if (ret)
				FATAL("pthread_once failed [%d]\n", ret);
			  sleep(1);
			  printf(" Child[%d]: attempting to taking the shared mutex lock...\n",
					  getpid());
			  LOCK_MTX(shmtx);
			  /*--- critical section begins */
			  printf(" Child[%d]: got the (shared) lock!\n", getpid());
			  /* Lets write into the shmem buffer; first, a 5-byte
			   * 'signature', followed by a message. */
			  memset(&gshmbuf[0], 'c', 5);
			  snprintf(&gshmbuf[0]+6, 32, "child %d here!\n", getpid());
			  /*--- critical section ends */
			  UNLOCK_MTX(shmtx);
			  exit(EXIT_SUCCESS);
		default : // Parent process
			  if (wait(0) < 0) /* wait for child to die, 
				      preventing a naughty zombie */
				  WARN("wait failed\n");
	}

	/* Have main join on the thread(s) */
	for (i = 0; i < NTHREADS; i++) {
		printf("main: joining (waiting) upon thread #%ld ...\n", i);
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() failed! [%d]\n", ret);
		else
			printf("Thread #%ld successfully joined; it terminated with "
				"status=%d\n", i, stat);
	}

	printf("\nShared Memory 'comm' buffer:\n");
	showbuf(gshmbuf);

	/* Cleanup: detach and delete the shmem segments */
	if (shmdt(shmaddr) < 0)
		WARN("Shmem segment detach 1 failed\n");
	if (shmctl(gshm_id, IPC_RMID, 0) < 0)
		FATAL("Deletion of shmem segment 1 failed\n");

	if (shmdt(gshmbuf) < 0)
		WARN("Shmem segment detach 2 failed\n");
	if (shmctl(gshmbuf_id, IPC_RMID, 0) < 0)
		FATAL("Deletion of shmem segment 2 failed\n");

	pthread_exit(NULL);
}

/* vi: ts=8 */
