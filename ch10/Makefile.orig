# Makefile
#  ch10 : fork
ALL := fork1 fork2 fork3 fork4 fork5 \
	fork4_prnum fork_r6_of \
	fork_malloc_test fork_malloc_test_dbg \
	simpsh_v1 simpsh_v1_dbg \
	simpsh_v2 simpsh_v2_dbg \
	simpsh_v3 simpsh_v3_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-Wall
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
CFLAGS_DBG_ASAN= -fsanitize=address
CFLAGS_DBG_UB= -fsanitize=undefined
CFLAGS_DBG_MSAN= -fsanitize=memory -fPIE -pie

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} ${CFLAGS} -c ../common.c -o common.o
common_dbg.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG} -c ../common.c -o common_dbg.o


fork1: common.o fork1.o
	${CC} ${CFLAGS} -o fork1 fork1.c common.o
fork2: common.o fork2.o
	${CC} ${CFLAGS} -o fork2 fork2.c common.o
fork3: common.o fork3.o
	${CC} ${CFLAGS} -o fork3 fork3.c common.o
fork4: common.o fork4.o
	${CC} ${CFLAGS} -o fork4 fork4.c common.o
fork4_prnum: common.o fork4_prnum.o
	${CC} ${CFLAGS} -o fork4_prnum fork4_prnum.c common.o
fork5: common.o fork5.o
	${CC} ${CFLAGS} -o fork5 fork5.c common.o

fork_r6_of: common.o fork_r6_of.o
	${CC} ${CFLAGS} -o fork_r6_of fork_r6_of.c common.o
fork_r6_of_dbg.o: fork_r6_of.c
	${CC} ${CFLAGS_DBG} -c fork_r6_of.c -o fork_r6_of_dbg.o
fork_r6_of_dbg: fork_r6_of_dbg.o common_dbg.o
	${CC} -o fork_r6_of_dbg fork_r6_of_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_ASAN} -o fork_r6_of_asan fork_r6_of_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_UB} -o fork_r6_of_ub fork_r6_of_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_MSAN} -o fork_r6_of_msan fork_r6_of_dbg.o common_dbg.o

fork_malloc_test: common.o fork_malloc_test.o
	${CC} ${CFLAGS} -o fork_malloc_test fork_malloc_test.c common.o
fork_malloc_test_dbg.o: fork_malloc_test.c
	${CC} ${CFLAGS_DBG} -c fork_malloc_test.c -o fork_malloc_test_dbg.o
fork_malloc_test_dbg: fork_malloc_test_dbg.o common_dbg.o
	${CC} -o fork_malloc_test_dbg fork_malloc_test_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_ASAN} -o fork_malloc_test_asan fork_malloc_test_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_UB} -o fork_malloc_test_ub fork_malloc_test_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_MSAN} -o fork_malloc_test_msan fork_malloc_test_dbg.o common_dbg.o

simpsh_v1: common.o simpsh_v1.o
	${CC} ${CFLAGS} -o simpsh_v1 simpsh_v1.c common.o
simpsh_v1_dbg.o: simpsh_v1.c
	${CC} ${CFLAGS_DBG} -c simpsh_v1.c -o simpsh_v1_dbg.o
simpsh_v1_dbg: simpsh_v1_dbg.o common_dbg.o
	${CC} -o simpsh_v1_dbg simpsh_v1_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_ASAN} -o simpsh_v1_asan simpsh_v1_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_UB} -o simpsh_v1_ub simpsh_v1_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_MSAN} -o simpsh_v1_msan simpsh_v1_dbg.o common_dbg.o

simpsh_v2: common.o simpsh_v2.o
	${CC} ${CFLAGS} -o simpsh_v2 simpsh_v2.c common.o
simpsh_v2_dbg.o: simpsh_v2.c
	${CC} ${CFLAGS_DBG} -c simpsh_v2.c -o simpsh_v2_dbg.o
simpsh_v2_dbg: simpsh_v2_dbg.o common_dbg.o
	${CC} -o simpsh_v2_dbg simpsh_v2_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_ASAN} -o simpsh_v2_asan simpsh_v2_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_UB} -o simpsh_v2_ub simpsh_v2_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_MSAN} -o simpsh_v2_msan simpsh_v2_dbg.o common_dbg.o

simpsh_v3: common.o simpsh_v3.o
	${CC} ${CFLAGS} -o simpsh_v3 simpsh_v3.c common.o
simpsh_v3_dbg.o: simpsh_v3.c
	${CC} ${CFLAGS_DBG} -c simpsh_v3.c -o simpsh_v3_dbg.o
simpsh_v3_dbg: simpsh_v3_dbg.o common_dbg.o
	${CC} -o simpsh_v3_dbg simpsh_v3_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_ASAN} -o simpsh_v3_asan simpsh_v3_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_UB} -o simpsh_v3_ub simpsh_v3_dbg.o common_dbg.o
#	${CL} ${CFLAGS_DBG_MSAN} -o simpsh_v3_msan simpsh_v3_dbg.o common_dbg.o


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
