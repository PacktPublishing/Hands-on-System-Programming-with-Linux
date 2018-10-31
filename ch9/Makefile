# Makefile : Ch 9.
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 9 : Process Execution
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

ALL :=  execl_eg execl_eg_dbg \
	predcs_name predcs_name_dbg \
	successor_setnm successor_setnm_dbg \
	pdfrdr_exec pdfrdr_exec_dbg #pdfrdr_exec_dbg_asan pdfrdr_exec_dbg_ub pdfrdr_exec_dbg_msan

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

#--- Target :: execl_eg
execl_eg.o: execl_eg.c
	${CC} ${CFLAGS} -c execl_eg.c -o execl_eg.o
execl_eg: common.o execl_eg.o
	${CC} -o execl_eg execl_eg.o common.o ${LINKIN}

execl_eg_dbg.o: execl_eg.c
	${CC} ${CFLAGS_DBG} -c execl_eg.c -o execl_eg_dbg.o
execl_eg_dbg: execl_eg_dbg.o common_dbg.o
	${CC} -o execl_eg_dbg execl_eg_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for execl_eg :: (use clang): <foo>_dbg_[asan|ub|msan]
execl_eg_dbg_asan.o: execl_eg.c
	${CL} ${CFLAGS_DBG_ASAN} -c execl_eg.c -o execl_eg_dbg_asan.o
execl_eg_dbg_asan: execl_eg_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o execl_eg_dbg_asan execl_eg_dbg_asan.o common_dbg_asan.o ${LINKIN}

execl_eg_dbg_ub.o: execl_eg.c
	${CL} ${CFLAGS_DBG_UB} -c execl_eg.c -o execl_eg_dbg_ub.o
execl_eg_dbg_ub: execl_eg_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o execl_eg_dbg_ub execl_eg_dbg_ub.o common_dbg_ub.o ${LINKIN}

execl_eg_dbg_msan.o: execl_eg.c
	${CL} ${CFLAGS_DBG_MSAN} -c execl_eg.c -o execl_eg_dbg_msan.o
execl_eg_dbg_msan: execl_eg_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o execl_eg_dbg_msan execl_eg_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: predcs_name
predcs_name.o: predcs_name.c
	${CC} ${CFLAGS} -c predcs_name.c -o predcs_name.o
predcs_name: common.o predcs_name.o
	${CC} -o predcs_name predcs_name.o common.o ${LINKIN}

predcs_name_dbg.o: predcs_name.c
	${CC} ${CFLAGS_DBG} -c predcs_name.c -o predcs_name_dbg.o
predcs_name_dbg: predcs_name_dbg.o common_dbg.o
	${CC} -o predcs_name_dbg predcs_name_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for predcs_name :: (use clang): <foo>_dbg_[asan|ub|msan]
predcs_name_dbg_asan.o: predcs_name.c
	${CL} ${CFLAGS_DBG_ASAN} -c predcs_name.c -o predcs_name_dbg_asan.o
predcs_name_dbg_asan: predcs_name_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o predcs_name_dbg_asan predcs_name_dbg_asan.o common_dbg_asan.o ${LINKIN}

predcs_name_dbg_ub.o: predcs_name.c
	${CL} ${CFLAGS_DBG_UB} -c predcs_name.c -o predcs_name_dbg_ub.o
predcs_name_dbg_ub: predcs_name_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o predcs_name_dbg_ub predcs_name_dbg_ub.o common_dbg_ub.o ${LINKIN}

predcs_name_dbg_msan.o: predcs_name.c
	${CL} ${CFLAGS_DBG_MSAN} -c predcs_name.c -o predcs_name_dbg_msan.o
predcs_name_dbg_msan: predcs_name_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o predcs_name_dbg_msan predcs_name_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: successor_setnm
successor_setnm.o: successor_setnm.c
	${CC} ${CFLAGS} -c successor_setnm.c -o successor_setnm.o
successor_setnm: common.o successor_setnm.o
	${CC} -o successor_setnm successor_setnm.o common.o ${LINKIN}

successor_setnm_dbg.o: successor_setnm.c
	${CC} ${CFLAGS_DBG} -c successor_setnm.c -o successor_setnm_dbg.o
successor_setnm_dbg: successor_setnm_dbg.o common_dbg.o
	${CC} -o successor_setnm_dbg successor_setnm_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for successor_setnm :: (use clang): <foo>_dbg_[asan|ub|msan]
successor_setnm_dbg_asan.o: successor_setnm.c
	${CL} ${CFLAGS_DBG_ASAN} -c successor_setnm.c -o successor_setnm_dbg_asan.o
successor_setnm_dbg_asan: successor_setnm_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o successor_setnm_dbg_asan successor_setnm_dbg_asan.o common_dbg_asan.o ${LINKIN}

successor_setnm_dbg_ub.o: successor_setnm.c
	${CL} ${CFLAGS_DBG_UB} -c successor_setnm.c -o successor_setnm_dbg_ub.o
successor_setnm_dbg_ub: successor_setnm_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o successor_setnm_dbg_ub successor_setnm_dbg_ub.o common_dbg_ub.o ${LINKIN}

successor_setnm_dbg_msan.o: successor_setnm.c
	${CL} ${CFLAGS_DBG_MSAN} -c successor_setnm.c -o successor_setnm_dbg_msan.o
successor_setnm_dbg_msan: successor_setnm_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o successor_setnm_dbg_msan successor_setnm_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: pdfrdr_exec
pdfrdr_exec.o: pdfrdr_exec.c
	${CC} ${CFLAGS} -c pdfrdr_exec.c -o pdfrdr_exec.o
pdfrdr_exec: common.o pdfrdr_exec.o
	${CC} -o pdfrdr_exec pdfrdr_exec.o common.o ${LINKIN}

pdfrdr_exec_dbg.o: pdfrdr_exec.c
	${CC} ${CFLAGS_DBG} -c pdfrdr_exec.c -o pdfrdr_exec_dbg.o
pdfrdr_exec_dbg: pdfrdr_exec_dbg.o common_dbg.o
	${CC} -o pdfrdr_exec_dbg pdfrdr_exec_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for pdfrdr_exec :: (use clang): <foo>_dbg_[asan|ub|msan]
pdfrdr_exec_dbg_asan.o: pdfrdr_exec.c
	${CL} ${CFLAGS_DBG_ASAN} -c pdfrdr_exec.c -o pdfrdr_exec_dbg_asan.o
pdfrdr_exec_dbg_asan: pdfrdr_exec_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o pdfrdr_exec_dbg_asan pdfrdr_exec_dbg_asan.o common_dbg_asan.o ${LINKIN}

pdfrdr_exec_dbg_ub.o: pdfrdr_exec.c
	${CL} ${CFLAGS_DBG_UB} -c pdfrdr_exec.c -o pdfrdr_exec_dbg_ub.o
pdfrdr_exec_dbg_ub: pdfrdr_exec_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o pdfrdr_exec_dbg_ub pdfrdr_exec_dbg_ub.o common_dbg_ub.o ${LINKIN}

pdfrdr_exec_dbg_msan.o: pdfrdr_exec.c
	${CL} ${CFLAGS_DBG_MSAN} -c pdfrdr_exec.c -o pdfrdr_exec_dbg_msan.o
pdfrdr_exec_dbg_msan: pdfrdr_exec_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o pdfrdr_exec_dbg_msan pdfrdr_exec_dbg_msan.o common_dbg_msan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
