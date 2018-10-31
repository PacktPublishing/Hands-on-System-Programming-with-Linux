# Makefile : Ch 13
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 13 : Timers
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
ALL :=  alarm1 alarm1_dbg \
	alarm2_rep alarm2_rep_dbg \
	intv_clksimple intv_clksimple_dbg \
	react react_dbg \
	runwalk_timer runwalk_timer_dbg #runwalk_timer_dbg_asan

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

LINKIN=
   # set as required to libraries to link in; f.e.  -lrt

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} ${CFLAGS} -c ../common.c -o common.o
common_dbg.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG} -c ../common.c -o common_dbg.o
#--- Sanitizers (use clang): common_dbg_*
common_dbg_asan.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_ASAN} -c ../common.c -o common_dbg_asan.o

#--- Target :: alarm1
alarm1.o: alarm1.c
	${CC} ${CFLAGS} -c alarm1.c -o alarm1.o
alarm1: common.o alarm1.o
	${CC} -o alarm1 alarm1.o common.o ${LINKIN}

alarm1_dbg.o: alarm1.c
	${CC} ${CFLAGS_DBG} -c alarm1.c -o alarm1_dbg.o
alarm1_dbg: alarm1_dbg.o common_dbg.o
	${CC} -o alarm1_dbg alarm1_dbg.o common_dbg.o ${LINKIN}

#--- Target :: alarm2_rep
alarm2_rep.o: alarm2_rep.c
	${CC} ${CFLAGS} -c alarm2_rep.c -o alarm2_rep.o
alarm2_rep: common.o alarm2_rep.o
	${CC} -o alarm2_rep alarm2_rep.o common.o ${LINKIN}

alarm2_rep_dbg.o: alarm2_rep.c
	${CC} ${CFLAGS_DBG} -c alarm2_rep.c -o alarm2_rep_dbg.o
alarm2_rep_dbg: alarm2_rep_dbg.o common_dbg.o
	${CC} -o alarm2_rep_dbg alarm2_rep_dbg.o common_dbg.o ${LINKIN}

#--- Target :: intv_clksimple
intv_clksimple.o: intv_clksimple.c
	${CC} ${CFLAGS} -c intv_clksimple.c -o intv_clksimple.o
intv_clksimple: common.o intv_clksimple.o
	${CC} -o intv_clksimple intv_clksimple.o common.o ${LINKIN}

intv_clksimple_dbg.o: intv_clksimple.c
	${CC} ${CFLAGS_DBG} -c intv_clksimple.c -o intv_clksimple_dbg.o
intv_clksimple_dbg: intv_clksimple_dbg.o common_dbg.o
	${CC} -o intv_clksimple_dbg intv_clksimple_dbg.o common_dbg.o ${LINKIN}

#--- Target :: react
react.o: react.c
	${CC} ${CFLAGS} -c react.c -o react.o
react: common.o react.o
	${CC} -o react react.o common.o -lrt -lm

react_dbg.o: react.c
	${CC} ${CFLAGS_DBG} -c react.c -o react_dbg.o
react_dbg: react_dbg.o common_dbg.o
	${CC} -o react_dbg react_dbg.o common_dbg.o -lrt -lm

LINKIN := -lrt
#--- Target :: runwalk_timer
runwalk_timer.o: runwalk_timer.c
	${CC} ${CFLAGS} -c runwalk_timer.c -o runwalk_timer.o
runwalk_timer: common.o runwalk_timer.o
	${CC} -o runwalk_timer runwalk_timer.o common.o ${LINKIN}

runwalk_timer_dbg.o: runwalk_timer.c
	${CC} ${CFLAGS_DBG} -c runwalk_timer.c -o runwalk_timer_dbg.o
runwalk_timer_dbg: runwalk_timer_dbg.o common_dbg.o
	${CC} -o runwalk_timer_dbg runwalk_timer_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for runwalk_timer :: (use clang): <foo>_dbg_[asan|ub|msan]
runwalk_timer_dbg_asan.o: runwalk_timer.c
	${CL} ${CFLAGS_DBG_ASAN} -c runwalk_timer.c -o runwalk_timer_dbg_asan.o
runwalk_timer_dbg_asan: runwalk_timer_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o runwalk_timer_dbg_asan runwalk_timer_dbg_asan.o common_dbg_asan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
