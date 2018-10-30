# Makefile, Ch 3.
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 3 : Resource Limits
#----------------------------------------------------------------------
ALL :=  rlimits_show rlimits_show_dbg \
	rlimit_primes rlimit_primes_dbg

CC=gcc
ARMCC=${CROSS_COMPILE}gcc
CFLAGS=-Wall
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN= -fsanitize=memory -fPIE -pie
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} -Wall -c ../common.c -o common.o
common_arm.o: ../common.c ../common.h
	${ARMCC} -Wall -c ../common.c -o common_arm.o
common_dbg.o: ../common.c ../common.h
	${CC} -g -ggdb -gdwarf-4 -O0 -Wall -Wextra -c ../common.c -o common_dbg.o

rlimits_show: common.o rlimits_show.o
	${CC} -Wall -o rlimits_show rlimits_show.c common.o
rlimits_show_dbg.o: rlimits_show.c
	${CC} -g -ggdb -gdwarf-4 -O0 -Wall -Wextra -c rlimits_show.c -o rlimits_show_dbg.o
rlimits_show_dbg: rlimits_show_dbg.o common_dbg.o
	${CC} -o rlimits_show_dbg rlimits_show_dbg.o common_dbg.o

rlimit_primes: common.o rlimit_primes.o
	${CC} -Wall -o rlimit_primes rlimit_primes.c common.o
rlimit_primes_arm: common_arm.o rlimit_primes.o
	${ARMCC} -Wall -o rlimit_primes_arm rlimit_primes.c common_arm.o
rlimit_primes_dbg.o: rlimit_primes.c
	${CC} -g -ggdb -gdwarf-4 -O0 -Wall -Wextra -c rlimit_primes.c -o rlimit_primes_dbg.o
rlimit_primes_dbg: rlimit_primes_dbg.o common_dbg.o
	${CC} -o rlimit_primes_dbg rlimit_primes_dbg.o common_dbg.o

# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
