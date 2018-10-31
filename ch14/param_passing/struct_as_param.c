/*
 * ch14/param_passing/struct_as_param.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From:  Ch 14 : Multithreading Part I - The Essentials
 ****************************************************************
 * Brief Description:
 * A demo of passing a data structure as the parameter to multiple
 * threads, as they come alive.
 *
 * For details, please refer the book, Ch 14.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../../common.h"

#define NTHREADS	  3
#define IATA_MAXSZ	  3
#define CITY_MAXSZ	128

/* Our data structure that we intend to pass as a parameter to the threads.
 * City Airport information. 
 */
typedef struct {
	char IATA_code[IATA_MAXSZ]; /* http://www.nationsonline.org/oneworld/IATA_Codes/ */
	char city[CITY_MAXSZ];      /* city name */
	float latitude, longitude;  /* coordinates of the city airport */
	unsigned int altitude;      /* metres */
	/* todo: add # runways, runway direction, radio beacons freq, ILS,etc */
	unsigned long reserved;     /* for future use */
} Airport;

static const Airport city_airports[3] = {
	/* yes! the {lat,long,alt} tuple is accurate :-) */
	{ "BLR", "Bangalore International", 13.1986, 77.7066, 904, 0 },
	{ "BNE", "Brisbane International", 27.3942, 153.1218, 4, 0 },
	{ "BRU", "Brussels National", 50.9010, 4.4856, 58, 0 },
};

void * worker(void *data)
{
	Airport * arg = (Airport *)data;
	int slptm=8;

	printf( "\n----------- Airports Details ---------------\n"
		"  IATA code : %.*s           %32s\n"
		"  Latitude, Longitude, Altitude : %9.4f %9.4f %9um\n"
		, IATA_MAXSZ, arg->IATA_code,
		arg->city,
		arg->latitude, arg->longitude, arg->altitude);

	printf(" worker : will sleep for %ds now ...\n", slptm);
	sleep(slptm);
	printf(" worker : work done, exiting now\n");

	/* Terminate with success: status value 0.
	 * The join will pick this up. */
	pthread_exit((void *)0);
}

int main(void)
{
	long i;
	int ret, stat=0;
	pthread_t tid[NTHREADS];
	pthread_attr_t attr;
	Airport * plocdata[NTHREADS];

	/* Init the thread attribute structure to defaults */
	pthread_attr_init(&attr);
	/* Create all threads as joinable */
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Thread creation loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: creating thread #%ld ...\n", i);

		/* Allocate and initialize data structure to be passed to the
		 * thread as a parameter */
		plocdata[i] = calloc(1, sizeof(Airport));
		if (!plocdata[i])
			FATAL("calloc [%d] failed\n", i);
		memcpy(plocdata[i], &city_airports[i], sizeof(Airport));

		ret = pthread_create(&tid[i], &attr, worker, (void *)plocdata[i]);
		if (ret)
			FATAL("pthread_create() index %d failed! [%d]\n", i, ret);
	}
	pthread_attr_destroy(&attr);

	// Thread join loop
	for (i = 0; i < NTHREADS; i++) {
		printf("main: joining (waiting) upon thread #%ld ...\n", i);
		ret = pthread_join(tid[i], (void **)&stat);
		if (ret)
			WARN("pthread_join() index %d failed! [%d]\n", i, ret);
		else
			printf("Thread #%ld successfully joined; it terminated with "
				"status=%d\n", i, stat);
		free(plocdata[i]);
	}

	printf("\nmain: now dying... <Dramatic!> Farewell!\n");
	pthread_exit(NULL);
}

/* vi: ts=8 */
