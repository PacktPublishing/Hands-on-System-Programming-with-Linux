# Makefile : ch14/speed_multiprcs_vs_multithrd_simple/create_destroy/
# Book: Hands-On System Programming with Linux, ch14
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
ALL :=  fork_test fork_test_dbg \
	pthread_test pthread_test_dbg #pthread_test_dbg_asan pthread_test_dbg_ub pthread_test_dbg_msan
		#fork_test_dbg_asan fork_test_dbg_ub fork_test_dbg_msan

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

common.o: ../../../common.c ../../../common.h
	${CC} ${CFLAGS} -c ../../../common.c -o common.o
common_dbg.o: ../../../common.c ../../../common.h
	${CC} ${CFLAGS_DBG} -c ../../../common.c -o common_dbg.o

 #--- Sanitizers (use clang): common_dbg_*
common_dbg_asan.o: ../../../common.c ../../../common.h
	${CL} ${CFLAGS_DBG_ASAN} -c ../../../common.c -o common_dbg_asan.o
common_dbg_ub.o: ../../../common.c ../../../common.h
	${CL} ${CFLAGS_DBG_UB} -c ../../../common.c -o common_dbg_ub.o
common_dbg_msan.o: ../../../common.c ../../../common.h
	${CL} ${CFLAGS_DBG_MSAN} -c ../../../common.c -o common_dbg_msan.o

#--- Target :: fork_test
fork_test.o: fork_test.c
	${CC} ${CFLAGS} -c fork_test.c -o fork_test.o
fork_test: common.o fork_test.o
	${CC} -o fork_test fork_test.o common.o ${LINKIN}

fork_test_dbg.o: fork_test.c
	${CC} ${CFLAGS_DBG} -c fork_test.c -o fork_test_dbg.o
fork_test_dbg: fork_test_dbg.o common_dbg.o
	${CC} -o fork_test_dbg fork_test_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork_test :: (use clang): <foo>_dbg_[asan|ub|msan]
fork_test_dbg_asan.o: fork_test.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork_test.c -o fork_test_dbg_asan.o
fork_test_dbg_asan: fork_test_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork_test_dbg_asan fork_test_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork_test_dbg_ub.o: fork_test.c
	${CL} ${CFLAGS_DBG_UB} -c fork_test.c -o fork_test_dbg_ub.o
fork_test_dbg_ub: fork_test_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork_test_dbg_ub fork_test_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork_test_dbg_msan.o: fork_test.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork_test.c -o fork_test_dbg_msan.o
fork_test_dbg_msan: fork_test_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork_test_dbg_msan fork_test_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: pthread_test
LINKIN=-lpthread
pthread_test.o: pthread_test.c
	${CC} ${CFLAGS} -c pthread_test.c -o pthread_test.o
pthread_test: common.o pthread_test.o
	${CC} -o pthread_test pthread_test.o common.o ${LINKIN}

pthread_test_dbg.o: pthread_test.c
	${CC} ${CFLAGS_DBG} -c pthread_test.c -o pthread_test_dbg.o
pthread_test_dbg: pthread_test_dbg.o common_dbg.o
	${CC} -o pthread_test_dbg pthread_test_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for pthread_test :: (use clang): <foo>_dbg_[asan|ub|msan]
pthread_test_dbg_asan.o: pthread_test.c
	${CL} ${CFLAGS_DBG_ASAN} -c pthread_test.c -o pthread_test_dbg_asan.o
pthread_test_dbg_asan: pthread_test_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o pthread_test_dbg_asan pthread_test_dbg_asan.o common_dbg_asan.o ${LINKIN}

pthread_test_dbg_ub.o: pthread_test.c
	${CL} ${CFLAGS_DBG_UB} -c pthread_test.c -o pthread_test_dbg_ub.o
pthread_test_dbg_ub: pthread_test_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o pthread_test_dbg_ub pthread_test_dbg_ub.o common_dbg_ub.o ${LINKIN}

pthread_test_dbg_msan.o: pthread_test.c
	${CL} ${CFLAGS_DBG_MSAN} -c pthread_test.c -o pthread_test_dbg_msan.o
pthread_test_dbg_msan: pthread_test_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o pthread_test_dbg_msan pthread_test_dbg_msan.o common_dbg_msan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
