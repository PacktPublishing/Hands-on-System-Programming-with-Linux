# Makefile : solutions_to_assgn/ch3 
#----------------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#----------------------------------------------------------------------
#  ASSUMPTIONS ::
#   1. the convenience files ../../common.h and ../../common.c
#      are present
#   2. the clang/LLVM compiler is installed
#   3. the indent utility is installed
#   WARNING! Do NOT start a source filename with 'core' !
#       (will get Erased when 'make clean' is performed).
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired.
ALL :=  segflt_nocore segflt_nocore_dbg #segflt_nocore_dbg_asan segflt_nocore_dbg_ub segflt_nocore_dbg_msan

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

LINKIN :=
 # user will need to explicitly set libraries to link in as required;
 # f.e. -lrt -pthread

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../../common.c ../../common.h
	${CC} ${CFLAGS} -c ../../common.c -o common.o
common_dbg.o: ../../common.c ../../common.h
	${CC} ${CFLAGS_DBG} -c ../../common.c -o common_dbg.o

 #--- Sanitizers (use clang): common_dbg_*
common_dbg_asan.o: ../../common.c ../../common.h
	${CL} ${CFLAGS_DBG_ASAN} -c ../../common.c -o common_dbg_asan.o
common_dbg_ub.o: ../../common.c ../../common.h
	${CL} ${CFLAGS_DBG_UB} -c ../../common.c -o common_dbg_ub.o
common_dbg_msan.o: ../../common.c ../../common.h
	${CL} ${CFLAGS_DBG_MSAN} -c ../../common.c -o common_dbg_msan.o

#--- Target :: segflt_nocore
segflt_nocore.o: segflt_nocore.c
	${CC} ${CFLAGS} -c segflt_nocore.c -o segflt_nocore.o
segflt_nocore: common.o segflt_nocore.o
	${CC} -o segflt_nocore segflt_nocore.o common.o ${LINKIN}

segflt_nocore_dbg.o: segflt_nocore.c
	${CC} ${CFLAGS_DBG} -c segflt_nocore.c -o segflt_nocore_dbg.o
segflt_nocore_dbg: segflt_nocore_dbg.o common_dbg.o
	${CC} -o segflt_nocore_dbg segflt_nocore_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for segflt_nocore :: (use clang): <foo>_dbg_[asan|ub|msan]
segflt_nocore_dbg_asan.o: segflt_nocore.c
	${CL} ${CFLAGS_DBG_ASAN} -c segflt_nocore.c -o segflt_nocore_dbg_asan.o
segflt_nocore_dbg_asan: segflt_nocore_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o segflt_nocore_dbg_asan segflt_nocore_dbg_asan.o common_dbg_asan.o ${LINKIN}

segflt_nocore_dbg_ub.o: segflt_nocore.c
	${CL} ${CFLAGS_DBG_UB} -c segflt_nocore.c -o segflt_nocore_dbg_ub.o
segflt_nocore_dbg_ub: segflt_nocore_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o segflt_nocore_dbg_ub segflt_nocore_dbg_ub.o common_dbg_ub.o ${LINKIN}

segflt_nocore_dbg_msan.o: segflt_nocore.c
	${CL} ${CFLAGS_DBG_MSAN} -c segflt_nocore.c -o segflt_nocore_dbg_msan.o
segflt_nocore_dbg_msan: segflt_nocore_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o segflt_nocore_dbg_msan segflt_nocore_dbg_msan.o common_dbg_msan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
