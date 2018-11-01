# Makefile
#----------------------------------------------------------------------
#  ASSUMPTIONS ::
#   1. the convenience files ../common.h and ../common.c
#      are present
#   2. the clang/LLVM compiler is installed
#   3. the indent utility is installed
#
#   WARNING! Do NOT start a source filename with 'core' !
#       (will get Erased when 'make clean' is performed).
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired.
ALL :=  see_tid see_tid_dbg \
	named_threads named_threads_dbg \
	pthreads_joiner3 pthreads_joiner3_dbg \
	pthreads_joiner4 pthreads_joiner4_dbg

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

#--- Target :: see_tid
see_tid.o: see_tid.c
	${CC} ${CFLAGS} -c see_tid.c -o see_tid.o
see_tid: common.o see_tid.o
	${CC} -o see_tid see_tid.o common.o -lpthread

see_tid_dbg.o: see_tid.c
	${CC} ${CFLAGS_DBG} -c see_tid.c -o see_tid_dbg.o
see_tid_dbg: see_tid_dbg.o common_dbg.o
	${CC} -o see_tid_dbg see_tid_dbg.o common_dbg.o -lpthread

#--- Target :: named_threads
named_threads.o: named_threads.c
	${CC} ${CFLAGS} -c named_threads.c -o named_threads.o
named_threads: common.o named_threads.o
	${CC} -o named_threads named_threads.o common.o -lpthread

named_threads_dbg.o: named_threads.c
	${CC} ${CFLAGS_DBG} -c named_threads.c -o named_threads_dbg.o
named_threads_dbg: named_threads_dbg.o common_dbg.o
	${CC} -o named_threads_dbg named_threads_dbg.o common_dbg.o -lpthread

#--- Target :: pthreads_joiner3
pthreads_joiner3.o: pthreads_joiner3.c
	${CC} ${CFLAGS} -c pthreads_joiner3.c -o pthreads_joiner3.o
pthreads_joiner3: common.o pthreads_joiner3.o
	${CC} -o pthreads_joiner3 pthreads_joiner3.o common.o -lpthread

pthreads_joiner3_dbg.o: pthreads_joiner3.c
	${CC} ${CFLAGS_DBG} -c pthreads_joiner3.c -o pthreads_joiner3_dbg.o
pthreads_joiner3_dbg: pthreads_joiner3_dbg.o common_dbg.o
	${CC} -o pthreads_joiner3_dbg pthreads_joiner3_dbg.o common_dbg.o -lpthread

#--- Target :: pthreads_joiner4
pthreads_joiner4.o: pthreads_joiner4.c
	${CC} ${CFLAGS} -c pthreads_joiner4.c -o pthreads_joiner4.o
pthreads_joiner4: common.o pthreads_joiner4.o
	${CC} -o pthreads_joiner4 pthreads_joiner4.o common.o -lpthread

pthreads_joiner4_dbg.o: pthreads_joiner4.c
	${CC} ${CFLAGS_DBG} -c pthreads_joiner4.c -o pthreads_joiner4_dbg.o
pthreads_joiner4_dbg: pthreads_joiner4_dbg.o common_dbg.o
	${CC} -o pthreads_joiner4_dbg pthreads_joiner4_dbg.o common_dbg.o -lpthread

 #--- Sanitizers for pthreads_joiner4 :: (use clang): <foo>_dbg_[asan|ub|msan]
pthreads_joiner4_dbg_asan.o: pthreads_joiner4.c
	${CL} ${CFLAGS_DBG_ASAN} -c pthreads_joiner4.c -o pthreads_joiner4_dbg_asan.o
pthreads_joiner4_dbg_asan: pthreads_joiner4_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o pthreads_joiner4_dbg_asan pthreads_joiner4_dbg_asan.o common_dbg_asan.o -lpthread

pthreads_joiner4_dbg_ub.o: pthreads_joiner4.c
	${CL} ${CFLAGS_DBG_UB} -c pthreads_joiner4.c -o pthreads_joiner4_dbg_ub.o
pthreads_joiner4_dbg_ub: pthreads_joiner4_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o pthreads_joiner4_dbg_ub pthreads_joiner4_dbg_ub.o common_dbg_ub.o -lpthread

pthreads_joiner4_dbg_msan.o: pthreads_joiner4.c
	${CL} ${CFLAGS_DBG_MSAN} -c pthreads_joiner4.c -o pthreads_joiner4_dbg_msan.o
pthreads_joiner4_dbg_msan: pthreads_joiner4_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o pthreads_joiner4_dbg_msan pthreads_joiner4_dbg_msan.o common_dbg_msan.o -lpthread


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
