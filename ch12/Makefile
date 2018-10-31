# Makefile : Ch 12
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 12 : Signaling Part II
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
## as desired. (Here, we have removed from build several of the
## <foo>_dbg_[a|m|ub]san targets).
ALL :=  handle_segv handle_segv_dbg \
	sleeping_beauty sleeping_beauty_dbg \
	rtsigs_waiter rtsigs_waiter_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} ${CFLAGS} -c ../common.c -o common.o
common_dbg.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG} -c ../common.c -o common_dbg.o

#--- Sanitizers (use clang): common_dbg_*
common_dbg_asan.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_ASAN} -c ../common.c -o common_dbg_asan.o
common_dbg_ub.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_UB} -c ../common.c -o common_dbg_ub.o
common_dbg_msan.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_MSAN} -c ../common.c -o common_dbg_msan.o

#--- Target :: handle_segv
handle_segv.o: handle_segv.c
	${CC} ${CFLAGS} -c handle_segv.c -o handle_segv.o
handle_segv: common.o handle_segv.o
	${CC} ${CFLAGS} -o handle_segv handle_segv.o common.o

handle_segv_dbg.o: handle_segv.c
	${CC} ${CFLAGS_DBG} -c handle_segv.c -o handle_segv_dbg.o
handle_segv_dbg: handle_segv_dbg.o common_dbg.o
	${CC} ${CFLAGS_DBG} -o handle_segv_dbg handle_segv_dbg.o common_dbg.o

#--- Sanitizers (use clang): <foo>_dbg_[asan|ub|msan]
handle_segv_dbg_asan.o: handle_segv.c
	${CL} ${CFLAGS_DBG_ASAN} -c handle_segv.c -o handle_segv_dbg_asan.o
handle_segv_dbg_asan: handle_segv_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o handle_segv_dbg_asan handle_segv_dbg_asan.o common_dbg_asan.o

handle_segv_dbg_ub.o: handle_segv.c
	${CL} ${CFLAGS_DBG_UB} -c handle_segv.c -o handle_segv_dbg_ub.o
handle_segv_dbg_ub: handle_segv_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o handle_segv_dbg_ub handle_segv_dbg_ub.o common_dbg_ub.o

handle_segv_dbg_msan.o: handle_segv.c
	${CL} ${CFLAGS_DBG_MSAN} -c handle_segv.c -o handle_segv_dbg_msan.o
handle_segv_dbg_msan: handle_segv_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o handle_segv_dbg_msan handle_segv_dbg_msan.o common_dbg_msan.o

#--- Target :: sleeping_beauty
sleeping_beauty.o: sleeping_beauty.c
	${CC} ${CFLAGS} -c sleeping_beauty.c -o sleeping_beauty.o
sleeping_beauty: common.o sleeping_beauty.o
	${CC} ${CFLAGS} -o sleeping_beauty sleeping_beauty.o common.o

sleeping_beauty_dbg.o: sleeping_beauty.c
	${CC} ${CFLAGS_DBG} -c sleeping_beauty.c -o sleeping_beauty_dbg.o
sleeping_beauty_dbg: sleeping_beauty_dbg.o common_dbg.o
	${CC} ${CFLAGS_DBG} -o sleeping_beauty_dbg sleeping_beauty_dbg.o common_dbg.o

#--- Sanitizers (use clang): <foo>_dbg_[asan|ub|msan]
sleeping_beauty_dbg_asan.o: sleeping_beauty.c
	${CL} ${CFLAGS_DBG_ASAN} -c sleeping_beauty.c -o sleeping_beauty_dbg_asan.o
sleeping_beauty_dbg_asan: sleeping_beauty_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o sleeping_beauty_dbg_asan sleeping_beauty_dbg_asan.o common_dbg_asan.o

sleeping_beauty_dbg_ub.o: sleeping_beauty.c
	${CL} ${CFLAGS_DBG_UB} -c sleeping_beauty.c -o sleeping_beauty_dbg_ub.o
sleeping_beauty_dbg_ub: sleeping_beauty_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o sleeping_beauty_dbg_ub sleeping_beauty_dbg_ub.o common_dbg_ub.o

sleeping_beauty_dbg_msan.o: sleeping_beauty.c
	${CL} ${CFLAGS_DBG_MSAN} -c sleeping_beauty.c -o sleeping_beauty_dbg_msan.o
sleeping_beauty_dbg_msan: sleeping_beauty_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o sleeping_beauty_dbg_msan sleeping_beauty_dbg_msan.o common_dbg_msan.o

#--- Target :: rtsigs_waiter
rtsigs_waiter.o: rtsigs_waiter.c
	${CC} ${CFLAGS} -c rtsigs_waiter.c -o rtsigs_waiter.o
rtsigs_waiter: common.o rtsigs_waiter.o
	${CC} ${CFLAGS} -o rtsigs_waiter rtsigs_waiter.o common.o

rtsigs_waiter_dbg.o: rtsigs_waiter.c
	${CC} ${CFLAGS_DBG} -c rtsigs_waiter.c -o rtsigs_waiter_dbg.o
rtsigs_waiter_dbg: rtsigs_waiter_dbg.o common_dbg.o
	${CC} ${CFLAGS_DBG} -o rtsigs_waiter_dbg rtsigs_waiter_dbg.o common_dbg.o

#--- Sanitizers (use clang): <foo>_dbg_[asan|ub|msan]
rtsigs_waiter_dbg_asan.o: rtsigs_waiter.c
	${CL} ${CFLAGS_DBG_ASAN} -c rtsigs_waiter.c -o rtsigs_waiter_dbg_asan.o
rtsigs_waiter_dbg_asan: rtsigs_waiter_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o rtsigs_waiter_dbg_asan rtsigs_waiter_dbg_asan.o common_dbg_asan.o

rtsigs_waiter_dbg_ub.o: rtsigs_waiter.c
	${CL} ${CFLAGS_DBG_UB} -c rtsigs_waiter.c -o rtsigs_waiter_dbg_ub.o
rtsigs_waiter_dbg_ub: rtsigs_waiter_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o rtsigs_waiter_dbg_ub rtsigs_waiter_dbg_ub.o common_dbg_ub.o

rtsigs_waiter_dbg_msan.o: rtsigs_waiter.c
	${CL} ${CFLAGS_DBG_MSAN} -c rtsigs_waiter.c -o rtsigs_waiter_dbg_msan.o
rtsigs_waiter_dbg_msan: rtsigs_waiter_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o rtsigs_waiter_dbg_msan rtsigs_waiter_dbg_msan.o common_dbg_msan.o


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
