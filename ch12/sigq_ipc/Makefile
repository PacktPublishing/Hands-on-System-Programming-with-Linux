# Makefile, Ch 13
# ch13/sigq_ipc/Makefile
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 13 : Signaling Part II
#----------------------------------------------------------------------
#  ASSUMPTIONS ::
#   1. the convenience files ../common.h and ../common.c
#      are present
#   2. the clang/LLVM compiler is installed
#   3. the indent utility is installed
#   WARNING! Do NOT start a source filename with 'core' !
#       (will get Erased when 'make clean' is performed).
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired. We have deliberately not kept the sanitizer targets; add
## them to the 'ALL := ' statement if you wish to build them by default.
ALL := sigq_sender sigq_recv

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-Wall
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
CFLAGS_DBG_ASAN= -fsanitize=address
CFLAGS_DBG_MSAN= -fsanitize=memory -fPIE -pie
CFLAGS_DBG_UB= -fsanitize=undefined

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../../common.c ../../common.h
	${CC} ${CFLAGS} -c ../../common.c -o common.o
common_dbg.o: ../../common.c ../../common.h
	${CC} ${CFLAGS_DBG} -c ../../common.c -o common_dbg.o

sigq_sender: common.o sigq_sender.o
	${CC} ${CFLAGS} -o sigq_sender sigq_sender.c common.o
sigq_sender_dbg.o: sigq_sender.c
	${CC} ${CFLAGS_DBG} -c sigq_sender.c -o sigq_sender_dbg.o
sigq_sender_dbg: sigq_sender_dbg.o common_dbg.o
	${CC} -o sigq_sender_dbg sigq_sender_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_ASAN} -o sigq_sender_asan sigq_sender_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_UB} -o sigq_sender_ub sigq_sender_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_MSAN} -o sigq_sender_msan sigq_sender_dbg.o common_dbg.o

sigq_recv: common.o sigq_recv.o
	${CC} ${CFLAGS} -o sigq_recv sigq_recv.c common.o
sigq_recv_dbg.o: sigq_recv.c
	${CC} ${CFLAGS_DBG} -c sigq_recv.c -o sigq_recv_dbg.o
sigq_recv_dbg: sigq_recv_dbg.o common_dbg.o
	${CC} -o sigq_recv_dbg sigq_recv_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_ASAN} -o sigq_recv_asan sigq_recv_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_UB} -o sigq_recv_ub sigq_recv_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_MSAN} -o sigq_recv_msan sigq_recv_dbg.o common_dbg.o

# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
