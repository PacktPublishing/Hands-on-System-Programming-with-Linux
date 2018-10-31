# Makefile : ch14/
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
ALL :=  pthreads1 pthreads1_dbg \
	pthreads2 pthreads2_dbg \
	pthreads3 pthreads3_dbg \
	cr8_so_many_threads cr8_so_many_threads_dbg \
	disp_defattr_pthread disp_defattr_pthread_dbg \
	pthreads_joiner1 pthreads_joiner1_dbg \
	pthreads_joiner2 pthreads_joiner2_dbg \
	stack_test stack_test_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG -pthread
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG -pthread
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

LINKIN := -pthread
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
common_dbg_ub.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_UB} -c ../common.c -o common_dbg_ub.o
common_dbg_msan.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_MSAN} -c ../common.c -o common_dbg_msan.o

#--- Target :: pthreads1
pthreads1.o: pthreads1.c
	${CC} ${CFLAGS} -c pthreads1.c -o pthreads1.o
pthreads1: common.o pthreads1.o
	${CC} -o pthreads1 pthreads1.o common.o ${LINKIN}

pthreads1_dbg.o: pthreads1.c
	${CC} ${CFLAGS_DBG} -c pthreads1.c -o pthreads1_dbg.o
pthreads1_dbg: pthreads1_dbg.o common_dbg.o
	${CC} -o pthreads1_dbg pthreads1_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for pthreads1 :: (use clang): <foo>_dbg_[asan|ub|msan]
pthreads1_dbg_asan.o: pthreads1.c
	${CL} ${CFLAGS_DBG_ASAN} -c pthreads1.c -o pthreads1_dbg_asan.o
pthreads1_dbg_asan: pthreads1_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o pthreads1_dbg_asan pthreads1_dbg_asan.o common_dbg_asan.o ${LINKIN}

pthreads1_dbg_ub.o: pthreads1.c
	${CL} ${CFLAGS_DBG_UB} -c pthreads1.c -o pthreads1_dbg_ub.o
pthreads1_dbg_ub: pthreads1_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o pthreads1_dbg_ub pthreads1_dbg_ub.o common_dbg_ub.o ${LINKIN}

pthreads1_dbg_msan.o: pthreads1.c
	${CL} ${CFLAGS_DBG_MSAN} -c pthreads1.c -o pthreads1_dbg_msan.o
pthreads1_dbg_msan: pthreads1_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o pthreads1_dbg_msan pthreads1_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: pthreads2
pthreads2.o: pthreads2.c
	${CC} ${CFLAGS} -c pthreads2.c -o pthreads2.o
pthreads2: common.o pthreads2.o
	${CC} -o pthreads2 pthreads2.o common.o ${LINKIN}

pthreads2_dbg.o: pthreads2.c
	${CC} ${CFLAGS_DBG} -c pthreads2.c -o pthreads2_dbg.o
pthreads2_dbg: pthreads2_dbg.o common_dbg.o
	${CC} -o pthreads2_dbg pthreads2_dbg.o common_dbg.o ${LINKIN}

#--- Target :: pthreads3
pthreads3.o: pthreads3.c
	${CC} ${CFLAGS} -c pthreads3.c -o pthreads3.o
pthreads3: common.o pthreads3.o
	${CC} -o pthreads3 pthreads3.o common.o ${LINKIN}

pthreads3_dbg.o: pthreads3.c
	${CC} ${CFLAGS_DBG} -c pthreads3.c -o pthreads3_dbg.o
pthreads3_dbg: pthreads3_dbg.o common_dbg.o
	${CC} -o pthreads3_dbg pthreads3_dbg.o common_dbg.o ${LINKIN}

#--- Target :: cr8_so_many_threads
cr8_so_many_threads.o: cr8_so_many_threads.c
	${CC} ${CFLAGS} -c cr8_so_many_threads.c -o cr8_so_many_threads.o
cr8_so_many_threads: common.o cr8_so_many_threads.o
	${CC} -o cr8_so_many_threads cr8_so_many_threads.o common.o ${LINKIN}

cr8_so_many_threads_dbg.o: cr8_so_many_threads.c
	${CC} ${CFLAGS_DBG} -c cr8_so_many_threads.c -o cr8_so_many_threads_dbg.o
cr8_so_many_threads_dbg: cr8_so_many_threads_dbg.o common_dbg.o
	${CC} -o cr8_so_many_threads_dbg cr8_so_many_threads_dbg.o common_dbg.o ${LINKIN}

#--- Target :: disp_defattr_pthread
disp_defattr_pthread.o: disp_defattr_pthread.c
	${CC} ${CFLAGS} -c disp_defattr_pthread.c -o disp_defattr_pthread.o
disp_defattr_pthread: common.o disp_defattr_pthread.o
	${CC} -o disp_defattr_pthread disp_defattr_pthread.o common.o ${LINKIN}

disp_defattr_pthread_dbg.o: disp_defattr_pthread.c
	${CC} ${CFLAGS_DBG} -c disp_defattr_pthread.c -o disp_defattr_pthread_dbg.o
disp_defattr_pthread_dbg: disp_defattr_pthread_dbg.o common_dbg.o
	${CC} -o disp_defattr_pthread_dbg disp_defattr_pthread_dbg.o common_dbg.o ${LINKIN}

#--- Target :: pthreads_joiner1
pthreads_joiner1.o: pthreads_joiner1.c
	${CC} ${CFLAGS} -c pthreads_joiner1.c -o pthreads_joiner1.o
pthreads_joiner1: common.o pthreads_joiner1.o
	${CC} -o pthreads_joiner1 pthreads_joiner1.o common.o ${LINKIN}

pthreads_joiner1_dbg.o: pthreads_joiner1.c
	${CC} ${CFLAGS_DBG} -c pthreads_joiner1.c -o pthreads_joiner1_dbg.o
pthreads_joiner1_dbg: pthreads_joiner1_dbg.o common_dbg.o
	${CC} -o pthreads_joiner1_dbg pthreads_joiner1_dbg.o common_dbg.o ${LINKIN}

#--- Target :: pthreads_joiner2
pthreads_joiner2.o: pthreads_joiner2.c
	${CC} ${CFLAGS} -c pthreads_joiner2.c -o pthreads_joiner2.o
pthreads_joiner2: common.o pthreads_joiner2.o
	${CC} -o pthreads_joiner2 pthreads_joiner2.o common.o ${LINKIN}

pthreads_joiner2_dbg.o: pthreads_joiner2.c
	${CC} ${CFLAGS_DBG} -c pthreads_joiner2.c -o pthreads_joiner2_dbg.o
pthreads_joiner2_dbg: pthreads_joiner2_dbg.o common_dbg.o
	${CC} -o pthreads_joiner2_dbg pthreads_joiner2_dbg.o common_dbg.o ${LINKIN}

#--- Target :: stack_test
stack_test.o: stack_test.c
	${CC} ${CFLAGS} -c stack_test.c -o stack_test.o
stack_test: common.o stack_test.o
	${CC} -o stack_test stack_test.o common.o ${LINKIN}

stack_test_dbg.o: stack_test.c
	${CC} ${CFLAGS_DBG} -c stack_test.c -o stack_test_dbg.o
stack_test_dbg: stack_test_dbg.o common_dbg.o
	${CC} -o stack_test_dbg stack_test_dbg.o common_dbg.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
