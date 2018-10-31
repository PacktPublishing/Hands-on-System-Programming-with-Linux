# Makefile : B_daemon/
#----------------------------------------------------------------------
#  ASSUMPTIONS ::
#   1. the convenience files ../common.h and ../common.c
#      are present
#   2. the clang/LLVM compiler is installed
#   3. the indent utility is installed
#
#   WARNING! Do NOT start a source filename with 'core' !
#       (will get Erased when 'make clean' is performed).
#
# This program is part of the source code released for the book
#   "Hands-on System Programming with Linux"
#   (c) Author: Kaiwan N Billimoria
#   Publisher:  Packt
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired.
ALL :=  daemon_trad daemon_trad_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

#LINKIN=
 # user will need to explicitly set libraries to link in as required; f.e. -lrt

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

#--- Target :: daemon_trad
daemon_trad.o: daemon_trad.c
	${CC} ${CFLAGS} -c daemon_trad.c -o daemon_trad.o
daemon_trad: common.o daemon_trad.o
	${CC} -o daemon_trad daemon_trad.o common.o

daemon_trad_dbg.o: daemon_trad.c
	${CC} ${CFLAGS_DBG} -c daemon_trad.c -o daemon_trad_dbg.o
daemon_trad_dbg: daemon_trad_dbg.o common_dbg.o
	${CC} -o daemon_trad_dbg daemon_trad_dbg.o common_dbg.o

 #--- Sanitizers for bankacc :: (use clang): <foo>_dbg_[asan|ub|msan]
daemon_trad_dbg_asan.o: daemon_trad.c
	${CL} ${CFLAGS_DBG_ASAN} -c daemon_trad.c -o daemon_trad_dbg_asan.o
daemon_trad_dbg_asan: daemon_trad_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o daemon_trad_dbg_asan daemon_trad_dbg_asan.o common_dbg_asan.o


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~
