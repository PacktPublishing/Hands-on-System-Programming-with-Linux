/*
 * ch18/mmap_file_simple.c
 ***************************************************************
 * This program is part of the source code released for the book
 *  "Hands-on System Programming with Linux"
 *  (c) Author: Kaiwan N Billimoria
 *  Publisher:  Packt
 *
 * From: Ch 18 : Advanced File IO
 ****************************************************************
 * Brief Description:
 * mmap() a file region and display the contents. We setup a
 * private mapping.
 *
 * For details, please refer the book, Ch 18.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common.h"

/*--------------- Function hex_dump() sourced from:
http://www.alexonlinux.com/hex-dump-functions

Added an start-offset, a 'verbose' parameter, and fixed a potential
buffer overflow issue (by increasing the 'temp' array size to 20 (from just 8),
and replacing the sprintf(3) with snprintf(3).
(Kaiwan Billimoria).
*/
void hex_dump(unsigned char *data, int offset, int size, char *caption, int verbose)
{
	int i;			// index in data...
	int j;			// index in line...
#define MAX_NUMERALS 20  // max # of chars needed to hold an offset
	char temp[MAX_NUMERALS];
	char buffer[128];
	char *ascii;

	memset(buffer, 0, 128);

	if (verbose && caption)
		printf("---------> %s <--------- (%d bytes from %p)\n", caption,
		       size, data);

	// Printing the ruler...
	printf
	    ("        +0          +4          +8          +c            0   4   8   c   \n");

	// Hex portion of the line is 8 (the padding) + 3 * 16 = 52 chars long
	// We add another four bytes padding and place the ASCII version...
	ascii = buffer + 58;
	memset(buffer, ' ', 58 + 16);

	snprintf(temp, MAX_NUMERALS, "+%06d", offset);
	buffer[58 + 16] = '\n';
	buffer[58 + 17] = '\0';
	buffer[0] = '+';

	// Set offset to initial offset
	buffer[1] = temp[1];
	buffer[2] = temp[2];
	buffer[3] = temp[3];
	buffer[4] = temp[4];
	buffer[5] = temp[5];
	buffer[6] = temp[6];

	for (i = 0, j = 0; i < size; i++, j++) {
		if (j == 16) {
			printf("%s", buffer);
			memset(buffer, ' ', 58 + 16);

			snprintf(temp, MAX_NUMERALS, "+%06d", i+offset); // set offset to initial offset
			memcpy(buffer, temp, 7);

			j = 0;
		}

		snprintf(temp, MAX_NUMERALS, "%02x", 0xff & data[i]);
		memcpy(buffer + 8 + (j * 3), temp, 2);
		if ((data[i] > 31) && (data[i] < 127))
			ascii[j] = data[i];
		else
			ascii[j] = '.';
	}

	if (j != 0)
		printf("%s", buffer);
}

static int gVerbose=1;

int main(int argc, char **argv)
{
	int fd_from;
	off_t fsz, len, off = 0;
	struct stat sstat;
	void *data_src=0, *mmap_base=0;

	if (argc < 2 || argc > 4 || argc == 3) {
		fprintf(stderr, "Usage: %s source-file [start_offset length]\n"
				" Either specify Both start_offset and length or don't specify either\n"
				" If specified, both start_offset and length are taken into account\n"
				" If not, we dump the entire file.\n",
			argv[0]);
		exit(EXIT_FAILURE);
	}

	fd_from = open(argv[1], O_RDONLY);
	if (fd_from < 0)
		FATAL("open: src file");

	/* Query src file size */
	if (fstat(fd_from, &sstat) == -1)
		FATAL("fstat on src file %s failed\n", argv[1]);

	fsz = sstat.st_size;
	if (0 == fsz)
		FATAL("source-file %s size 0 bytes, aborting...\n", argv[1]);

	len = fsz;
	if (argc == 4) {
		len = atol(argv[3]);
		if (len <= 0)
			FATAL("invalid length %ld, aborting...\n", len);
	}

	if (argc >= 3) {
		off = atol(argv[2]);
		if ((off < 0) || ((off+len) > fsz))
			FATAL("invalid offset or offset/length combination, aborting...\n");
	}

	/*
	 File-map via the mmap(2):
	   void *mmap(void *addr, size_t length, int prot, int flags,
	              int fd, off_t offset);
	 A private mapping (obtained by using the MAP_PRIVATE flag), will
	 initialize the mapped memory to the file region being mapped; any
	 modifications to the mapped region are _not_ carried through to
	 the file, though.
	 */
	mmap_base = data_src = mmap(0, fsz, PROT_READ, MAP_PRIVATE, fd_from, 0);
	if (data_src == MAP_FAILED)
		FATAL("mmap: to src file %s failed\n", argv[1]);

	if ((unsigned long)mmap_base % getpagesize() != 0)
		WARN("mmap: not aligned to page boundary\n");
	VPRINT("fsz=%ld  mmap_base = %p\n", fsz, mmap_base);

	// void hex_dump(unsigned char *data, int offset, int size, char *caption, int verbose)
	hex_dump(data_src + off, off, len, "Data", 0);

	/* int msync(void *addr, size_t length, int flags); */
	if (msync (mmap_base, len, MS_SYNC) < 0) /* actually, the msync is redundant
				         here as we're calling munmap() immd ..*/
		WARN("mysnc failed\n");

	if (munmap(mmap_base, len) == -1)
		FATAL("munmap failed\n");

	close(fd_from);
	exit(EXIT_SUCCESS);
}

/* vi: ts=8 */
