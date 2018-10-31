# Makefile : solutions_to_assgn/ch11
#**************************************************************
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#***************************************************************

ALL := donthup atomic_A atomic_B

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

donthup: common.o donthup.o
	${CC} ${CFLAGS} -o donthup donthup.c common.o
donthup_dbg.o: donthup.c
	${CC} ${CFLAGS_DBG} -c donthup.c -o donthup_dbg.o
donthup_dbg: donthup_dbg.o common_dbg.o
	${CC} -o donthup_dbg donthup_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_ASAN} -o donthup_asan donthup_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_UB} -o donthup_ub donthup_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_MSAN} -o donthup_msan donthup_dbg.o common_dbg.o

atomic_A: common.o atomic_A.o
	${CC} ${CFLAGS} -o atomic_A atomic_A.c common.o
atomic_A_dbg.o: atomic_A.c
	${CC} ${CFLAGS_DBG} -c atomic_A.c -o atomic_A_dbg.o
atomic_A_dbg: atomic_A_dbg.o common_dbg.o
	${CC} -o atomic_A_dbg atomic_A_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_ASAN} -o atomic_A_asan atomic_A_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_UB} -o atomic_A_ub atomic_A_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_MSAN} -o atomic_A_msan atomic_A_dbg.o common_dbg.o

atomic_B: common.o atomic_B.o
	${CC} ${CFLAGS} -o atomic_B atomic_B.c common.o
atomic_B_dbg.o: atomic_B.c
	${CC} ${CFLAGS_DBG} -c atomic_B.c -o atomic_B_dbg.o
atomic_B_dbg: atomic_B_dbg.o common_dbg.o
	${CC} -o atomic_B_dbg atomic_B_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_ASAN} -o atomic_B_asan atomic_B_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_UB} -o atomic_B_ub atomic_B_dbg.o common_dbg.o
	${CL} ${CFLAGS_DBG_MSAN} -o atomic_B_msan atomic_B_dbg.o common_dbg.o

# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
