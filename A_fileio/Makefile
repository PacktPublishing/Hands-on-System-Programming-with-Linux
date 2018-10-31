# Makefile : A_iobuf/
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
#  For Ch A : File IO Part I - The Essentials
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired. We have deliberately not kept the sanitizer targets; add
## them to the 'ALL := ' statement if you wish to build them by default.

ALL :=  streamio1 streamio1_dbg \
	redirct redirct_dbg \
	fork_printf_1 fork_printf_1_dbg \
	fork_printf_2 fork_printf_2_dbg \
	iobuf iobuf_dbg \
	io_setbuf io_setbuf_dbg \
	mode_def mode_def_dbg \
	simpcp1 simpcp1_dbg \
	simpcp2 simpcp2_dbg

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

#--- Target :: streamio1
streamio1.o: streamio1.c
	${CC} ${CFLAGS} -c streamio1.c -o streamio1.o
streamio1: common.o streamio1.o
	${CC} -o streamio1 streamio1.o common.o ${LINKIN}

streamio1_dbg.o: streamio1.c
	${CC} ${CFLAGS_DBG} -c streamio1.c -o streamio1_dbg.o
streamio1_dbg: streamio1_dbg.o common_dbg.o
	${CC} -o streamio1_dbg streamio1_dbg.o common_dbg.o ${LINKIN}

#--- Target :: redirct
redirct.o: redirct.c
	${CC} ${CFLAGS} -c redirct.c -o redirct.o
redirct: common.o redirct.o
	${CC} -o redirct redirct.o common.o ${LINKIN}

redirct_dbg.o: redirct.c
	${CC} ${CFLAGS_DBG} -c redirct.c -o redirct_dbg.o
redirct_dbg: redirct_dbg.o common_dbg.o
	${CC} -o redirct_dbg redirct_dbg.o common_dbg.o ${LINKIN}

#--- Target :: fork_printf_1
fork_printf_1.o: fork_printf_1.c
	${CC} ${CFLAGS} -c fork_printf_1.c -o fork_printf_1.o
fork_printf_1: common.o fork_printf_1.o
	${CC} -o fork_printf_1 fork_printf_1.o common.o ${LINKIN}

fork_printf_1_dbg.o: fork_printf_1.c
	${CC} ${CFLAGS_DBG} -c fork_printf_1.c -o fork_printf_1_dbg.o
fork_printf_1_dbg: fork_printf_1_dbg.o common_dbg.o
	${CC} -o fork_printf_1_dbg fork_printf_1_dbg.o common_dbg.o ${LINKIN}

#--- Target :: fork_printf_2
fork_printf_2.o: fork_printf_2.c
	${CC} ${CFLAGS} -c fork_printf_2.c -o fork_printf_2.o
fork_printf_2: common.o fork_printf_2.o
	${CC} -o fork_printf_2 fork_printf_2.o common.o ${LINKIN}

fork_printf_2_dbg.o: fork_printf_2.c
	${CC} ${CFLAGS_DBG} -c fork_printf_2.c -o fork_printf_2_dbg.o
fork_printf_2_dbg: fork_printf_2_dbg.o common_dbg.o
	${CC} -o fork_printf_2_dbg fork_printf_2_dbg.o common_dbg.o ${LINKIN}

#--- Target :: iobuf
iobuf.o: iobuf.c
	${CC} ${CFLAGS} -c iobuf.c -o iobuf.o
iobuf: common.o iobuf.o
	${CC} -o iobuf iobuf.o common.o ${LINKIN}

iobuf_dbg.o: iobuf.c
	${CC} ${CFLAGS_DBG} -c iobuf.c -o iobuf_dbg.o
iobuf_dbg: iobuf_dbg.o common_dbg.o
	${CC} -o iobuf_dbg iobuf_dbg.o common_dbg.o ${LINKIN}

#--- Target :: io_setbuf
io_setbuf.o: io_setbuf.c
	${CC} ${CFLAGS} -c io_setbuf.c -o io_setbuf.o
io_setbuf: common.o io_setbuf.o
	${CC} -o io_setbuf io_setbuf.o common.o ${LINKIN}

io_setbuf_dbg.o: io_setbuf.c
	${CC} ${CFLAGS_DBG} -c io_setbuf.c -o io_setbuf_dbg.o
io_setbuf_dbg: io_setbuf_dbg.o common_dbg.o
	${CC} -o io_setbuf_dbg io_setbuf_dbg.o common_dbg.o ${LINKIN}

#--- Target :: mode_def
mode_def.o: mode_def.c
	${CC} ${CFLAGS} -c mode_def.c -o mode_def.o
mode_def: common.o mode_def.o
	${CC} -o mode_def mode_def.o common.o ${LINKIN}

mode_def_dbg.o: mode_def.c
	${CC} ${CFLAGS_DBG} -c mode_def.c -o mode_def_dbg.o
mode_def_dbg: mode_def_dbg.o common_dbg.o
	${CC} -o mode_def_dbg mode_def_dbg.o common_dbg.o ${LINKIN}

#--- Target :: simpcp1
simpcp1.o: simpcp1.c
	${CC} ${CFLAGS} -c simpcp1.c -o simpcp1.o
simpcp1: common.o simpcp1.o
	${CC} -o simpcp1 simpcp1.o common.o ${LINKIN}

simpcp1_dbg.o: simpcp1.c
	${CC} ${CFLAGS_DBG} -c simpcp1.c -o simpcp1_dbg.o
simpcp1_dbg: simpcp1_dbg.o common_dbg.o
	${CC} -o simpcp1_dbg simpcp1_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for simpcp1 :: (use clang): <foo>_dbg_[asan|ub|msan]
simpcp1_dbg_asan.o: simpcp1.c
	${CL} ${CFLAGS_DBG_ASAN} -c simpcp1.c -o simpcp1_dbg_asan.o
simpcp1_dbg_asan: simpcp1_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o simpcp1_dbg_asan simpcp1_dbg_asan.o common_dbg_asan.o ${LINKIN}

#--- Target :: simpcp2
simpcp2.o: simpcp2.c
	${CC} ${CFLAGS} -c simpcp2.c -o simpcp2.o
simpcp2: common.o simpcp2.o
	${CC} -o simpcp2 simpcp2.o common.o ${LINKIN}

simpcp2_dbg.o: simpcp2.c
	${CC} ${CFLAGS_DBG} -c simpcp2.c -o simpcp2_dbg.o
simpcp2_dbg: simpcp2_dbg.o common_dbg.o
	${CC} -o simpcp2_dbg simpcp2_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for simpcp2 :: (use clang): <foo>_dbg_[asan|ub|msan]
simpcp2_dbg_asan.o: simpcp2.c
	${CL} ${CFLAGS_DBG_ASAN} -c simpcp2.c -o simpcp2_dbg_asan.o
simpcp2_dbg_asan: simpcp2_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o simpcp2_dbg_asan simpcp2_dbg_asan.o common_dbg_asan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~ ltrc*
