# Makefile, Ch 10.
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 10 : Process Creation
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
## as desired. We have deliberately not kept the sanitizer targets; add
## them to the 'ALL := ' statement if you wish to build them by default.

#ALL :=  fork1 fork1_dbg fork1_dbg_asan fork1_dbg_ub fork1_dbg_msan
ALL :=  fork1 fork1_dbg \
	fork2 fork2_dbg \
	fork3 fork3_dbg \
	fork4 fork4_dbg \
	fork4_prnum fork4_prnum_dbg \
	fork5 fork5_dbg \
	fork_r6_of fork_r6_of_dbg \
	fork_malloc_test fork_malloc_test_dbg \
	simpsh_v1 simpsh_v1_dbg \
	simpsh_v2 simpsh_v2_dbg \
	simpsh_v3 simpsh_v3_dbg

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

#--- Target :: fork1
fork1.o: fork1.c
	${CC} ${CFLAGS} -c fork1.c -o fork1.o
fork1: common.o fork1.o
	${CC} -o fork1 fork1.o common.o ${LINKIN}

fork1_dbg.o: fork1.c
	${CC} ${CFLAGS_DBG} -c fork1.c -o fork1_dbg.o
fork1_dbg: fork1_dbg.o common_dbg.o
	${CC} -o fork1_dbg fork1_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork1 :: (use clang): <foo>_dbg_[asan|ub|msan]
fork1_dbg_asan.o: fork1.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork1.c -o fork1_dbg_asan.o
fork1_dbg_asan: fork1_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork1_dbg_asan fork1_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork1_dbg_ub.o: fork1.c
	${CL} ${CFLAGS_DBG_UB} -c fork1.c -o fork1_dbg_ub.o
fork1_dbg_ub: fork1_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork1_dbg_ub fork1_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork1_dbg_msan.o: fork1.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork1.c -o fork1_dbg_msan.o
fork1_dbg_msan: fork1_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork1_dbg_msan fork1_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: fork2
fork2.o: fork2.c
	${CC} ${CFLAGS} -c fork2.c -o fork2.o
fork2: common.o fork2.o
	${CC} -o fork2 fork2.o common.o ${LINKIN}

fork2_dbg.o: fork2.c
	${CC} ${CFLAGS_DBG} -c fork2.c -o fork2_dbg.o
fork2_dbg: fork2_dbg.o common_dbg.o
	${CC} -o fork2_dbg fork2_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork2 :: (use clang): <foo>_dbg_[asan|ub|msan]
fork2_dbg_asan.o: fork2.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork2.c -o fork2_dbg_asan.o
fork2_dbg_asan: fork2_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork2_dbg_asan fork2_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork2_dbg_ub.o: fork2.c
	${CL} ${CFLAGS_DBG_UB} -c fork2.c -o fork2_dbg_ub.o
fork2_dbg_ub: fork2_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork2_dbg_ub fork2_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork2_dbg_msan.o: fork2.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork2.c -o fork2_dbg_msan.o
fork2_dbg_msan: fork2_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork2_dbg_msan fork2_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: fork3
fork3.o: fork3.c
	${CC} ${CFLAGS} -c fork3.c -o fork3.o
fork3: common.o fork3.o
	${CC} -o fork3 fork3.o common.o ${LINKIN}

fork3_dbg.o: fork3.c
	${CC} ${CFLAGS_DBG} -c fork3.c -o fork3_dbg.o
fork3_dbg: fork3_dbg.o common_dbg.o
	${CC} -o fork3_dbg fork3_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork3 :: (use clang): <foo>_dbg_[asan|ub|msan]
fork3_dbg_asan.o: fork3.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork3.c -o fork3_dbg_asan.o
fork3_dbg_asan: fork3_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork3_dbg_asan fork3_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork3_dbg_ub.o: fork3.c
	${CL} ${CFLAGS_DBG_UB} -c fork3.c -o fork3_dbg_ub.o
fork3_dbg_ub: fork3_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork3_dbg_ub fork3_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork3_dbg_msan.o: fork3.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork3.c -o fork3_dbg_msan.o
fork3_dbg_msan: fork3_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork3_dbg_msan fork3_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: fork4
fork4.o: fork4.c
	${CC} ${CFLAGS} -c fork4.c -o fork4.o
fork4: common.o fork4.o
	${CC} -o fork4 fork4.o common.o ${LINKIN}

fork4_dbg.o: fork4.c
	${CC} ${CFLAGS_DBG} -c fork4.c -o fork4_dbg.o
fork4_dbg: fork4_dbg.o common_dbg.o
	${CC} -o fork4_dbg fork4_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork4 :: (use clang): <foo>_dbg_[asan|ub|msan]
fork4_dbg_asan.o: fork4.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork4.c -o fork4_dbg_asan.o
fork4_dbg_asan: fork4_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork4_dbg_asan fork4_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork4_dbg_ub.o: fork4.c
	${CL} ${CFLAGS_DBG_UB} -c fork4.c -o fork4_dbg_ub.o
fork4_dbg_ub: fork4_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork4_dbg_ub fork4_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork4_dbg_msan.o: fork4.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork4.c -o fork4_dbg_msan.o
fork4_dbg_msan: fork4_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork4_dbg_msan fork4_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: fork4_prnum
fork4_prnum.o: fork4_prnum.c
	${CC} ${CFLAGS} -c fork4_prnum.c -o fork4_prnum.o
fork4_prnum: common.o fork4_prnum.o
	${CC} -o fork4_prnum fork4_prnum.o common.o ${LINKIN}

fork4_prnum_dbg.o: fork4_prnum.c
	${CC} ${CFLAGS_DBG} -c fork4_prnum.c -o fork4_prnum_dbg.o
fork4_prnum_dbg: fork4_prnum_dbg.o common_dbg.o
	${CC} -o fork4_prnum_dbg fork4_prnum_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork4_prnum :: (use clang): <foo>_dbg_[asan|ub|msan]
fork4_prnum_dbg_asan.o: fork4_prnum.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork4_prnum.c -o fork4_prnum_dbg_asan.o
fork4_prnum_dbg_asan: fork4_prnum_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork4_prnum_dbg_asan fork4_prnum_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork4_prnum_dbg_ub.o: fork4_prnum.c
	${CL} ${CFLAGS_DBG_UB} -c fork4_prnum.c -o fork4_prnum_dbg_ub.o
fork4_prnum_dbg_ub: fork4_prnum_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork4_prnum_dbg_ub fork4_prnum_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork4_prnum_dbg_msan.o: fork4_prnum.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork4_prnum.c -o fork4_prnum_dbg_msan.o
fork4_prnum_dbg_msan: fork4_prnum_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork4_prnum_dbg_msan fork4_prnum_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: fork5
fork5.o: fork5.c
	${CC} ${CFLAGS} -c fork5.c -o fork5.o
fork5: common.o fork5.o
	${CC} -o fork5 fork5.o common.o ${LINKIN}

fork5_dbg.o: fork5.c
	${CC} ${CFLAGS_DBG} -c fork5.c -o fork5_dbg.o
fork5_dbg: fork5_dbg.o common_dbg.o
	${CC} -o fork5_dbg fork5_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork5 :: (use clang): <foo>_dbg_[asan|ub|msan]
fork5_dbg_asan.o: fork5.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork5.c -o fork5_dbg_asan.o
fork5_dbg_asan: fork5_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork5_dbg_asan fork5_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork5_dbg_ub.o: fork5.c
	${CL} ${CFLAGS_DBG_UB} -c fork5.c -o fork5_dbg_ub.o
fork5_dbg_ub: fork5_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork5_dbg_ub fork5_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork5_dbg_msan.o: fork5.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork5.c -o fork5_dbg_msan.o
fork5_dbg_msan: fork5_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork5_dbg_msan fork5_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: fork_r6_of
fork_r6_of.o: fork_r6_of.c
	${CC} ${CFLAGS} -c fork_r6_of.c -o fork_r6_of.o
fork_r6_of: common.o fork_r6_of.o
	${CC} -o fork_r6_of fork_r6_of.o common.o ${LINKIN}

fork_r6_of_dbg.o: fork_r6_of.c
	${CC} ${CFLAGS_DBG} -c fork_r6_of.c -o fork_r6_of_dbg.o
fork_r6_of_dbg: fork_r6_of_dbg.o common_dbg.o
	${CC} -o fork_r6_of_dbg fork_r6_of_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork_r6_of :: (use clang): <foo>_dbg_[asan|ub|msan]
fork_r6_of_dbg_asan.o: fork_r6_of.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork_r6_of.c -o fork_r6_of_dbg_asan.o
fork_r6_of_dbg_asan: fork_r6_of_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork_r6_of_dbg_asan fork_r6_of_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork_r6_of_dbg_ub.o: fork_r6_of.c
	${CL} ${CFLAGS_DBG_UB} -c fork_r6_of.c -o fork_r6_of_dbg_ub.o
fork_r6_of_dbg_ub: fork_r6_of_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork_r6_of_dbg_ub fork_r6_of_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork_r6_of_dbg_msan.o: fork_r6_of.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork_r6_of.c -o fork_r6_of_dbg_msan.o
fork_r6_of_dbg_msan: fork_r6_of_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork_r6_of_dbg_msan fork_r6_of_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: fork_malloc_test
fork_malloc_test.o: fork_malloc_test.c
	${CC} ${CFLAGS} -c fork_malloc_test.c -o fork_malloc_test.o
fork_malloc_test: common.o fork_malloc_test.o
	${CC} -o fork_malloc_test fork_malloc_test.o common.o ${LINKIN}

fork_malloc_test_dbg.o: fork_malloc_test.c
	${CC} ${CFLAGS_DBG} -c fork_malloc_test.c -o fork_malloc_test_dbg.o
fork_malloc_test_dbg: fork_malloc_test_dbg.o common_dbg.o
	${CC} -o fork_malloc_test_dbg fork_malloc_test_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for fork_malloc_test :: (use clang): <foo>_dbg_[asan|ub|msan]
fork_malloc_test_dbg_asan.o: fork_malloc_test.c
	${CL} ${CFLAGS_DBG_ASAN} -c fork_malloc_test.c -o fork_malloc_test_dbg_asan.o
fork_malloc_test_dbg_asan: fork_malloc_test_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o fork_malloc_test_dbg_asan fork_malloc_test_dbg_asan.o common_dbg_asan.o ${LINKIN}

fork_malloc_test_dbg_ub.o: fork_malloc_test.c
	${CL} ${CFLAGS_DBG_UB} -c fork_malloc_test.c -o fork_malloc_test_dbg_ub.o
fork_malloc_test_dbg_ub: fork_malloc_test_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o fork_malloc_test_dbg_ub fork_malloc_test_dbg_ub.o common_dbg_ub.o ${LINKIN}

fork_malloc_test_dbg_msan.o: fork_malloc_test.c
	${CL} ${CFLAGS_DBG_MSAN} -c fork_malloc_test.c -o fork_malloc_test_dbg_msan.o
fork_malloc_test_dbg_msan: fork_malloc_test_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o fork_malloc_test_dbg_msan fork_malloc_test_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: simpsh_v1
simpsh_v1.o: simpsh_v1.c
	${CC} ${CFLAGS} -c simpsh_v1.c -o simpsh_v1.o
simpsh_v1: common.o simpsh_v1.o
	${CC} -o simpsh_v1 simpsh_v1.o common.o ${LINKIN}

simpsh_v1_dbg.o: simpsh_v1.c
	${CC} ${CFLAGS_DBG} -c simpsh_v1.c -o simpsh_v1_dbg.o
simpsh_v1_dbg: simpsh_v1_dbg.o common_dbg.o
	${CC} -o simpsh_v1_dbg simpsh_v1_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for simpsh_v1 :: (use clang): <foo>_dbg_[asan|ub|msan]
simpsh_v1_dbg_asan.o: simpsh_v1.c
	${CL} ${CFLAGS_DBG_ASAN} -c simpsh_v1.c -o simpsh_v1_dbg_asan.o
simpsh_v1_dbg_asan: simpsh_v1_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o simpsh_v1_dbg_asan simpsh_v1_dbg_asan.o common_dbg_asan.o ${LINKIN}

simpsh_v1_dbg_ub.o: simpsh_v1.c
	${CL} ${CFLAGS_DBG_UB} -c simpsh_v1.c -o simpsh_v1_dbg_ub.o
simpsh_v1_dbg_ub: simpsh_v1_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o simpsh_v1_dbg_ub simpsh_v1_dbg_ub.o common_dbg_ub.o ${LINKIN}

simpsh_v1_dbg_msan.o: simpsh_v1.c
	${CL} ${CFLAGS_DBG_MSAN} -c simpsh_v1.c -o simpsh_v1_dbg_msan.o
simpsh_v1_dbg_msan: simpsh_v1_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o simpsh_v1_dbg_msan simpsh_v1_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: simpsh_v2
simpsh_v2.o: simpsh_v2.c
	${CC} ${CFLAGS} -c simpsh_v2.c -o simpsh_v2.o
simpsh_v2: common.o simpsh_v2.o
	${CC} -o simpsh_v2 simpsh_v2.o common.o ${LINKIN}

simpsh_v2_dbg.o: simpsh_v2.c
	${CC} ${CFLAGS_DBG} -c simpsh_v2.c -o simpsh_v2_dbg.o
simpsh_v2_dbg: simpsh_v2_dbg.o common_dbg.o
	${CC} -o simpsh_v2_dbg simpsh_v2_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for simpsh_v2 :: (use clang): <foo>_dbg_[asan|ub|msan]
simpsh_v2_dbg_asan.o: simpsh_v2.c
	${CL} ${CFLAGS_DBG_ASAN} -c simpsh_v2.c -o simpsh_v2_dbg_asan.o
simpsh_v2_dbg_asan: simpsh_v2_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o simpsh_v2_dbg_asan simpsh_v2_dbg_asan.o common_dbg_asan.o ${LINKIN}

simpsh_v2_dbg_ub.o: simpsh_v2.c
	${CL} ${CFLAGS_DBG_UB} -c simpsh_v2.c -o simpsh_v2_dbg_ub.o
simpsh_v2_dbg_ub: simpsh_v2_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o simpsh_v2_dbg_ub simpsh_v2_dbg_ub.o common_dbg_ub.o ${LINKIN}

simpsh_v2_dbg_msan.o: simpsh_v2.c
	${CL} ${CFLAGS_DBG_MSAN} -c simpsh_v2.c -o simpsh_v2_dbg_msan.o
simpsh_v2_dbg_msan: simpsh_v2_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o simpsh_v2_dbg_msan simpsh_v2_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: simpsh_v3
simpsh_v3.o: simpsh_v3.c
	${CC} ${CFLAGS} -c simpsh_v3.c -o simpsh_v3.o
simpsh_v3: common.o simpsh_v3.o
	${CC} -o simpsh_v3 simpsh_v3.o common.o ${LINKIN}

simpsh_v3_dbg.o: simpsh_v3.c
	${CC} ${CFLAGS_DBG} -c simpsh_v3.c -o simpsh_v3_dbg.o
simpsh_v3_dbg: simpsh_v3_dbg.o common_dbg.o
	${CC} -o simpsh_v3_dbg simpsh_v3_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for simpsh_v3 :: (use clang): <foo>_dbg_[asan|ub|msan]
simpsh_v3_dbg_asan.o: simpsh_v3.c
	${CL} ${CFLAGS_DBG_ASAN} -c simpsh_v3.c -o simpsh_v3_dbg_asan.o
simpsh_v3_dbg_asan: simpsh_v3_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o simpsh_v3_dbg_asan simpsh_v3_dbg_asan.o common_dbg_asan.o ${LINKIN}

simpsh_v3_dbg_ub.o: simpsh_v3.c
	${CL} ${CFLAGS_DBG_UB} -c simpsh_v3.c -o simpsh_v3_dbg_ub.o
simpsh_v3_dbg_ub: simpsh_v3_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o simpsh_v3_dbg_ub simpsh_v3_dbg_ub.o common_dbg_ub.o ${LINKIN}

simpsh_v3_dbg_msan.o: simpsh_v3.c
	${CL} ${CFLAGS_DBG_MSAN} -c simpsh_v3.c -o simpsh_v3_dbg_msan.o
simpsh_v3_dbg_msan: simpsh_v3_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o simpsh_v3_dbg_msan simpsh_v3_dbg_msan.o common_dbg_msan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
