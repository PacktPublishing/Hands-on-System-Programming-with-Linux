# Makefile, Ch 11.
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From:  Ch 11 : Signaling Part I
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

#ALL :=  sig1 sig1_dbg sig1_dbg_asan sig1_dbg_ub sig1_dbg_msan
ALL :=  sig1 sig1_dbg \
	sig2 sig2_dbg \
	query_mask query_mask_dbg \
	zombies_clear_linux26 zombies_clear_linux26_dbg \
	defer_or_not defer_or_not_dbg \
	altstack altstack_dbg

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

#--- Target :: sig1
sig1.o: sig1.c
	${CC} ${CFLAGS} -c sig1.c -o sig1.o
sig1: common.o sig1.o
	${CC} -o sig1 sig1.o common.o ${LINKIN}

sig1_dbg.o: sig1.c
	${CC} ${CFLAGS_DBG} -c sig1.c -o sig1_dbg.o
sig1_dbg: sig1_dbg.o common_dbg.o
	${CC} -o sig1_dbg sig1_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for sig1 :: (use clang): <foo>_dbg_[asan|ub|msan]
sig1_dbg_asan.o: sig1.c
	${CL} ${CFLAGS_DBG_ASAN} -c sig1.c -o sig1_dbg_asan.o
sig1_dbg_asan: sig1_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o sig1_dbg_asan sig1_dbg_asan.o common_dbg_asan.o ${LINKIN}

sig1_dbg_ub.o: sig1.c
	${CL} ${CFLAGS_DBG_UB} -c sig1.c -o sig1_dbg_ub.o
sig1_dbg_ub: sig1_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o sig1_dbg_ub sig1_dbg_ub.o common_dbg_ub.o ${LINKIN}

sig1_dbg_msan.o: sig1.c
	${CL} ${CFLAGS_DBG_MSAN} -c sig1.c -o sig1_dbg_msan.o
sig1_dbg_msan: sig1_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o sig1_dbg_msan sig1_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: sig2
sig2.o: sig2.c
	${CC} ${CFLAGS} -c sig2.c -o sig2.o
sig2: common.o sig2.o
	${CC} -o sig2 sig2.o common.o ${LINKIN}

sig2_dbg.o: sig2.c
	${CC} ${CFLAGS_DBG} -c sig2.c -o sig2_dbg.o
sig2_dbg: sig2_dbg.o common_dbg.o
	${CC} -o sig2_dbg sig2_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for sig2 :: (use clang): <foo>_dbg_[asan|ub|msan]
sig2_dbg_asan.o: sig2.c
	${CL} ${CFLAGS_DBG_ASAN} -c sig2.c -o sig2_dbg_asan.o
sig2_dbg_asan: sig2_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o sig2_dbg_asan sig2_dbg_asan.o common_dbg_asan.o ${LINKIN}

sig2_dbg_ub.o: sig2.c
	${CL} ${CFLAGS_DBG_UB} -c sig2.c -o sig2_dbg_ub.o
sig2_dbg_ub: sig2_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o sig2_dbg_ub sig2_dbg_ub.o common_dbg_ub.o ${LINKIN}

sig2_dbg_msan.o: sig2.c
	${CL} ${CFLAGS_DBG_MSAN} -c sig2.c -o sig2_dbg_msan.o
sig2_dbg_msan: sig2_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o sig2_dbg_msan sig2_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: query_mask
query_mask.o: query_mask.c
	${CC} ${CFLAGS} -c query_mask.c -o query_mask.o
query_mask: common.o query_mask.o
	${CC} -o query_mask query_mask.o common.o ${LINKIN}

query_mask_dbg.o: query_mask.c
	${CC} ${CFLAGS_DBG} -c query_mask.c -o query_mask_dbg.o
query_mask_dbg: query_mask_dbg.o common_dbg.o
	${CC} -o query_mask_dbg query_mask_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for query_mask :: (use clang): <foo>_dbg_[asan|ub|msan]
query_mask_dbg_asan.o: query_mask.c
	${CL} ${CFLAGS_DBG_ASAN} -c query_mask.c -o query_mask_dbg_asan.o
query_mask_dbg_asan: query_mask_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o query_mask_dbg_asan query_mask_dbg_asan.o common_dbg_asan.o ${LINKIN}

query_mask_dbg_ub.o: query_mask.c
	${CL} ${CFLAGS_DBG_UB} -c query_mask.c -o query_mask_dbg_ub.o
query_mask_dbg_ub: query_mask_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o query_mask_dbg_ub query_mask_dbg_ub.o common_dbg_ub.o ${LINKIN}

query_mask_dbg_msan.o: query_mask.c
	${CL} ${CFLAGS_DBG_MSAN} -c query_mask.c -o query_mask_dbg_msan.o
query_mask_dbg_msan: query_mask_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o query_mask_dbg_msan query_mask_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: zombies_clear_linux26
zombies_clear_linux26.o: zombies_clear_linux26.c
	${CC} ${CFLAGS} -c zombies_clear_linux26.c -o zombies_clear_linux26.o
zombies_clear_linux26: common.o zombies_clear_linux26.o
	${CC} -o zombies_clear_linux26 zombies_clear_linux26.o common.o ${LINKIN}

zombies_clear_linux26_dbg.o: zombies_clear_linux26.c
	${CC} ${CFLAGS_DBG} -c zombies_clear_linux26.c -o zombies_clear_linux26_dbg.o
zombies_clear_linux26_dbg: zombies_clear_linux26_dbg.o common_dbg.o
	${CC} -o zombies_clear_linux26_dbg zombies_clear_linux26_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for zombies_clear_linux26 :: (use clang): <foo>_dbg_[asan|ub|msan]
zombies_clear_linux26_dbg_asan.o: zombies_clear_linux26.c
	${CL} ${CFLAGS_DBG_ASAN} -c zombies_clear_linux26.c -o zombies_clear_linux26_dbg_asan.o
zombies_clear_linux26_dbg_asan: zombies_clear_linux26_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o zombies_clear_linux26_dbg_asan zombies_clear_linux26_dbg_asan.o common_dbg_asan.o ${LINKIN}

zombies_clear_linux26_dbg_ub.o: zombies_clear_linux26.c
	${CL} ${CFLAGS_DBG_UB} -c zombies_clear_linux26.c -o zombies_clear_linux26_dbg_ub.o
zombies_clear_linux26_dbg_ub: zombies_clear_linux26_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o zombies_clear_linux26_dbg_ub zombies_clear_linux26_dbg_ub.o common_dbg_ub.o ${LINKIN}

zombies_clear_linux26_dbg_msan.o: zombies_clear_linux26.c
	${CL} ${CFLAGS_DBG_MSAN} -c zombies_clear_linux26.c -o zombies_clear_linux26_dbg_msan.o
zombies_clear_linux26_dbg_msan: zombies_clear_linux26_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o zombies_clear_linux26_dbg_msan zombies_clear_linux26_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: defer_or_not
defer_or_not.o: defer_or_not.c
	${CC} ${CFLAGS} -c defer_or_not.c -o defer_or_not.o
defer_or_not: common.o defer_or_not.o
	${CC} -o defer_or_not defer_or_not.o common.o ${LINKIN}

defer_or_not_dbg.o: defer_or_not.c
	${CC} ${CFLAGS_DBG} -c defer_or_not.c -o defer_or_not_dbg.o
defer_or_not_dbg: defer_or_not_dbg.o common_dbg.o
	${CC} -o defer_or_not_dbg defer_or_not_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for defer_or_not :: (use clang): <foo>_dbg_[asan|ub|msan]
defer_or_not_dbg_asan.o: defer_or_not.c
	${CL} ${CFLAGS_DBG_ASAN} -c defer_or_not.c -o defer_or_not_dbg_asan.o
defer_or_not_dbg_asan: defer_or_not_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o defer_or_not_dbg_asan defer_or_not_dbg_asan.o common_dbg_asan.o ${LINKIN}

defer_or_not_dbg_ub.o: defer_or_not.c
	${CL} ${CFLAGS_DBG_UB} -c defer_or_not.c -o defer_or_not_dbg_ub.o
defer_or_not_dbg_ub: defer_or_not_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o defer_or_not_dbg_ub defer_or_not_dbg_ub.o common_dbg_ub.o ${LINKIN}

defer_or_not_dbg_msan.o: defer_or_not.c
	${CL} ${CFLAGS_DBG_MSAN} -c defer_or_not.c -o defer_or_not_dbg_msan.o
defer_or_not_dbg_msan: defer_or_not_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o defer_or_not_dbg_msan defer_or_not_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: altstack
altstack.o: altstack.c
	${CC} ${CFLAGS} -c altstack.c -o altstack.o
altstack: common.o altstack.o
	${CC} -o altstack altstack.o common.o ${LINKIN}

altstack_dbg.o: altstack.c
	${CC} ${CFLAGS_DBG} -c altstack.c -o altstack_dbg.o
altstack_dbg: altstack_dbg.o common_dbg.o
	${CC} -o altstack_dbg altstack_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for altstack :: (use clang): <foo>_dbg_[asan|ub|msan]
altstack_dbg_asan.o: altstack.c
	${CL} ${CFLAGS_DBG_ASAN} -c altstack.c -o altstack_dbg_asan.o
altstack_dbg_asan: altstack_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o altstack_dbg_asan altstack_dbg_asan.o common_dbg_asan.o ${LINKIN}

altstack_dbg_ub.o: altstack.c
	${CL} ${CFLAGS_DBG_UB} -c altstack.c -o altstack_dbg_ub.o
altstack_dbg_ub: altstack_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o altstack_dbg_ub altstack_dbg_ub.o common_dbg_ub.o ${LINKIN}

altstack_dbg_msan.o: altstack.c
	${CL} ${CFLAGS_DBG_MSAN} -c altstack.c -o altstack_dbg_msan.o
altstack_dbg_msan: altstack_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o altstack_dbg_msan altstack_dbg_msan.o common_dbg_msan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
