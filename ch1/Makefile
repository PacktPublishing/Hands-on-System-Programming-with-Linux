# Makefile
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
#  For Ch 1 : Linux System Architecture
#----------------------------------------------------------------------
ALL := hello hello_dbg getreg_rcx getreg_cr0

CC=gcc
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
CFLAGS=-Wall -Wextra

all: ${ALL}
CB_FILES := *.[ch]

hello: hello.c
	${CC} ${CFLAGS} hello.c -o hello
hello_dbg: hello.c
	${CC} ${CFLAGS_DBG} hello.c -o hello_dbg
	objdump --source ./hello_dbg > hello_dbg_objdump.txt

getreg_rcx: getreg_rcx.c
	${CC} ${CFLAGS} getreg_rcx.c -o getreg_rcx
getreg_cr0: getreg_cr0.c
	${CC} ${CFLAGS_DBG} getreg_cr0.c -o getreg_cr0

# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* *.o *~
