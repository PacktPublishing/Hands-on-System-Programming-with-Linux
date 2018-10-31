# Makefile : ch14/speed_multiprcs_vs_multithrd_simple/matrixmul
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
ALL :=  prcs_matrixmul prcs_matrixmul_dbg \
	thrd_matrixmul thrd_matrixmul_dbg #thrd_matrixmul_dbg_asan thrd_matrixmul_dbg_ub thrd_matrixmul_dbg_msan \
  #prcs_matrixmul_dbg_asan prcs_matrixmul_dbg_ub prcs_matrixmul_dbg_msan \

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

#--- Target :: prcs_matrixmul
LINKIN := -pthread
prcs_matrixmul.o: prcs_matrixmul.c
	${CC} ${CFLAGS} -c prcs_matrixmul.c -o prcs_matrixmul.o
prcs_matrixmul: common.o prcs_matrixmul.o
	${CC} -o prcs_matrixmul prcs_matrixmul.o common.o ${LINKIN}

prcs_matrixmul_dbg.o: prcs_matrixmul.c
	${CC} ${CFLAGS_DBG} -c prcs_matrixmul.c -o prcs_matrixmul_dbg.o
prcs_matrixmul_dbg: prcs_matrixmul_dbg.o common_dbg.o
	${CC} -o prcs_matrixmul_dbg prcs_matrixmul_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for prcs_matrixmul :: (use clang): <foo>_dbg_[asan|ub|msan]
prcs_matrixmul_dbg_asan.o: prcs_matrixmul.c
	${CL} ${CFLAGS_DBG_ASAN} -c prcs_matrixmul.c -o prcs_matrixmul_dbg_asan.o
prcs_matrixmul_dbg_asan: prcs_matrixmul_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o prcs_matrixmul_dbg_asan prcs_matrixmul_dbg_asan.o common_dbg_asan.o ${LINKIN}

prcs_matrixmul_dbg_ub.o: prcs_matrixmul.c
	${CL} ${CFLAGS_DBG_UB} -c prcs_matrixmul.c -o prcs_matrixmul_dbg_ub.o
prcs_matrixmul_dbg_ub: prcs_matrixmul_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o prcs_matrixmul_dbg_ub prcs_matrixmul_dbg_ub.o common_dbg_ub.o ${LINKIN}

prcs_matrixmul_dbg_msan.o: prcs_matrixmul.c
	${CL} ${CFLAGS_DBG_MSAN} -c prcs_matrixmul.c -o prcs_matrixmul_dbg_msan.o
prcs_matrixmul_dbg_msan: prcs_matrixmul_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o prcs_matrixmul_dbg_msan prcs_matrixmul_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: thrd_matrixmul
LINKIN := -lpthread
thrd_matrixmul.o: thrd_matrixmul.c
	${CC} ${CFLAGS} -c thrd_matrixmul.c -o thrd_matrixmul.o
thrd_matrixmul: common.o thrd_matrixmul.o
	${CC} -o thrd_matrixmul thrd_matrixmul.o common.o ${LINKIN}

thrd_matrixmul_dbg.o: thrd_matrixmul.c
	${CC} ${CFLAGS_DBG} -c thrd_matrixmul.c -o thrd_matrixmul_dbg.o
thrd_matrixmul_dbg: thrd_matrixmul_dbg.o common_dbg.o
	${CC} -o thrd_matrixmul_dbg thrd_matrixmul_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for thrd_matrixmul :: (use clang): <foo>_dbg_[asan|ub|msan]
thrd_matrixmul_dbg_asan.o: thrd_matrixmul.c
	${CL} ${CFLAGS_DBG_ASAN} -c thrd_matrixmul.c -o thrd_matrixmul_dbg_asan.o
thrd_matrixmul_dbg_asan: thrd_matrixmul_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o thrd_matrixmul_dbg_asan thrd_matrixmul_dbg_asan.o common_dbg_asan.o ${LINKIN}

thrd_matrixmul_dbg_ub.o: thrd_matrixmul.c
	${CL} ${CFLAGS_DBG_UB} -c thrd_matrixmul.c -o thrd_matrixmul_dbg_ub.o
thrd_matrixmul_dbg_ub: thrd_matrixmul_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o thrd_matrixmul_dbg_ub thrd_matrixmul_dbg_ub.o common_dbg_ub.o ${LINKIN}

thrd_matrixmul_dbg_msan.o: thrd_matrixmul.c
	${CL} ${CFLAGS_DBG_MSAN} -c thrd_matrixmul.c -o thrd_matrixmul_dbg_msan.o
thrd_matrixmul_dbg_msan: thrd_matrixmul_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o thrd_matrixmul_dbg_msan thrd_matrixmul_dbg_msan.o common_dbg_msan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
