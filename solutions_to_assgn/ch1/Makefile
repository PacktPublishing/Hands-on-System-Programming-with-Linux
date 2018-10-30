# Makefile
# This program is part of the source code released for the book
#  "Linux System Programming"
#  (c) Kaiwan N Billimoria
#  Packt Publishers
#
# From:
#  Ch 1 : Linux System Architecture
#  Assignment #5 : Makefile.
ALL := getreg_cr2 getreg_cr2_dbg

CC=gcc
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
CFLAGS=-Wall -Wextra

all: ${ALL}
CB_FILES := *.[ch]

getreg_cr2: getreg_cr2.c
	${CC} ${CFLAGS} getreg_cr2.c -o getreg_cr2
getreg_cr2_dbg: getreg_cr2.c
	${CC} ${CFLAGS_DBG} getreg_cr2.c -o getreg_cr2_dbg

# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* *.o
