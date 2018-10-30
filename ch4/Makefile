# Makefile, Ch 4.
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 4 : Dynamic Memory Allocation
#----------------------------------------------------------------------
ALL := mallocmax mallocmax_dbg mallocmax32    \
  show_curbrk show_curbrk_dbg                 \
  malloc_brk_test malloc_brk_test_dbg         \
  mlock_try mlock_try_dbg                     \
  memprot memprot_dbg                         \
  alloca_try alloca_try_dbg

CROSS_COMPILE=arm-linux-gnueabihf-
CC=gcc
CCARM=${CROSS_COMPILE}gcc
CL=clang
CLARM=${CROSS_COMPILE}clang

CFLAGS=-Wall
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory -fPIE -pie

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} ${CFLAGS} -c ../common.c -o common.o
	${CC} ${CFLAGS} -m32 -c ../common.c -o common32.o
common_arm.o: ../common.c ../common.h
	${CCARM} ${CFLAGS} -c ../common.c -o common_arm.o
common_dbg.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG} -c ../common.c -o common_dbg.o

mallocmax: common.o mallocmax.o
	${CC} ${CFLAGS} -o mallocmax mallocmax.c common.o -lm
	${CC} ${CFLAGS} -m32 -o mallocmax32 mallocmax.c common32.o -lm
mallocmax_dbg.o: mallocmax.c
	${CC} ${CFLAGS_DBG} -c mallocmax.c -o mallocmax_dbg.o
mallocmax_dbg: mallocmax_dbg.o common_dbg.o
	${CC} -o mallocmax_dbg mallocmax_dbg.o common_dbg.o -lm

show_curbrk: common.o show_curbrk.o
	${CC} ${CFLAGS} -o $@ $^
show_curbrk_dbg.o: show_curbrk.c
	${CC} ${CFLAGS_DBG} -c show_curbrk.c -o show_curbrk_dbg.o
show_curbrk_dbg: show_curbrk_dbg.o common_dbg.o
	${CC} -o show_curbrk_dbg show_curbrk_dbg.o common_dbg.o

malloc_brk_test: malloc_brk_test.o common.o
	${CC} ${CFLAGS} -o $@ $^
malloc_brk_test_dbg.o: malloc_brk_test.c
	${CC} ${CFLAGS_DBG} -c malloc_brk_test.c -o malloc_brk_test_dbg.o
malloc_brk_test_dbg: malloc_brk_test_dbg.o common_dbg.o
	${CC} -o malloc_brk_test_dbg malloc_brk_test_dbg.o common_dbg.o

mlock_try: common.o mlock_try.o
	${CC} ${CFLAGS} -o mlock_try mlock_try.c common.o
mlock_try_dbg.o: mlock_try.c
	${CC} ${CFLAGS_DBG} -c mlock_try.c -o mlock_try_dbg.o
mlock_try_dbg: mlock_try_dbg.o common_dbg.o
	${CC} -o mlock_try_dbg mlock_try_dbg.o common_dbg.o

memprot: common.o memprot.o
	${CC} ${CFLAGS} -o memprot memprot.c common.o
memprot_dbg.o: memprot.c
	${CC} ${CFLAGS_DBG} -c memprot.c -o memprot_dbg.o
memprot_dbg: memprot_dbg.o common_dbg.o
	${CC} -o memprot_dbg memprot_dbg.o common_dbg.o

memprot_arm: common_arm.o memprot_arm.o
	${CCARM} ${CFLAGS} -o memprot_arm memprot_arm.c common_arm.o

alloca_try: common.o alloca_try.o
	${CC} ${CFLAGS} -o alloca_try alloca_try.c common.o
alloca_try_dbg.o: alloca_try.c
	${CC} ${CFLAGS_DBG} -c alloca_try.c -o alloca_try_dbg.o
alloca_try_dbg: alloca_try_dbg.o common_dbg.o
	${CC} -o alloca_try_dbg alloca_try_dbg.o common_dbg.o


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
